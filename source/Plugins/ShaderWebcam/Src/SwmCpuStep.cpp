#include "SwmCpuStep.h"

namespace ShaderWebcam
{
	Thread::Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		:	ProceduralTextures::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight )
	{
	}

	Thread::~Thread()
	{
	}

	void Thread::Initialise( std::shared_ptr< cv::VideoCapture > p_pCapture, std::shared_ptr< ProceduralTextures::PixelBuffer > p_pBuffer )
	{
		m_capture = p_pCapture;
		m_pBuffer = p_pBuffer;
	}

	void Thread::DoStep()
	{
		std::shared_ptr< cv::VideoCapture > l_capture = m_capture.lock();
		std::shared_ptr< ProceduralTextures::PixelBuffer > l_buffer = m_pBuffer.lock();

		if ( l_capture && l_buffer && l_capture->grab() )
		{
			cv::Mat l_frame;
			l_capture->retrieve( l_frame );

			if ( l_frame.cols > 0 )
			{
				cv::cvtColor( l_frame, l_frame, cv::COLOR_BGR2RGB );
				cv::flip( l_frame, l_frame, 0 );
				l_buffer->Set< PG_UINT8_RGB >( ( uint8_t * )l_frame.ptr(), NULL );
			}
		}
	}

	//*************************************************************************************************

	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, std::shared_ptr< cv::VideoCapture > p_capture, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::CpuStep< Thread >( p_generator, p_size )
		, m_capture( p_capture )
	{
	}

	CpuStep::~CpuStep()
	{
	}

	void CpuStep::DoInitialiseStep()
	{
	}

	void CpuStep::DoInitialise()
	{
		std::shared_ptr< cv::VideoCapture > l_capture = m_capture.lock();
		int l_iWidth = int( m_sizeImage.x() );
		int l_iHeight = int( m_sizeImage.y() );

		if ( l_capture )
		{
			DoCreateThread( 0, l_iHeight, l_iHeight );
			DoForEachThread( [&l_capture, this]( Thread & p_thread )
			{
				p_thread.Initialise( l_capture, m_finalBuffer );
				p_thread.Run();
			} );
		}

		m_finalBuffer->Initialise( m_sizeImage );
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoInitialiseCapture()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
	}
}
