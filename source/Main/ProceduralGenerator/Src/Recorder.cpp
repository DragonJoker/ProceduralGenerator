#include "MainFrame.h"

#include <Generator.h>

#include <emmintrin.h>

#include "RenderPanel.h"

#pragma warning( push )
#	pragma warning( disable:4996 )
#	include <wx/filedlg.h>
#	include <wx/msgdlg.h>
#pragma warning( pop )
 
#if defined( PGEN_RECORDS ) && defined( __linux__ )
#	undef PGEN_RECORDS
#endif

#if defined( PGEN_RECORDS )
#	if defined( PGEN_FFMPEG )
#	define PGEN_FFMPEG_USE_STREAMS 0
extern "C"
{
#	include <libavutil/opt.h>
#	include <libavcodec/avcodec.h>
#	include <libavformat/avformat.h>
#	include <libavutil/common.h>
#	include <libavutil/imgutils.h>
#	include <libavutil/mathematics.h>
#	include <libavutil/samplefmt.h>
#	include <libswscale/swscale.h>
//#	include <libswresample/swresample.h>
}
#	elif defined( PGEN_OCV )
#	include <opencv2/opencv.hpp>
#	endif
#endif

using namespace ProceduralTextures;

namespace ProceduralGenerator
{
	namespace
	{
#if defined( PGEN_RECORDS )
#	if !defined( MAKEFOURCC )
#		if BYTE_ORDER == BIG_ENDIAN
#			define MAKEFOURCC(a,b,c,d) ( ( uint32_t( a ) << 24 ) | ( uint32_t( b ) << 16 ) | ( uint32_t( c ) <<  8 ) | ( uint32_t( d ) <<  0 ) )
#		else
#			define MAKEFOURCC(a,b,c,d) ( ( uint32_t( a ) <<  0 ) | ( uint32_t( b ) <<  8 ) | ( uint32_t( c ) << 16 ) | ( uint32_t( d ) << 24 ) )
#		endif
#	endif

