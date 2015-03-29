#include "MainFrame.h"

#include <Generator.h>

#include <emmintrin.h>

#include "RenderPanel.h"

#pragma warning( push )
#pragma warning( disable:4996 )
#include <wx/dcclient.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#pragma warning( pop )

using namespace ProceduralTextures;

namespace ProceduralGenerator
{
#if !defined( MAKEFOURCC )
#	if BYTE_ORDER == BIG_ENDIAN
#		define MAKEFOURCC(a,b,c,d) ( ( uint32_t( a ) << 24 ) | ( uint32_t( b ) << 16 ) | ( uint32_t( c ) <<  8 ) | ( uint32_t( d ) <<  0 ) )
#	else
#		define MAKEFOURCC(a,b,c,d) ( ( uint32_t( a ) <<  0 ) | ( uint32_t( b ) <<  8 ) | ( uint32_t( c ) << 16 ) | ( uint32_t( d ) << 24 ) )
#	endif
#endif
	namespace
	{
		eKEYBOARD_KEY ConvertKeyCode( int p_code )
		{
			eKEYBOARD_KEY l_return = eKEYBOARD_KEY_NONE;

			if ( p_code < 0x20 )
			{
				switch ( p_code )
				{
				case WXK_BACK:
				case WXK_TAB:
				case WXK_RETURN:
				case WXK_ESCAPE:
					l_return = eKEYBOARD_KEY( p_code );
					break;
				}
			}
			else if ( p_code == 0x7F )
			{
				l_return = eKEY_DELETE;
			}
			else if ( p_code > 0xFF )
			{
				l_return = eKEYBOARD_KEY( p_code + eKEY_START - WXK_START );
			}
			else
			{
				// ASCII or extended ASCII character
				l_return = eKEYBOARD_KEY( p_code );
			}

			return l_return;
		}
	}

#if defined( PGEN_FFMPEG )
	static const AVCodecID		g_iCodecID		= CODEC_ID_H264;
	static const int 			g_iBitRate		= 600000;
	static const AVPixelFormat	g_iPixFmt		= PIX_FMT_YUV420P;
#endif
	static const int 			g_iWantedFPS	= 25;

	//*************************************************************************************************

	RenderPanel::RenderPanel( MainFrame * p_parent, int * p_attribList, wxPoint const & p_position, wxSize const & p_size )
		: wxGLCanvas( p_parent, wxID_ANY, p_attribList, p_position, p_size )
		, m_pContext( NULL )
		, m_bInitialised( false )
		, m_pGenerator( NULL )
		, m_bFullScreen( false )
		, m_pFrame( p_parent )
		, m_mouseMoveIndex( 0 )
#if defined( PGEN_FFMPEG )
		, m_pAvCodec( NULL )
		, m_pAvFrame( NULL )
#	if USE_STREAMS
		, m_pAvFormatContext( NULL )
		, m_pAvStream( NULL )
#	else
		, m_pAvCodecContext( NULL )
		, m_pFile( NULL )
#	endif
#endif
#if defined( PGEN_RECORDS )
		, m_uiRecordedCount( 0 )
		, m_ui64RecordedTime( 0 )
#endif
		, m_timer( NULL )
		, m_count( 0 )
	{
#if defined( PGEN_FFMPEG )
		m_avEncodedPicture.data[0] = NULL;
		av_register_all();
#endif
		m_timer = new wxTimer( this, eID_TIMER );
		SetBackgroundColour( p_parent->GetBackgroundColour() );
		SetForegroundColour( p_parent->GetForegroundColour() );
		Show();
	}

	RenderPanel::~RenderPanel()
	{
		delete m_timer;
	}

