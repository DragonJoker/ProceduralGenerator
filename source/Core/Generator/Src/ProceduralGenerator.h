/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_PROCEDURAL_GENERATOR_H___
#define ___GENERATOR_PROCEDURAL_GENERATOR_H___

#include "TextOverlay.h"
#include "GeneratorUtils.h"
#include "ControlInfos.h"
#include "Buffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Base class for every generator
	*/
	class GeneratorAPI ProceduralGenerator
		: public wxThread
		, public wxEvtHandler
	{
	protected:
		/*!
		\author		Sylvain DOREMUS
		\date		23/05/2012
		\brief		Worker thread
		\remark		A worker thread is defined to work on a vertical portion of the final surface.
					< br / >This portion is defined by a top and a bottom
					< br / >When the worker thread has finished it's job it sends an event to the generator
		*/
		class GeneratorAPI Thread
			: public wxThread
		{
		public:
			/**
			 *\brief		Constructor
			 *\param[in]	p_pParent	The parent generator
			 *\param[in]	p_uiIndex	The thread index
			 *\param[in]	p_iWidth	The surface width
			 *\param[in]	p_iTop		The surface portion's top
			 *\param[in]	p_iBottom	The surface portion's bottom
			 *\param[in]	p_iHeight	The surface height
			 *\param[in]	p_pxColour	The pixel colour
			 */
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int p_iWidth, int p_iTop, int p_iBottom, int p_iHeight, const UbPixel & p_pxColour );
			/**
			 *\brief		Destructor
			 */
			virtual ~Thread();
			/**
			 *\brief		The thread function
			 */
			virtual ExitCode Entry();
			/**
			 *\brief		The computing function
			 *\remark		To be implemented by derived classes
			 */
			virtual void Step() = 0;
			/**
			 *\brief		Tells the thread is stopped or not
			 *\return		The stop status
			 */
			inline bool IsStopped()const
			{
				wxMutexLocker l_locker( m_mutex );
				return m_bStopped;
			}
			/**
			 *\brief		Stops the thread
			 */
			inline void Stop()
			{
				wxMutexLocker l_locker( m_mutex );
				m_bStopped = true;
			}
			/**
			 *\brief		Tells thread is launched or not
			 *\return		The launch status
			 */
			inline bool IsLaunched()const
			{
				wxMutexLocker l_locker( m_mutex );
				return m_bLaunched;
			}
			/**
			 *\brief		Launches the thread
			 */
			inline void Launch()
			{
				wxMutexLocker l_locker( m_mutex );
				m_bLaunched = true;
			}
			/**
			 *\brief		Sets the red value of the pixel
			 */
			inline void SetRed( uint8_t val )
			{
				m_pxColour.r = val;
			}
			/**
			 *\brief		Sets the green value of the pixel
			 */
			inline void SetGreen( uint8_t val )
			{
				m_pxColour.g = val;
			}
			/**
			 *\brief		Sets the blue value of the pixel
			 */
			inline void SetBlue( uint8_t val )
			{
				m_pxColour.b = val;
			}

		private:
			//! Tells the thread is stopped
			bool m_bStopped;
			//! Tells the thread has been launched
			bool m_bLaunched;

		protected:
			//! The mutex, mutable because it has no real influence on the thread status
			mutable wxMutex m_mutex;
			//! The ProceduralGenerator
			ProceduralGenerator * m_pParent;
			//! The compute surface width
			int m_iWidth;
			//! The compute surface height
			int m_iHeight;
			//! The surface portion's top
			int m_iTop;
			//! The surface portion's bottom
			int m_iBottom;
			//! The pixel colour (image is in computed in white and m_pxColour )
			UbPixel m_pxColour;
			//! The thread index
			size_t m_uiIndex;
		};

	public:
		ProceduralGenerator( int p_width, int p_height, int p_iWndId, wxFrame * p_pMainFrame, bool p_bNeedsConfigPanel = true, int p_iSleepTime = 10 );
		virtual ~ProceduralGenerator();

		virtual ExitCode Entry();

		void SetRed( uint8_t val );
		void SetGreen( uint8_t val );
		void SetBlue( uint8_t val );
		virtual void SwapBuffers();
		void InitialiseStep();
		void ClearAllThreads();
		void GlInitialise();
		void GlPreRender();
		void GlRender( bool p_bChanged );
		void GlPostRender();
		void GlCleanup();

		inline void SetCpuStepEndHandler( wxObjectEventFunction p_pfnHandler )
		{
			m_pfnCpuStepEndHandler = p_pfnHandler;
		}

		void WaitForStepEnd();
		void Resize( int p_iWidth, int p_iHeight );
		void Resize( const wxSize & p_size );
		void SetSaveFrame();
		void ShowMessageBox( wxString const & p_title, wxString const & p_message, int p_flags = wxOK | wxCENTRE );

		template < class ThreadClass >
		ThreadClass * CreateThread( int WXUNUSED( iWidth ), int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		{
			ThreadClass * l_pThread = new ThreadClass( this, m_arraySlaveThreads.size(), m_sizeImage.x, iTop, iBottom, iTotalHeight, p_pxColour );
			m_arraySlaveThreads.push_back( l_pThread );
			l_pThread->Create();
			return l_pThread;
		}

		inline uint8_t GetRed()const
		{
			return m_pxColour.r;
		}
		inline uint8_t GetGreen()const
		{
			return m_pxColour.g;
		}
		inline uint8_t GetBlue()const
		{
			return m_pxColour.b;
		}
		inline bool IsInitialised()const
		{
			return m_initialised;
		}
		inline const UbPixel * GetPixels()const
		{
			return m_frontBuffer.const_ptr();
		}
		inline const PixelBuffer * GetPixelsBuffer()const
		{
			return & m_frontBuffer;
		}
		inline PixelBuffer * GetPixelsBuffer()
		{
			return & m_frontBuffer;
		}
		inline wxSize const & GetDisplaySize()const
		{
			return m_sizeDisplay;
		}
		inline wxSize const & GetImageSize()const
		{
			return m_sizeImage;
		}
		inline bool NeedsConfigPanel()const
		{
			return m_bNeedsConfigPanel;
		}
		inline wxFrame * GetMainFrame()const
		{
			return m_pMainFrame;
		}
		inline bool IsRendered()const
		{
			wxMutexLocker l_locker( m_mutex );
			return m_bRendered;
		}
		inline void SetRendered( bool p_bVal )
		{
			wxMutexLocker l_locker( m_mutex );
			m_bRendered = p_bVal;
		}
		inline bool IsStopped()const
		{
			wxMutexLocker l_locker( m_mutex );
			return m_bStopped;
		}
		inline void Stop()
		{
			wxMutexLocker l_locker( m_mutex );
			m_bStopped = true;
			ClearAllThreads();
		}
		inline CtrlInfosArray::iterator	BeginCtrls()
		{
			return m_arrayControls.begin();
		}
		inline CtrlInfosArray::iterator	EndCtrls()
		{
			return m_arrayControls.end();
		}
		inline void SetDisplaySize( const wxSize & p_size )
		{
			m_sizeDisplay = p_size;
		}
		inline PixelBuffer const & GetSavedFrame()const
		{
			return m_saveBuffer;
		}
		inline PixelBuffer & GetSavedFrame()
		{
			return m_saveBuffer;
		}

	protected:
		/**
		 *\brief		Initialises the configuration panel structure
		 */
		virtual void DoGeneratePanel() = 0;
		/**
		 *\brief		Resize things like frame buffers, render textures, internal buffers, ...
		 */
		virtual void DoResize( const wxSize & WXUNUSED( p_size ) ) {}
		/**
		 *\brief		Defines red value for generators that use it
		 */
		virtual void DoSetRed( uint8_t WXUNUSED( val ) ) {}
		/**
		 *\brief		Defines green value for generators that use it
		 */
		virtual void DoSetGreen( uint8_t WXUNUSED( val ) ) {}
		/**
		 *\brief		Defines blue value for generators that use it
		 */
		virtual void DoSetBlue( uint8_t WXUNUSED( val ) ) {}
		/**
		 *\brief		Thread specific step initialisation
		 */
		virtual void DoInitialiseStep() {}
		/**
		 *\brief		All OpenGL global initialisations must be made in this function
		 */
		virtual void DoGlInitialise() = 0;
		/**
		 *\brief		All OpenGL frame initialisations must be made in this function
		 */
		virtual void DoGlPreRender() = 0;
		/**
		 *\brief		All OpenGL draw calls must be made in this function
		 *\remark		Drawings will go in the generator frame buffer
		 */
		virtual void DoGlRender( bool & p_bChanged ) = 0;
		/**
		 *\brief		All OpenGL frame cleanups must be made in this function
		 */
		virtual void DoGlPostRender() = 0;
		/**
		 *\brief		All OpenGL global cleanups must be made in this function
		 */
		virtual void DoGlCleanup() = 0;
		/**
		 *\brief		Initialises front and save buffers to the given size
		 */
		void DoInitBuffers( wxSize const & p_size );

		void	DoCleanup();
		wxSize	DoLoadImage( const wxString & p_strImagePath, wxImage & p_pImage, PixelBuffer & p_buffer );
		void	DoSubRender();
		void	DoSaveFrame();

	private:
		void OnThreadEnd( wxCommandEvent & p_event );
		void OnStop( wxCommandEvent & p_event );
		void OnMouseMove( wxMouseEvent & p_event );

	protected:
		wxSize m_sizeDisplay;
		wxSize m_sizeImage;
		unsigned long long m_step;
		PixelBuffer m_frontBuffer;
		PixelBuffer m_backBuffer;
		PixelBuffer m_saveBuffer;
		UbPixel m_pxColour;
		size_t m_uiThreadCount;
		size_t m_uiThreadEndedCount;
		bool m_bStopped;
		wxFrame * m_pMainFrame;
		bool m_bNeedsConfigPanel;
		std::vector< Thread * > m_arraySlaveThreads;
		bool m_initialised;
		GlVertexBuffer m_vertexBuffer;
		GlIndexBuffer m_indexBuffer;
		GlTexture m_texture;
		GlFrameBuffer m_frameBuffer;
		GlTexture m_textureOverlays;
		GlFrameBuffer m_frameBufferOverlays;
		GlTexture m_textureSave;
		GlFrameBuffer m_frameBufferSave;
		bool m_bToSave;
		mutable wxMutex m_mutex;
		bool m_bRendered;
		wxObjectEventFunction m_pfnCpuStepEndHandler;
		CtrlInfosArray m_arrayControls;
		wxLongLong m_sleepTime;
		wxPoint m_ptMousePosition;
		OpenGl m_openGl;
		TextOverlay m_overlayStatus;
		wxImage m_image;
	};
}

#endif
