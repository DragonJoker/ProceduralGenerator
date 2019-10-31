#include "CpuStep.h"

#include "GeneratorUtils.h"

namespace ProceduralTextures
{
	Thread::Thread( std::shared_ptr< CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		: m_pParent( p_pParent )
		, m_uiIndex( p_uiIndex )
		, m_iWidth( iWidth )
		, m_iBottom( iBottom )
		, m_iTop( iTop )
		, m_iHeight( iTotalHeight )
		, m_threadEndIndex( 0 )
	{
		m_launched = false;
		m_stopped = false;
	}

	Thread::~Thread()
	{
	}

	void Thread::Run()
	{
		m_thread = std::thread( [this]()
		{
			do
			{
				if ( IsLaunched() )
				{
					{
						std::unique_lock< std::mutex > l_lock( m_mutex );
						m_launched = false;
						DoStep();
					}

					m_signalThreadEnd( m_uiIndex );
				}
				else
				{
					std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
				}
			}
			while ( !IsStopped() && m_thread.joinable() );

			m_stoppedCondition.notify_one();
		} );
	}

	void Thread::Stop()
	{
		m_launched = false;
		m_stopped = true;
		std::unique_lock< std::mutex > l_lock( m_mutex );
		m_signalThreadEnd( m_uiIndex );
		m_thread.detach();
		m_stoppedCondition.wait_for( l_lock, std::chrono::milliseconds( 1000 ) );
	}

	//*************************************************************************************************

	CpuStepBase::CpuStepBase( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size )
		: m_generator( p_generator )
		, m_sizeImage( p_size )
		, m_initialised( false )
		, m_threadsCount( std::max( 1, System::GetCPUCount() ) )
		, m_finalBuffer( std::make_shared< ProceduralTextures::PixelBuffer >( p_size ) )
	{
		m_stopped = false;
	}

	CpuStepBase::~CpuStepBase()
	{
		m_finalBuffer->Clear();
		m_finalBuffer.reset();
	}

	void CpuStepBase::Initialise()
	{
		DoInitialise();
		m_initialised = true;
	}

	void CpuStepBase::Cleanup()
	{
		m_initialised = false;
		DoCleanup();
		DoThreadsCleanup();
	}

	void CpuStepBase::Run()
	{
		m_thread = std::thread( [this]()
		{
			do
			{
				if ( m_initialised )
				{
					std::unique_lock< std::mutex > l_lock( m_mutexWake );
					bool l_go = m_conditionWake.wait_for( l_lock, std::chrono::milliseconds( 10 ) ) == std::cv_status::no_timeout;
					l_lock.unlock();

					if ( l_go )
					{
						Render();
					}
				}
			}
			while ( !IsStopped() && m_thread.joinable() );

			m_stoppedCondition.notify_one();
		} );
	}

	void CpuStepBase::Stop()
	{
		std::unique_lock< std::mutex > l_lock( m_mutex );
		m_thread.detach();
		m_stopped = true;
		m_stoppedCondition.wait_for( l_lock, std::chrono::milliseconds( 1000 ) );
	}

	void CpuStepBase::SwapBuffers()
	{
		Clock::time_point l_startTime = Clock::now();
		DoSwapBuffers();
		m_time += std::chrono::duration_cast< std::chrono::milliseconds >( Clock::now() - l_startTime );
	}

	void CpuStepBase::Render()
	{
		Clock::time_point l_startTime = Clock::now();
		m_time = std::chrono::milliseconds( 0 );
		DoStartRender();
		Wait( std::chrono::milliseconds( 2000 ) );
		m_time += std::chrono::duration_cast< std::chrono::milliseconds >( Clock::now() - l_startTime );
	}

	void CpuStepBase::Resize( int p_iWidth, int p_iHeight )
	{
		Resize( Size( p_iWidth, p_iHeight ) );
	}

	void CpuStepBase::Resize( Size const & p_size )
	{
		m_sizeImage = p_size;
	}

	void CpuStepBase::Wake()
	{
		std::unique_lock< std::mutex > l_lock( m_mutexWake );
		m_conditionWake.notify_one();
	}

	void CpuStepBase::Wait( std::chrono::milliseconds p_timeout )
	{
		std::unique_lock< std::mutex > l_lock( m_mutexEnd );
		m_conditionEnd.wait_for( l_lock, p_timeout );
	}

	void CpuStepBase::OnThreadEnd( size_t p_index )
	{
		m_endedThreadsCount++;

		if ( m_endedThreadsCount >= m_threadsCount )
		{
			std::unique_lock< std::mutex > l_lock( m_mutexEnd );
			m_conditionEnd.notify_one();
		}
	}

	void CpuStepBase::DoStartRender()
	{
		m_endedThreadsCount = 0;
		DoInitialiseStep();
		DoThreadsStart();
	}

	//*************************************************************************************************

	DefaultCpuStep::DefaultCpuStep( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size )
		: CpuStepBase( p_generator, p_size )
	{
	}

	DefaultCpuStep::~DefaultCpuStep()
	{
	}

	void DefaultCpuStep::DoInitialiseStep()
	{
	}

	void DefaultCpuStep::DoInitialise()
	{
	}

	void DefaultCpuStep::DoCleanup()
	{
	}

	void DefaultCpuStep::DoSwapBuffers()
	{
	}

	void DefaultCpuStep::DoThreadsStart()
	{
	}

	void DefaultCpuStep::DoThreadsCleanup()
	{
	}
}