	void RenderPanel::SetGenerator( std::shared_ptr< GeneratorBase > p_pGenerator )
	{
		m_timer->Stop();
		m_bFullScreen = false;
		DoStopGenerator();
		m_pGenerator = p_pGenerator;

		if ( m_pGenerator )
		{
			SetSize( wxSize( p_pGenerator->GetDisplaySize().x(), p_pGenerator->GetDisplaySize().y() ) );
			m_pGenerator->Run();
			m_timer->Start( int( m_pGenerator->GetFrameTime().count() ) );
		}

		m_bInitialised = false;
	}

	std::shared_ptr< GeneratorBase > RenderPanel::GetGenerator()const
	{
		return m_pGenerator;
	}

	void RenderPanel::SaveFrame()
	{
		if ( m_pGenerator )
		{
			m_pGenerator->SaveFrame();
			DoRender();
			PixelBuffer const & l_buffer = m_pGenerator->GetSavedFrame();
			Size l_size = m_pGenerator->GetImageSize();
			wxImage l_image( l_size.x(), l_size.y(), true );
			int x = 0;

			for ( uint32_t i = 0; i < l_size.x(); i++ )
			{
				int y = l_size.y() - 1;

				for ( uint32_t j = 0; j < l_size.y(); j++ )
				{
					UbPixel const & l_pixel = l_buffer[j][i];
					l_image.SetRGB( x, y, l_pixel.r, l_pixel.g, l_pixel.b );
					y--;
				}

				x++;
			}

			wxString l_strWildcard = _( "All supported files" );
			l_strWildcard += wxT( " (*.bmp;*.gif;*.png;*.jpg)|*.bmp;*.gif;*.png;*.jpg|" );
			l_strWildcard += _( "BITMAP files" );
			l_strWildcard += wxT( " (*.bmp)|*.bmp|" );
			l_strWildcard += _( "GIF files" );
			l_strWildcard += wxT( " (*.gif)|*.gif|" );
			l_strWildcard += _( "JPEG files" );
			l_strWildcard += wxT( " (*.jpg)|*.jpg|" );
			l_strWildcard += _( "PNG files" );
			l_strWildcard += wxT( " (*.png)|*.png" );
			wxFileDialog l_dialog( this, _( "Please choose an image file name" ), wxEmptyString, wxEmptyString, l_strWildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				l_image.SaveFile( l_dialog.GetPath() );
			}
		}
	}

