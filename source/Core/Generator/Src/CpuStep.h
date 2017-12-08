/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_CPU_STEP_H___
#define ___GENERATOR_CPU_STEP_H___

#include "GeneratorUtils.h"
#include "Buffer.h"
#include "GeneratorSignal.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <chrono>

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		23/05/2012
	@brief
		Worker thread
	@remarks
		A worker thread is defined to work on a vertical portion of the final surface.
		< br / >This portion is defined by a top and a bottom
		< br / >When the worker thread has finished it's job it sends an event to the generator
	*/
	class GeneratorAPI Thread
	{
	public:
		/** Constructor
		@param[in] p_pParent
			The parent generator
		@param[in] p_uiIndex
			The thread index
		@param[in] p_iWidth
			The surface width
		@param[in] p_iTop
			The surface portion's top
		@param[in] p_iBottom
			The surface portion's bottom
		@param[in] p_iHeight
			The surface height
		*/
		Thread( std::shared_ptr< CpuStepBase > p_pParent, size_t p_uiIndex, int p_iWidth, int p_iTop, int p_iBottom, int p_iHeight );

		/** Destructor
		*/
		virtual ~Thread();

		/** Launches the thread
		*/
		void Run();

		/** Stops the thread
		*/
		void Stop();

		/** Connects a function to the thread end signal
		@param[in] p_function
			The function to connect
		*/
		template< typename Function >
		inline void Connect( Function p_function )
		{
			m_threadEndIndex = m_signalThreadEnd.Connect( p_function );
		}

		/** Disconnects from the thread end signal
		*/
		inline void Disconnect()
		{
			m_signalThreadEnd.Disconnect( m_threadEndIndex );
		}

		/** Tells the thread is stopped or not
		@return
			The stop status
		*/
		inline bool IsStopped()const
		{
			return m_stopped;
		}

		/** Tells thread is launched or not
		@return
			The launch status
		*/
		inline bool IsLaunched()const
		{
			return m_launched;
		}

		/** Launches the thread
		*/
		inline void Launch()
		{
			m_launched = true;
		}

	protected:
		/** The computing function
		*/
		virtual void DoStep() = 0;

	public:
		//! The thread index
		Signal< std::function< void( size_t ) > > m_signalThreadEnd;

	protected:
		//! The mutex, mutable because it has no influence on the thread status
		mutable std::mutex m_mutex;
		//! The ProceduralGenerator
		std::weak_ptr< CpuStepBase > m_pParent;
		//! The compute surface width
		int m_iWidth;
		//! The compute surface height
		int m_iHeight;
		//! The surface portion's top
		int m_iTop;
		//! The surface portion's bottom
		int m_iBottom;
		//! The thread index
		size_t m_uiIndex;
		//! The thread end signal index
		uint32_t m_threadEndIndex;

	private:
		//! Tells the thread has been launched
		std::atomic_bool m_launched;
		//! Thread stopped status
		std::atomic_bool m_stopped;
		//! The thread
		std::thread m_thread;
		//! End thread condition
		mutable std::condition_variable m_stoppedCondition;
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/03/2015
	@version
		2.0.0
	@brief
		Base class for every CPU step
	*/
	class GeneratorAPI CpuStepBase
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The computing surface dimensions
		*/
		CpuStepBase( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size );

		/** Destructor
		*/
		virtual ~CpuStepBase();

		/** Initialises the generator
		*/
		void Initialise();

		/** Cleans up the generator
		*/
		void Cleanup();

		/** Launches the thread
		*/
		void Run();

		/** Stops the thread
		*/
		void Stop();

		/** Swaps background and foreground buffers
		*/
		void SwapBuffers();

		/** Executes the computations
		*/
		void Render();

		/** Resize the generator computation dimensions
		@param[in] p_iWidth, p_iHeight
			The new dimensions
		*/
		void Resize( int p_iWidth, int p_iHeight );

		/** Resize the generator computation dimensions
		@param[in] p_size
			The new dimensions
		*/
		void Resize( Size const & p_size );

		/** Wakes the step thread
		*/
		void Wake();

		/** Waits for the step thread end
		@param[in] p_timeout
			The maximum time to wait for
		*/
		void Wait( std::chrono::milliseconds p_timeout );

		/** The thread end event callback function
		@param[in] p_index
			The thread index
		*/
		void OnThreadEnd( size_t p_index );

		/** Tells if the generator is initialised
		@return
			The value
		*/
		inline bool IsInitialised()const
		{
			return m_initialised;
		}

		/** Tells if the thread is stopped
		@return
			The value
		*/
		inline bool IsStopped()const
		{
			return m_stopped;
		}

		/** Retrieves the computing image dimensions
		@return
			The value
		*/
		inline Size const & GetImageSize()const
		{
			return m_sizeImage;
		}

		/** Retrieves the computing buffer
		@return
			The value
		*/
		inline std::shared_ptr< PixelBuffer > GetBuffer()const
		{
			return m_finalBuffer;
		}

		/** Retrieves the CPU step duration
		@return
			The value
		*/
		inline std::chrono::milliseconds const & GetTime()const
		{
			return m_time;
		}

	protected:
		/** Thread specific step initialisation
		*/
		virtual void DoInitialiseStep() = 0;

		/** Initialises the slave threads and the buffers
		*/
		virtual void DoInitialise() = 0;

		/** Clears the slave threads and the buffers
		*/
		virtual void DoCleanup() = 0;

		/** Swaps back and front buffers
		*/
		virtual void DoSwapBuffers() = 0;

		/** Launches the threads
		*/
		virtual void DoThreadsStart() = 0;

		/** Cleans the threads up
		*/
		virtual void DoThreadsCleanup() = 0;

	private:
		/** Renders the frame : parallel execution of CPU and GPU steps
		@param[in] p_swapFunction
			The function used to swap OpenGL buffers
		*/
		void DoStartRender();

	protected:
		//! The parent generator
		std::weak_ptr< GeneratorBase > m_generator;
		//! The computation image dimensions
		Size m_sizeImage;
		//! The back buffer
		std::shared_ptr< PixelBuffer > m_finalBuffer;
		//! The total threads count
		size_t m_threadsCount;
		//! The ended threads count
		std::atomic_size_t m_endedThreadsCount;
		//! Tells the generator is initialised
		bool m_initialised;
		//! The thread
		std::thread m_thread;
		//! The mutex, mutable because it has no influence on the thread status
		mutable std::mutex m_mutex;
		//! Thread stopped status
		std::atomic_bool m_stopped;
		//! Thread stopped condition
		mutable std::condition_variable m_stoppedCondition;
		//! The signal raised when the CPU step is ended
		Signal< std::function< void() > > m_signalCpuStepEnd;
		//! The CPU time text overlay
		std::weak_ptr< TextOverlay > m_textCPUTime;
		//! The mutex used to wake the step thread
		std::mutex m_mutexWake;
		//! The condition variable used to wake the step thread
		std::condition_variable m_conditionWake;
		//! The mutex used to wait for the step thread end
		std::mutex m_mutexEnd;
		//! The condition variable used to wait for the step thread end
		std::condition_variable m_conditionEnd;
		//! The CPU step duration
		std::chrono::milliseconds m_time;
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/03/2015
	@version
		2.0.0
	@brief
		Base class for every CPU step
	*/
	template< class TThread >
	class CpuStep
		: public CpuStepBase
		, public std::enable_shared_from_this< CpuStep< TThread > >
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The computing surface dimensions
		*/
		CpuStep( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size )
			: CpuStepBase( p_generator, p_size )
		{
		}

		/** Destructor
		*/
		virtual ~CpuStep()
		{
		}

	protected:
		/** Creates a worker thread
		@param[in] iTop
			The top of the image area processed by the thread
		@param[in] iBottom
			The bottom of the image area processed by the thread
		@param[in] iTotalHeight
			The image total height
		*/
		inline void DoCreateThread( int iTop, int iBottom, int iTotalHeight )
		{
			std::unique_ptr< TThread > l_pThread = std::make_unique< TThread >( this->shared_from_this(), m_workerThreads.size(), m_sizeImage.x(), iTop, iBottom, iTotalHeight );
			l_pThread->Connect( std::bind( &CpuStepBase::OnThreadEnd, this, std::placeholders::_1 ) );
			m_workerThreads.push_back( std::move( l_pThread ) );
		}

		/** @copydoc ProceduralTextures::CpuStepBase::DoThreadsStart
		*/
		virtual void DoThreadsStart()
		{
			std::unique_lock< std::mutex > l_lock( m_mutexThreads );
			m_threadsCount = m_workerThreads.size();

			for ( auto && l_thread : m_workerThreads )
			{
				l_thread->Launch();
			}
		}

		/** @copydoc ProceduralTextures::CpuStepBase::DoThreadsCleanup
		*/
		virtual void DoThreadsCleanup()
		{
			std::unique_lock< std::mutex > l_lock( m_mutexThreads );

			while ( !m_workerThreads.empty() )
			{
				std::unique_ptr< TThread > & l_thread( *m_workerThreads.begin() );
				l_thread->Stop();
				l_thread->Disconnect();
				m_workerThreads.erase( m_workerThreads.begin() );
			}

			m_workerThreads.clear();
		}

		/** Applies a function to each thread
		@param[in] p_function
			The function to apply
		*/
		inline void DoForEachThread( std::function< void( TThread & ) > p_function )
		{
			std::unique_lock< std::mutex > l_lock( m_mutexThreads );

			for ( auto && l_thread: m_workerThreads )
			{
				p_function( *l_thread );
			}
		}

		/** Applies a function to each thread
		@param[in] p_function
			The function to apply
		*/
		inline void DoForEachThread( std::function< void( TThread const & ) > p_function )const
		{
			std::unique_lock< std::mutex > l_lock( m_mutexThreads );

			for ( auto && l_thread: m_workerThreads )
			{
				p_function( *l_thread );
			}
		}

		/** Applies a function to one thread
		@param[in] p_function
			The function to apply
		*/
		template< typename R >
		inline R DoForOneThread( size_t p_index, std::function< R( TThread & ) > p_function )
		{
			std::unique_lock< std::mutex > l_lock( m_mutexThreads );
			return p_function( *m_workerThreads[p_index] );
		}

		/** Applies a function to one thread
		@param[in] p_function
			The function to apply
		*/
		template< typename R >
		inline R DoForOneThread( size_t p_index, std::function< R( TThread const & ) > p_function )const
		{
			std::unique_lock< std::mutex > l_lock( m_mutexThreads );
			return p_function( *m_workerThreads[p_index] );
		}

	private:
		//! The mutex protecting the threads
		mutable std::mutex m_mutexThreads;
		//! The worker threads
		std::vector< std::unique_ptr< TThread > > m_workerThreads;
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/08/2015
	@version
		2.0.1
	@brief
		Default CPU step, does nothing
	*/
	class GeneratorAPI DefaultCpuStep
		:	public CpuStepBase
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The dimensions
		*/
		DefaultCpuStep( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size );
		/** Destructor
		*/
		virtual ~DefaultCpuStep();

	private:
		/** @copydoc ProceduralTexture::CpuStep::DoInitialiseStep
		*/
		virtual void DoInitialiseStep();
		/** @copydoc ProceduralTexture::CpuStep::DoInitialise
		*/
		virtual void DoInitialise();
		/** @copydoc ProceduralTexture::CpuStep::DoCleanup
		*/
		virtual void DoCleanup();
		/** @copydoc ProceduralTexture::CpuStep::DoSwapBuffers
		*/
		virtual void DoSwapBuffers();
		/** Launches the threads
		*/
		virtual void DoThreadsStart();
		/** Cleans the threads up
		*/
		virtual void DoThreadsCleanup();
	};
}

#endif