		class RecorderImplBase
			: public Recorder::IRecorderImpl
		{
		public:
			RecorderImplBase()
				: m_uiRecordedCount( 0 )
				, m_ui64RecordedTime( 0 )
			{
			}

			virtual ~RecorderImplBase()
			{
			}

			virtual bool UpdateTime()
			{
				uint64_t l_uiTimeDiff = ( wxGetLocalTimeMillis() - m_msSaved ).GetValue();

				if ( m_uiRecordedCount )
				{
					m_ui64RecordedTime += l_uiTimeDiff;
				}

				return DoUpdateTime( l_uiTimeDiff );
			}

			virtual bool StartRecord( Size const & p_size )
			{
				bool l_result = false;

				m_uiRecordedCount = 0;
				m_ui64RecordedTime = 0;
				m_bufferNV12.resize( p_size.x() * ( p_size.y() * 3 ) / 2 );
				wxString l_strWildcard = _( "AVI Video files" );
				l_strWildcard += wxT( "(*.avi)|*.avi" );
				wxFileDialog l_dialog( nullptr, _( "Please choose a video file name" ), wxEmptyString, wxEmptyString, l_strWildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

				if ( l_dialog.ShowModal() == wxID_OK )
				{
					wxString l_strFileName = l_dialog.GetPath();

					try
					{
						DoStartRecord( p_size, l_strFileName );
						l_result = IsRecording();
					}
					catch ( std::bad_alloc & )
					{
					}
					catch ( std::exception & exc )
					{
						wxString l_strMsg = _( "Can't start recording file" );
						l_strMsg += wxT( " " );
						l_strMsg += l_strFileName;
						l_strMsg += wxT( ":\n" );
						l_strMsg += wxString( exc.what(), wxMBConvLibc() );
						l_strMsg += wxT( ")" );
						throw std::runtime_error( ( char const * )l_strMsg.mb_str( wxConvUTF8 ) );
					}
				}

				return l_result;
			}

			virtual bool RecordFrame( PixelBuffer const & p_buffer, Size const & p_size )
			{
				bool l_result = false;
				DoRecordFrame( p_buffer, p_size );
				m_msSaved = wxGetLocalTimeMillis();
				m_uiRecordedCount++;
				l_result = true;
				return l_result;
			}

		protected:
			virtual bool DoStartRecord( Size const & p_size, wxString const & p_name ) = 0;
			virtual bool DoUpdateTime( uint64_t p_uiTimeDiff ) = 0;
			virtual void DoRecordFrame( PixelBuffer const & p_buffer, Size const & p_size ) = 0;

		protected:
			wxMilliClock_t m_msSaved;
			uint32_t m_uiRecordedCount;
			uint64_t m_ui64RecordedTime;
			std::vector< uint8_t > m_bufferNV12;
			const int m_iWantedFPS = 25;
		};

#	if defined( PGEN_FFMPEG )

		struct FFmpegFileWriter
		{
			FFmpegFileWriter()
				: m_pAvCodecContext( NULL )
				, m_pFile( NULL )
			{
			}

			bool IsValid()
			{
				return m_pAvCodecContext && m_pFile;
			}

			void Write( AVPacket & p_packet )
			{
				fwrite( p_packet.data, 1, p_packet.size, m_pFile );
				av_free_packet( &p_packet );
			}

			AVCodecContext * AllocateContext( AVCodec * p_codec, AVCodecID p_id )
			{
				m_pAvCodecContext = avcodec_alloc_context3( p_codec );
				return m_pAvCodecContext;
			}

			void DeallocateContext()
			{
				if ( m_pAvCodecContext )
				{
					avcodec_close( m_pAvCodecContext );
					av_free( m_pAvCodecContext );
					m_pAvCodecContext = NULL;
				}
			}

			void Finish( bool p_wasRecording )
			{
				if ( p_wasRecording )
				{
					uint8_t l_endcode[] = { 0, 0, 1, 0xb7 };
					fwrite( l_endcode, 1, sizeof( l_endcode ), m_pFile );
				}
			}

			void PreOpen( wxString const & p_name )
			{
			}

			bool Open( wxString const & p_name )
			{
				m_pFile = fopen( p_name.char_str().data(), "wb" );
				return m_pFile != NULL;
			}

			void Close()
			{
				if ( m_pFile )
				{
					fclose( m_pFile );
					m_pFile = NULL;
				}
			}

			AVCodecContext * m_pAvCodecContext;
			FILE * m_pFile;
		};

		struct FFmpegStreamWriter
		{
			FFmpegStreamWriter()
				: m_pAvFormatContext( NULL )
				, m_pAvStream( NULL )
			{
			}

			bool IsValid()
			{
				return m_pAvFormatContext && m_pAvStream;
			}

			void Write( AVPacket & p_packet )
			{
				p_packet.stream_index = m_pAvStream->index;
				av_interleaved_write_frame( m_pAvFormatContext, &p_packet );
			}

			AVCodecContext * AllocateContext( AVCodec * p_codec, AVCodecID p_id )
			{
				AVCodecContext * pAvCodecContext = NULL;
				m_pAvStream = avformat_new_stream( m_pAvFormatContext, p_codec );

				if ( m_pAvStream )
				{
					m_pAvStream->id = m_pAvFormatContext->nb_streams - 1;
					pAvCodecContext = m_pAvStream->codec;
					pAvCodecContext->codec_id = p_id;
					pAvCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
					pAvCodecContext->coder_type = FF_CODER_TYPE_VLC;
				}

				return pAvCodecContext;
			}

			void DeallocateContext()
			{
				if ( m_pAvStream )
				{
					avcodec_close( m_pAvStream->codec );
				}
			}

			void Finish( bool p_wasRecording )
			{
				if ( p_wasRecording )
				{
					av_write_trailer( m_pAvFormatContext );
				}
			}

			void PreOpen( wxString const & p_name )
			{
				avformat_alloc_output_context2( &m_pAvFormatContext, NULL, NULL, p_name.char_str().data() );

				if ( !m_pAvFormatContext )
				{
					//printf("Could not deduce output format from file extension: using MPEG.\n");
					avformat_alloc_output_context2( &m_pAvFormatContext, NULL, "mpeg", p_name.char_str().data() );
				}
			}

			bool Open( wxString const & p_name )
			{
				return avio_open( &m_pAvFormatContext->pb, p_name.char_str().data(), AVIO_FLAG_WRITE ) >= 0;
			}

			void Close()
			{
				if ( m_pAvFormatContext )
				{
					if ( m_pAvFormatContext->pb )
					{
						avio_close( m_pAvFormatContext->pb );
					}

					avformat_free_context( m_pAvFormatContext );
					m_pAvFormatContext = NULL;
				}
			}

			AVFormatContext * m_pAvFormatContext;
			AVStream * m_pAvStream;
		};

		template< typename Writer >
		class FFmpegRecorderImpl
			: public RecorderImplBase
			, private Writer
		{
		public:
			FFmpegRecorderImpl()
				: m_pAvCodec( NULL )
				, m_pAvFrame( NULL )
				, m_pAvCodecContext( NULL )
			{
				m_avEncodedPicture.data[0] = NULL;
				av_register_all();
			}

			virtual ~FFmpegRecorderImpl()
			{
			}

			virtual bool IsRecording()
			{
				return Writer::IsValid() && m_pAvCodec && m_pAvFrame;
			}

			virtual void StopRecord()
			{
				if ( IsRecording() )
				{
					AVPacket l_pkt = { 0 };
					av_init_packet( &l_pkt );
					int l_iGotOutput = 1;

					while ( l_iGotOutput )
					{
						int iRet = avcodec_encode_video2( m_pAvCodecContext, &l_pkt, NULL, &l_iGotOutput );

						if ( iRet >= 0 && l_iGotOutput )
						{
							Writer::Write( l_pkt );
						}
					}

					Writer::Finish( true );
				}
				else
				{
					Writer::Finish( false );
				}

				Writer::DeallocateContext();

				if ( m_avEncodedPicture.data[0] )
				{
					av_freep( &m_avEncodedPicture.data[0] );
					m_avEncodedPicture.data[0] = NULL;
				}

				if ( m_pAvFrame )
				{
					av_frame_free( &m_pAvFrame );
					m_pAvFrame = NULL;
				}

				Writer::Close();
			}

		private:
			virtual bool DoUpdateTime( uint64_t p_uiTimeDiff )
			{
				return p_uiTimeDiff >= 1000 / m_iWantedFPS;
			}

			virtual bool DoStartRecord( Size const & p_size, wxString const & p_name )
			{
				wxSize l_size( p_size.x(), p_size.y() );
				Writer::PreOpen( p_name );
				m_pAvCodec = avcodec_find_encoder( m_iCodecID );

				if ( !m_pAvCodec )
				{
					throw std::runtime_error( ( char const * )wxString( _( "Could not find H264 codec" ) ).mb_str( wxConvUTF8 ) );
				}

				m_pAvCodecContext = Writer::AllocateContext( m_pAvCodec, m_iCodecID );

				if ( !m_pAvCodecContext )
				{
					StopRecord();
					throw std::runtime_error( ( char const * )wxString( _( "Could not allocate video codec context" ) ).mb_str( wxConvUTF8 ) );
				}

				// Sample parameters
				m_pAvCodecContext->bit_rate = m_iBitRate;
				// Resolution
				m_pAvCodecContext->width = l_size.x;
				m_pAvCodecContext->height = l_size.y;
				// Frames per second
				m_pAvCodecContext->time_base = ( AVRational )
				{
					1, m_iWantedFPS
				};
				// Emit one intra frame every thirty frames
				m_pAvCodecContext->gop_size = 10;
				m_pAvCodecContext->max_b_frames = 1;
				m_pAvCodecContext->pix_fmt = m_iPixFmt;
				// x264 specifics
				av_opt_set( m_pAvCodecContext->priv_data, "preset", "slow", AV_OPT_SEARCH_CHILDREN );
				av_opt_set( m_pAvCodecContext->priv_data, "profile", "high", AV_OPT_SEARCH_CHILDREN );
				av_opt_set( m_pAvCodecContext->priv_data, "level", "4,1", AV_OPT_SEARCH_CHILDREN );

				if ( avcodec_open2( m_pAvCodecContext, m_pAvCodec, NULL ) < 0 )
				{
					StopRecord();
					throw std::runtime_error( ( char const * )wxString( _( "Could not open codec" ) ).mb_str( wxConvUTF8 ) );
				}

				m_pAvFrame = av_frame_alloc();

				if ( !m_pAvFrame )
				{
					StopRecord();
					throw std::runtime_error( ( char const * )wxString( _( "Could not allocate video frame" ) ).mb_str( wxConvUTF8 ) );
				}

				/* Allocate the encoded raw picture. */
				if ( avpicture_alloc( &m_avEncodedPicture, m_pAvCodecContext->pix_fmt, m_pAvCodecContext->width, m_pAvCodecContext->height ) < 0 )
				{
					StopRecord();
					throw std::runtime_error( ( char const * )wxString( _( "Could not allocate encoded picture buffer" ) ).mb_str( wxConvUTF8 ) );
				}

				*( ( AVPicture * )m_pAvFrame ) = m_avEncodedPicture;

				if ( !Writer::Open( p_name ) )
				{
					StopRecord();
					throw std::runtime_error( ( char const * )wxString( _( "Could not open file" ) ).mb_str( wxConvUTF8 ) );
				}
			}

			virtual void DoRecordFrame( PixelBuffer const & p_buffer, Size const & p_size )
			{
				static SwsContext * l_pSwsContext = NULL;

				if ( !l_pSwsContext )
				{
					l_pSwsContext = sws_getContext( m_pAvCodecContext->width, m_pAvCodecContext->height, AV_PIX_FMT_RGBA,
													m_pAvCodecContext->width, m_pAvCodecContext->height, m_pAvCodecContext->pix_fmt,
													SWS_BICUBIC, NULL, NULL, NULL );

					if ( !l_pSwsContext )
					{
						throw std::runtime_error( ( char const * )wxString( _( "Could not initialise conversion context" ) ).mb_str( wxConvUTF8 ) );
					}
				}

				if ( IsRecording() )
				{
					uint8_t const * l_pBuffer = ( uint8_t const * )p_buffer.ConstPtr();
					int l_lineSize[8] = { m_pAvCodecContext->width * 4, 0, 0, 0, 0, 0, 0, 0 };
					sws_scale( l_pSwsContext, &l_pBuffer, l_lineSize, 0, m_pAvCodecContext->height, m_pAvFrame->data, m_pAvFrame->linesize );
					AVPacket l_pkt = { 0 };
					av_init_packet( &l_pkt );
					m_pAvFrame->pts = m_uiRecordedCount++;
					int l_iGotOutput = 0;

					if ( avcodec_encode_video2( m_pAvCodecContext, &l_pkt, m_pAvFrame, &l_iGotOutput ) >= 0 )
					{
						if ( l_iGotOutput )
						{
							Writer::Write( l_pkt );
						}
					}
					else
					{
						throw std::runtime_error( ( char const * )wxString( _( "Could not encode frame" ) ).mb_str( wxConvUTF8 ) );
					}
				}
			}

		private:
			const AVCodecID m_iCodecID = AV_CODEC_ID_H264;
			const int m_iBitRate = 600000;
			const AVPixelFormat m_iPixFmt = PIX_FMT_YUV420P;
			AVCodec * m_pAvCodec;
			AVFrame * m_pAvFrame;
			AVPicture m_avEncodedPicture;
			AVCodecContext * m_pAvCodecContext;
		};

#		if PGEN_FFMPEG_USE_STREAMS
		typedef FFmpegRecorderImpl< FFmpegStreamWriter > RecorderImpl;
#		else
		typedef FFmpegRecorderImpl< FFmpegFileWriter > RecorderImpl;
#		endif
#	elif defined( PGEN_OCV )

		class RecorderImpl
			: public RecorderImplBase
		{
		public:
			RecorderImpl()
			{
			}

			virtual ~RecorderImpl()
			{
			}

			virtual bool IsRecording()
			{
				return m_writer.isOpened();
			}

			virtual void StopRecord()
			{
				m_writer = cv::VideoWriter();
			}

		private:
			virtual bool DoUpdateTime( uint64_t p_uiTimeDiff )
			{
				return true;
			}

			virtual bool DoStartRecord( Size const & p_size, wxString const & p_name )
			{
				if ( !m_writer.open( p_name.char_str().data(), MAKEFOURCC( 'X', '2', '6', '4' ), m_iWantedFPS, cv::Size( p_size.x(), p_size.y() ), true ) )
				{
#		if defined( _WIN32 )
					wxMessageBox( _( "Can't open file with OpenCV, encoding: H264, please select another codec" ) );

					if ( !m_writer.open( p_name.char_str().data(), -1, m_iWantedFPS, cv::Size( p_size.x(), p_size.y() ), true ) )
					{
						throw std::runtime_error( ( char const * )wxString( _( "Could not open file with OpenCV" ) ).mb_str( wxConvUTF8 ) );
					}

#		else
					throw std::runtime_error( ( char const * )wxString( _( "Could not open file with OpenCV, encoding: H264" ) ).mb_str( wxConvUTF8 ) );
#		endif
				}

				return true;
			}

			virtual void DoRecordFrame( PixelBuffer const & p_buffer, Size const & p_size )
			{
				PixelBuffer l_output( p_buffer );
				cv::Mat l_img( cv::Size( p_size.x(), p_size.y() ), CV_8UC4, l_output.Ptr() );
				cv::flip( l_img, l_img, 0 );
				cv::cvtColor( l_img, l_img, cv::COLOR_BGRA2RGBA );

				try
				{
					m_writer << l_img;
				}
				catch ( std::exception & )
				{
					throw;
				}
				catch ( char * p_exc )
				{
					throw std::runtime_error( p_exc );
				}
				catch ( wchar_t * p_exc )
				{
					throw std::runtime_error( ( char const * )wxString( p_exc, wxMBConvLibc() ).mb_str( wxConvUTF8 ) );
				}
				catch ( ... )
				{
					throw std::runtime_error( ( char const * )wxString( _( "Unknown error" ) ).mb_str( wxConvUTF8 ) );
				}
			}

		private:
			cv::VideoWriter m_writer;
		};

#	else

		class RecorderImpl
			: public RecorderImplBase
		{
		public:
			RecorderImpl()
			{
			}

			virtual ~RecorderImpl()
			{
			}

			virtual bool IsRecording()
			{
				return false;
			}

			virtual void StopRecord()
			{
			}

		private:
			virtual bool DoUpdateTime( uint64_t p_uiTimeDiff )
			{
				return true;
			}

			virtual bool DoStartRecord( Size const & p_size, wxString const & p_name )
			{
				return true;
			}

			virtual void DoRecordFrame( PixelBuffer const &, Size const & )
			{
			}
		};

#	endif
#else

		class RecorderImpl
			: public Recorder::IRecorderImpl
		{
		public:
			RecorderImpl()
			{
			}

			virtual ~RecorderImpl()
			{
			}

			virtual bool StartRecord( Size const & p_size )
			{
				return true;
			}

			virtual bool IsRecording()
			{
				return false;
			}

			virtual bool UpdateTime()
			{
				return false;
			}

			virtual bool RecordFrame( PixelBuffer const &, Size const & )
			{
				return true;
			}

			virtual void StopRecord()
			{
			}
		};

#endif
	}

	//*************************************************************************************************

	Recorder::Recorder()
		: m_impl( std::make_unique< RecorderImpl >() )
	{
	}

	Recorder::~Recorder()
	{
		m_impl.reset();
	}
}