	void RenderPanel::Render()
	{
		if ( IsShown() && GetParent()->IsShown() )
		{
			wxClientDC l_dc( this );

			if ( m_pGenerator )
			{
				if ( !m_pContext )
				{
					m_pContext = new wxGLContext( this, NULL );
				}

#if defined( PGEN_RECORDS )
				uint64_t l_uiTimeDiff = ( wxGetLocalTimeMillis() - m_msSaved ).GetValue();

				if ( m_uiRecordedCount != 0 )
				{
					m_ui64RecordedTime += l_uiTimeDiff;
				}

#	if defined( PGEN_FFMPEG )

				if ( IsRecording() && l_uiTimeDiff >= 1000 / g_iWantedFPS )
#	else
				if ( IsRecording() )
#	endif
				{
					m_pGenerator->SaveFrame();
					DoRender();
					PixelBuffer const & l_buffer = m_pGenerator->GetSavedFrame();
					Size l_size = m_pGenerator->GetImageSize();
#	if defined( PGEN_FFMPEG )
					static SwsContext * l_pSwsContext = NULL;
#		if USE_STREAMS
					AVCodecContext * pAvCodecContext = m_pAvStream->codec;
#		else
					AVCodecContext * pAvCodecContext = m_pAvCodecContext;
#		endif

					if ( !l_pSwsContext )
					{
						l_pSwsContext = sws_getContext( pAvCodecContext->width, pAvCodecContext->height, AV_PIX_FMT_RGBA,
														pAvCodecContext->width, pAvCodecContext->height, pAvCodecContext->pix_fmt,
														SWS_BICUBIC, NULL, NULL, NULL );

						if ( !l_pSwsContext )
						{
							m_pFrame->StopRecord();
							wxString l_strError = _( "Can't initialise conversion context" );
							wxMessageBox( l_strError );
						}
					}

					if ( IsRecording() )
					{
						uint8_t const * l_pBuffer = ( uint8_t const * )l_buffer.ConstPtr();
						int l_lineSize[8] = { pAvCodecContext->width * 4, 0, 0, 0, 0, 0, 0, 0 };
						sws_scale( l_pSwsContext, &l_pBuffer, l_lineSize, 0, pAvCodecContext->height, m_pAvFrame->data, m_pAvFrame->linesize );
						AVPacket l_pkt = { 0 };
						av_init_packet( &l_pkt );
						m_pAvFrame->pts = m_uiRecordedCount++;
						int l_iGotOutput = 0;

						if ( avcodec_encode_video2( pAvCodecContext, &l_pkt, m_pAvFrame, &l_iGotOutput ) >= 0 )
						{
							if ( l_iGotOutput )
							{
#		if USE_STREAMS
								l_pkt.stream_index = m_pAvStream->index;
								av_interleaved_write_frame( m_pAvFormatContext, &l_pkt );
#		else
								fwrite( l_pkt.data, 1, l_pkt.size, m_pFile );
								av_free_packet( &l_pkt );
#		endif
							}
						}
						else
						{
							wxString l_strError = _( "Can't encode frame" );
							wxMessageBox( l_strError );
						}
					}

#	elif defined( PGEN_OCV )
					PixelBuffer l_output( l_buffer );
					cv::Mat l_img( cv::Size( l_size.x(), l_size.y() ), CV_8UC4, l_output.Ptr() );
					cv::flip( l_img, l_img, 0 );
					cv::cvtColor( l_img, l_img, cv::COLOR_BGRA2RGBA );

					try
					{
						m_writer << l_img;
					}
					catch ( std::exception & exc )
					{
						m_pFrame->StopRecord();
						wxString l_strError = _( "Can't write image :\n" );
						l_strError << wxString( exc.what(), wxMBConvLibc() );
						wxMessageBox( l_strError );
					}
					catch ( char * szExc )
					{
						m_pFrame->StopRecord();
						wxString l_strError = _( "Can't write image :\n" );
						l_strError << wxString( szExc, wxMBConvLibc() );
						wxMessageBox( l_strError );
					}
					catch ( wchar_t * wszExc )
					{
						m_pFrame->StopRecord();
						wxString l_strError = _( "Can't write image :\n" );
						l_strError << wxString( wszExc, wxMBConvLibc() );
						wxMessageBox( l_strError );
					}
					catch ( ... )
					{
						m_pFrame->StopRecord();
					}

					m_uiRecordedCount++;
#	endif
					m_msSaved = wxGetLocalTimeMillis();
				}
				else
#endif
				{
					DoRender();
				}
			}
			else
			{
				l_dc.SetBrush( *wxBLACK_BRUSH );
				l_dc.DrawRectangle( wxPoint( 0, 0 ), GetClientSize() );
			}
		}
	}
#if defined( PGEN_RECORDS )
	bool RenderPanel::StartRecord()
	{
		bool l_bReturn = false;

		if ( m_pGenerator && !IsRecording() )
		{
			m_uiRecordedCount = 0;
			m_ui64RecordedTime = 0;
			m_bufferNV12.resize( m_pGenerator->GetImageSize().x() * ( m_pGenerator->GetImageSize().y() * 3 ) / 2 );
			wxString l_strWildcard = _( "AVI Video files" );
			l_strWildcard += wxT( "(*.avi)|*.avi" );
			wxFileDialog l_dialog( this, _( "Please choose a video file name" ), wxEmptyString, wxEmptyString, l_strWildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				wxString l_strFileName = l_dialog.GetPath();

				try
				{
#	if defined( PGEN_FFMPEG )
					wxSize l_size( m_pGenerator->GetImageSize().x(), m_pGenerator->GetImageSize().y() );
#		if USE_STREAMS
					avformat_alloc_output_context2( &m_pAvFormatContext, NULL, NULL, l_strFileName.char_str().data() );

					if ( !m_pAvFormatContext )
					{
//						printf("Could not deduce output format from file extension: using MPEG.\n");
						avformat_alloc_output_context2( &m_pAvFormatContext, NULL, "mpeg", l_strFileName.char_str().data() );
					}

#		endif
					m_pAvCodec = avcodec_find_encoder( g_iCodecID );

					if ( !m_pAvCodec )
					{
						wxString l_msg = _( "Can't find H264 codec" );
						throw std::runtime_error( ( char const * )l_msg.mb_str( wxConvUTF8 ) );
					}

#		if USE_STREAMS
					m_pAvStream = avformat_new_stream( m_pAvFormatContext, m_pAvCodec );

					if ( !m_pAvStream )
					{
						StopRecord();
						wxString l_msg = _( "Could not allocate stream" );
						throw std::runtime_error( ( char const * )l_msg.mb_str( wxConvUTF8 ) );
					}

					m_pAvStream->id = m_pAvFormatContext->nb_streams - 1;
					AVCodecContext * pAvCodecContext = m_pAvStream->codec;
					pAvCodecContext->codec_id = g_iCodecID;
					pAvCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
					pAvCodecContext->coder_type = FF_CODER_TYPE_VLC;
#		else
					m_pAvCodecContext = avcodec_alloc_context3( m_pAvCodec );

					if ( !m_pAvCodecContext )
					{
						StopRecord();
						wxString l_msg = _( "Could not allocate video codec context" );
						throw std::runtime_error( ( char const * )l_msg.mb_str( wxConvUTF8 ) );
					}

					AVCodecContext * pAvCodecContext = m_pAvCodecContext;
#		endif
					// Sample parameters
					pAvCodecContext->bit_rate = g_iBitRate;
					// Resolution
					pAvCodecContext->width = l_size.x;
					pAvCodecContext->height = l_size.y;
					// Frames per second
					pAvCodecContext->time_base = ( AVRational )
					{
						1, g_iWantedFPS
					};
					// Emit one intra frame every thirty frames
					pAvCodecContext->gop_size = 10;
					pAvCodecContext->max_b_frames = 1;
					pAvCodecContext->pix_fmt = g_iPixFmt;
					// x264 specifics
					av_opt_set( pAvCodecContext->priv_data, "preset",			"slow",	AV_OPT_SEARCH_CHILDREN );
					av_opt_set( pAvCodecContext->priv_data, "profile",			"high",	AV_OPT_SEARCH_CHILDREN );
					av_opt_set( pAvCodecContext->priv_data, "level",			"4,1",	AV_OPT_SEARCH_CHILDREN );

					if ( avcodec_open2( pAvCodecContext, m_pAvCodec, NULL ) < 0 )
					{
						StopRecord();
						wxString l_msg = _( "Could not open codec" );
						throw std::runtime_error( ( char const * )l_msg.mb_str( wxConvUTF8 ) );
					}

					m_pAvFrame = av_frame_alloc();

					if ( !m_pAvFrame )
					{
						StopRecord();
						wxString l_msg = _( "Could not allocate video frame" );
						throw std::runtime_error( ( char const * )l_msg.mb_str( wxConvUTF8 ) );
					}

					/* Allocate the encoded raw picture. */
					if ( avpicture_alloc( &m_avEncodedPicture, pAvCodecContext->pix_fmt, pAvCodecContext->width, pAvCodecContext->height ) < 0 )
					{
						StopRecord();
						wxString l_msg = _( "Could not allocate encoded picture buffer" );
						throw std::runtime_error( ( char const * )l_msg.mb_str( wxConvUTF8 ) );
					}

					*( ( AVPicture * )m_pAvFrame ) = m_avEncodedPicture;
#		if USE_STREAMS

					if ( avio_open( &m_pAvFormatContext->pb, l_strFileName.char_str().data(), AVIO_FLAG_WRITE ) < 0 )
					{
						StopRecord();
						wxString l_msg = _( "Could not open file" );
						throw std::runtime_error( ( char const * )l_msg.mb_str( wxConvUTF8 ) );
					}

#		else
					m_pFile = fopen( l_strFileName.char_str().data(), "wb" );

					if ( !m_pFile )
					{
						StopRecord();
						wxString l_msg = _( "Could not open file" );
						throw std::runtime_error( ( char const * )l_msg.mb_str( wxConvUTF8 ) );
					}

#		endif
#	elif defined( PGEN_OCV )
					Size l_size = m_pGenerator->GetImageSize();

					if ( !m_writer.open( l_strFileName.char_str().data(), MAKEFOURCC( 'X', '2', '6', '4' ), g_iWantedFPS, cv::Size( l_size.x(), l_size.y() ), true ) )
					{
#		if defined( _WIN32 )
						wxMessageBox( _( "Can't open file with OpenCV, encoding: H264, pleas select another codec" ) );

						if ( !m_writer.open( l_strFileName.char_str().data(), -1, g_iWantedFPS, cv::Size( l_size.x(), l_size.y() ), true ) )
						{
						}

#		else
						wxMessageBox( _( "Can't open file with OpenCV, encoding: H264" ) );
#		endif
					}

#	endif
					l_bReturn = IsRecording();
				}
				catch ( std::bad_alloc & )
				{
				}
				catch ( std::exception & exc )
				{
					wxString l_strMsg = _( "Can't open file " );
					l_strMsg += l_strFileName;
					l_strMsg += wxT( " (" );
					l_strMsg += wxString( exc.what(), wxMBConvLibc() );
					l_strMsg += wxT( ")" );
					wxMessageBox( l_strMsg );
				}
				catch ( wxChar const * pszExc )
				{
					wxString l_strMsg = _( "Can't open file " );
					l_strMsg += l_strFileName;
					l_strMsg += wxT( " (" );
					l_strMsg += wxString( pszExc );
					l_strMsg += wxT( ")" );
					wxMessageBox( l_strMsg );
				}
				catch ( char const * pszExc )
				{
					wxString l_strMsg = _( "Can't open file " );
					l_strMsg += l_strFileName;
					l_strMsg += wxT( " (" );
					l_strMsg += wxString( pszExc, wxMBConvLibc() );
					l_strMsg += wxT( ")" );
					wxMessageBox( l_strMsg );
				}
				catch ( ... )
				{
					wxString l_strMsg = _( "Can't open file " );
					l_strMsg += l_strFileName;
					l_strMsg += wxT( " (" );
					l_strMsg += _( "Unknown error" );
					l_strMsg += wxT( ")" );
					wxMessageBox( l_strMsg );
				}
			}
		}

		return l_bReturn;
	}

	bool RenderPanel::IsRecording()
	{
#	if defined( PGEN_FFMPEG )
#		if USE_STREAMS
		return m_pAvFormatContext && m_pAvCodec && m_pAvFrame && m_pAvStream;
#		else
		return m_pAvCodecContext && m_pAvCodec && m_pAvFrame && m_pFile;
#		endif
#	elif defined( PGEN_OCV )
		return m_writer.isOpened();
#	endif
	}

	void RenderPanel::StopRecord()
	{
#	if defined( PGEN_FFMPEG )

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
#		if USE_STREAMS
					l_pkt.stream_index = m_pAvStream->index;
					av_interleaved_write_frame( m_pAvFormatContext, &l_pkt );
#		else
					fwrite( l_pkt.data, 1, l_pkt.size, m_pFile );
					av_free_packet( &l_pkt );
#		endif
				}
			}
		}

#		if USE_STREAMS

		if ( IsRecording() )
		{
			av_write_trailer( m_pAvFormatContext );
		}

		if ( m_pAvStream )
		{
			avcodec_close( m_pAvStream->codec );
		}

#		else

		if ( IsRecording() )
		{
			uint8_t l_endcode[] = { 0, 0, 1, 0xb7 };
			fwrite( l_endcode, 1, sizeof( l_endcode ), m_pFile );
			fclose( m_pFile );
		}

		if ( m_pAvCodecContext )
		{
			avcodec_close( m_pAvCodecContext );
			av_free( m_pAvCodecContext );
			m_pAvCodecContext = NULL;
		}

#		endif

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

#		if USE_STREAMS

		if ( m_pAvFormatContext )
		{
			if ( m_pAvFormatContext->pb )
			{
				avio_close( m_pAvFormatContext->pb );
			}

			avformat_free_context( m_pAvFormatContext );
			m_pAvFormatContext = NULL;
		}

#		endif
#	elif defined( PGEN_OCV )
		m_writer = cv::VideoWriter();
#	endif
	}
#endif

	void RenderPanel::DoStopGenerator()
	{
		if ( m_pGenerator )
		{
			if ( m_pContext )
			{
				wxClientDC l_dc( this );
				m_pContext->SetCurrent( *this );
			}

			m_pGenerator->Stop();
			m_pGenerator->Cleanup();
			m_pGenerator->Destroy();
		}
	}

	void RenderPanel::DoRender()
	{
		m_pContext->SetCurrent( *this );

		if ( !m_pGenerator->IsInitialised() )
		{
			m_pGenerator->Initialise();
		}

		m_pGenerator->Render( [this]()
		{
			SwapBuffers();
		} );
	}

	BEGIN_EVENT_TABLE( RenderPanel, wxGLCanvas )
		EVT_PAINT( RenderPanel::OnPaint )
		EVT_SIZE( RenderPanel::OnSize )
		EVT_CLOSE( RenderPanel::OnClose )
		EVT_KEY_DOWN( RenderPanel::OnKeyDown )
		EVT_KEY_UP( RenderPanel::OnKeyUp )
		EVT_MOTION( RenderPanel::OnMouseMove )
		EVT_LEFT_DOWN( RenderPanel::OnLeftDown )
		EVT_LEFT_UP( RenderPanel::OnLeftUp )
		EVT_MIDDLE_DOWN( RenderPanel::OnMiddleDown )
		EVT_MIDDLE_UP( RenderPanel::OnMiddleUp )
		EVT_RIGHT_DOWN( RenderPanel::OnRightDown )
		EVT_RIGHT_UP( RenderPanel::OnRightUp )
		EVT_LEFT_DCLICK( RenderPanel::OnLeftDClick )
		EVT_CHAR( RenderPanel::OnChar )
		EVT_TIMER( eID_TIMER, RenderPanel::OnTimer )
	END_EVENT_TABLE()

	void RenderPanel::OnPaint( wxPaintEvent & p_event )
	{
		wxPaintDC l_dc( this );

		if ( !m_pGenerator )
		{
			l_dc.SetBrush( *wxBLACK_BRUSH );
			l_dc.DrawRectangle( wxPoint( 0, 0 ), GetClientSize() );
		}

		p_event.Skip();
	}

	void RenderPanel::OnSize( wxSizeEvent & p_event )
	{
		if ( m_pGenerator )
		{
			wxSize l_size = p_event.GetSize();
			m_pGenerator->UpdateDisplaySize( Size( l_size.x, l_size.y ) );
		}

#if defined( PGEN_RECORDS )
		StopRecord();
#endif

		if ( !m_pGenerator || !m_pGenerator->IsInitialised() )
		{
			wxClientDC l_dc( this );
			l_dc.SetBrush( *wxBLACK_BRUSH );
			l_dc.DrawRectangle( wxPoint( 0, 0 ), GetClientSize() );
		}

		p_event.Skip();
	}

	void RenderPanel::OnTimer( wxTimerEvent & p_event )
	{
		if ( m_pGenerator )
		{
			Render();
			std::chrono::milliseconds l_time = m_pGenerator->GetCpuTime() + m_pGenerator->GetGpuTime();

			if ( l_time.count() >= m_timer->GetInterval() )
			{
				if ( m_count++ >= 100 )
				{
					m_timer->Stop();
					m_timer->Start( m_timer->GetInterval() * 2 );
				}
			}
			else
			{
				m_count = 0;

				if ( l_time < m_pGenerator->GetFrameTime() )
				{
					m_timer->Stop();
					m_timer->Start( int( m_pGenerator->GetFrameTime().count() ) );
				}
			}
		}
	}

	void RenderPanel::OnClose( wxCloseEvent & p_event )
	{
#if defined( PGEN_RECORDS )
		StopRecord();
#endif
		DoStopGenerator();

		if ( m_pContext )
		{
			delete m_pContext;
			m_pContext = NULL;
		}

		p_event.Skip();
	}

	void RenderPanel::OnKeyDown( wxKeyEvent & p_event )
	{
		if ( p_event.GetKeyCode() == WXK_F12 )
		{
			SaveFrame();
		}
		else if ( m_pGenerator )
		{
			m_pGenerator->PostKeyboardEvent( eKEYBOARD_EVENT_KEY_PUSHED, ConvertKeyCode( p_event.GetKeyCode() ), p_event.ControlDown(), p_event.AltDown(), p_event.ShiftDown() );
		}

		p_event.Skip();
	}

	void RenderPanel::OnKeyUp( wxKeyEvent & p_event )
	{
		if ( p_event.GetKeyCode() == WXK_F12 )
		{
			SaveFrame();
		}
		else if ( m_pGenerator )
		{
			m_pGenerator->PostKeyboardEvent( eKEYBOARD_EVENT_KEY_RELEASED, ConvertKeyCode( p_event.GetKeyCode() ), p_event.ControlDown(), p_event.AltDown(), p_event.ShiftDown() );
		}

		p_event.Skip();
	}

	void RenderPanel::OnChar( wxKeyEvent & p_event )
	{
		if ( m_pGenerator )
		{
			wxChar l_key = p_event.GetUnicodeKey();
			wxString l_tmp;
			l_tmp << l_key;
			m_pGenerator->PostCharEvent( ConvertKeyCode( p_event.GetKeyCode() ), String( l_tmp.mb_str( wxConvUTF8 ) ) );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMouseMove( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_MOVE, Position( p_event.GetX(), p_event.GetY() ) );
		}

		p_event.Skip();
	}

	void RenderPanel::OnLeftDown( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_LEFT );
		}

		p_event.Skip();
	}

	void RenderPanel::OnLeftUp( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_LEFT );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMiddleDown( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_MIDDLE );
		}

		p_event.Skip();
	}

	void RenderPanel::OnMiddleUp( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_MIDDLE );
		}

		p_event.Skip();
	}

	void RenderPanel::OnRightDown( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_RIGHT );
		}

		p_event.Skip();
	}

	void RenderPanel::OnRightUp( wxMouseEvent & p_event )
	{
		if ( m_pGenerator )
		{
			m_pGenerator->PostMouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, Position( p_event.GetX(), p_event.GetY() ), eMOUSE_BUTTON_RIGHT );
		}

		p_event.Skip();
	}

	void RenderPanel::OnLeftDClick( wxMouseEvent & p_event )
	{
		if ( m_pGenerator && m_pGenerator->IsInitialised() )
		{
			m_bFullScreen = !m_bFullScreen;
			m_pFrame->ShowFullScreen( m_bFullScreen, wxFULLSCREEN_ALL );
		}

		p_event.Skip();
	}
}
