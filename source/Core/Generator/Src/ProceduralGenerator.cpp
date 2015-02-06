#include "ProceduralGenerator.h"

namespace ProceduralTextures
{
#ifdef _WIN32
#	define LibError() GetLastError()
#else
#	define LibError() dlerror()
#endif

	ProceduralGenerator::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	wxThread( wxTHREAD_DETACHED )
		,	m_pParent( p_pParent )
		,	m_uiIndex( p_uiIndex )
		,	m_iWidth( iWidth )
		,	m_iBottom( iBottom )
		,	m_iTop( iTop )
		,	m_iHeight( iTotalHeight )
		,	m_pxColour( p_pxColour )
		,	m_bStopped( false )
		,	m_bLaunched( false )
	{
	}

	ProceduralGenerator::Thread::~Thread()
	{
	}

	wxThread::ExitCode ProceduralGenerator::Thread::Entry()
	{
		while ( !IsStopped() )
		{
			if ( IsLaunched() )
			{
				// Lock thread data
				m_mutex.Lock();
				m_bLaunched = false;
				// Make computations
				Step();
				// Unlock thread data
				m_mutex.Unlock();
				// Tell parent I am ended
				wxCommandEvent l_event( wxEVT_NULL, eEVENT_WORKER_THREAD_END );
				l_event.SetInt( int( m_uiIndex ) );
				m_pParent->AddPendingEvent( l_event );
			}
			else
			{
				wxMilliSleep( 10 );
			}
		}

		return 0;
	}

	//*************************************************************************************************

	ProceduralGenerator::ProceduralGenerator( int p_width, int p_height, int WXUNUSED( p_iWndId ), wxFrame * p_pMainFrame, bool p_bNeedsConfigPanel, int p_iSleepTime )
		:	wxThread( wxTHREAD_DETACHED )
		,	m_openGl()
		,	m_sizeDisplay( p_width, p_height )
		,	m_sizeImage( p_width, p_height )
		,	m_step( 0 )
		,	m_initialised( false )
		,	m_pxColour( 255, 255, 255, 255 )
		,	m_bStopped( false )
		,	m_frontBuffer( Size( p_width, p_height ) )
		,	m_backBuffer( Size( p_width, p_height ) )
		,	m_saveBuffer( Size( p_width, p_height ) )
		,	m_pMainFrame( p_pMainFrame )
		,	m_bNeedsConfigPanel( p_bNeedsConfigPanel )
		,	m_bToSave( false )
		,	m_bRendered( true )
		,	m_sleepTime( p_iSleepTime )
		,	m_pfnCpuStepEndHandler( NULL )
		,	m_texture( &m_openGl )
		,	m_frameBuffer( &m_openGl )
		,	m_textureOverlays( &m_openGl )
		,	m_frameBufferOverlays( &m_openGl )
		,	m_textureSave( &m_openGl )
		,	m_frameBufferSave( &m_openGl )
		,	m_vertexBuffer( &m_openGl )
		,	m_indexBuffer( &m_openGl )
		,	m_overlayStatus( &m_openGl )
	{
		m_texture.Resize( m_sizeImage );
		m_frameBuffer.Resize( m_sizeImage );
		m_frameBuffer.AttachTexture( GL_COLOR_ATTACHMENT0, &m_texture );
		m_textureOverlays.Resize( m_sizeImage );
		m_frameBufferOverlays.Resize( m_sizeImage );
		m_frameBufferOverlays.AttachTexture( GL_COLOR_ATTACHMENT0, &m_textureOverlays );
		m_frameBufferSave.AttachTexture( GL_COLOR_ATTACHMENT0, &m_textureSave );
		m_uiThreadCount = GetCPUCount();// - 1;
		Connect( eEVENT_WORKER_THREAD_END,	wxEVT_NULL,		wxCommandEventHandler(	ProceduralGenerator::OnThreadEnd ) );
		Connect( eEVENT_THREAD_STOP,		wxEVT_NULL,		wxCommandEventHandler(	ProceduralGenerator::OnStop ) );
		Connect( wxEVT_MOTION, wxMouseEventHandler( ProceduralGenerator::OnMouseMove ), NULL, this );
		m_overlayStatus.SetSize( wxSize( p_width, 30 ) );
		m_overlayStatus.SetPosition( wxPoint( 0, p_height - 30 ) );
		m_overlayStatus.Create( 20, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
	}

	ProceduralGenerator::~ProceduralGenerator()
	{
	}

	wxThread::ExitCode ProceduralGenerator::Entry()
	{
		wxMilliClock_t	l_beforeTime;
		wxMilliClock_t	l_time;
		ExitCode 		l_eReturn = 0;

		while ( !IsStopped() && !wxThread::TestDestroy() )
		{
			l_beforeTime = wxGetLocalTimeMillis();

			if ( m_initialised && IsRendered() )
			{
				SetRendered( false );
				SwapBuffers();
				InitialiseStep();
				WaitForStepEnd();
				wxCommandEvent 	l_event( wxEVT_NULL, eEVENT_CPU_STEP_END );
				m_pMainFrame->GetEventHandler()->AddPendingEvent( l_event );
			}

			l_time = wxGetLocalTimeMillis() - l_beforeTime;

			if ( l_time < m_sleepTime )
			{
				wxMilliSleep( ( m_sleepTime - l_time ).ToLong() );
			}
			else
			{
				wxMilliSleep( m_sleepTime.ToLong() );
			}
		}

		Disconnect( eEVENT_WORKER_THREAD_END, wxEVT_NULL,	wxCommandEventHandler(	ProceduralGenerator::OnThreadEnd ) );
		Disconnect( eEVENT_THREAD_STOP, wxEVT_NULL,			wxCommandEventHandler(	ProceduralGenerator::OnStop ) );
		Disconnect( wxEVT_MOTION,							wxMouseEventHandler(	ProceduralGenerator::OnMouseMove ), NULL, this );
		DoCleanup();
		return l_eReturn;
	}

	void ProceduralGenerator::SetRed( uint8_t val )
	{
		m_pxColour.r = val;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] )
			{
				m_arraySlaveThreads[i]->SetRed( val );
			}
		}

		DoSetRed( val );
	}

	void ProceduralGenerator::SetGreen( uint8_t val )
	{
		m_pxColour.g = val;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] )
			{
				m_arraySlaveThreads[i]->SetGreen( val );
			}
		}

		DoSetGreen( val );
	}

	void ProceduralGenerator::SetBlue( uint8_t val )
	{
		m_pxColour.b = val;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] )
			{
				m_arraySlaveThreads[i]->SetBlue( val );
			}
		}

		DoSetBlue( val );
	}

	void ProceduralGenerator::SwapBuffers()
	{
		m_frontBuffer = m_backBuffer;
	}

	void ProceduralGenerator::InitialiseStep()
	{
		DoInitialiseStep();
		m_uiThreadEndedCount = 0;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				m_arraySlaveThreads[i]->Launch();
			}
		}
	}

	void ProceduralGenerator::ClearAllThreads()
	{
		m_initialised = false;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			ProceduralGenerator::Thread * l_pThread = m_arraySlaveThreads[i];
			m_arraySlaveThreads[i] = NULL;

			if ( l_pThread )
			{
				l_pThread->Stop();
			}
		}

		m_arraySlaveThreads.clear();
	}

	void ProceduralGenerator::GlInitialise()
	{
		m_openGl.Initialise();
		m_openGl.ClearColor( 1.0, 1.0, 1.0, 1.0 );
		m_openGl.ShadeModel( GL_FLAT );
		m_texture.Create();
		m_frameBuffer.Create();
		m_textureOverlays.Create();
		m_frameBufferOverlays.Create();
		m_textureSave.Create();
		m_frameBufferSave.Create();
		m_vertexBuffer.Initialise();
		m_indexBuffer.Initialise();
		m_overlayStatus.Initialise();
		m_overlayStatus.SetText( wxT( "Coin" ) );
		m_overlayStatus.SetBackground( wxColour( 0, 0, 0, 127 ) );
		m_overlayStatus.SetForeground( *wxWHITE );
		DoGlInitialise();
	}

	void ProceduralGenerator::GlPreRender()
	{
		m_texture.Initialise( &m_frontBuffer );
		m_frameBuffer.Initialise( m_sizeImage );
		m_textureOverlays.Initialise( m_sizeImage );
		m_frameBufferOverlays.Initialise( m_sizeImage );
		m_textureSave.Initialise( m_sizeImage );
		m_frameBufferSave.Initialise( m_sizeImage );
		DoGlPreRender();
	}

	void ProceduralGenerator::GlRender( bool p_bChanged )
	{
		bool l_bChanged( p_bChanged );
		DoGlRender( l_bChanged );
		m_openGl.Viewport( 0, 0, m_sizeImage.x, m_sizeImage.y );

		if ( m_bToSave )
		{
			m_frameBufferSave.Activate();
		}
		else
		{
			m_frameBufferOverlays.Activate();
		}

		m_openGl.MatrixMode( GL_PROJECTION );
		m_openGl.LoadIdentity();
		m_openGl.Ortho( 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 );
		m_openGl.MatrixMode( GL_MODELVIEW );
		m_openGl.LoadIdentity();
		m_openGl.Clear( GL_COLOR_BUFFER_BIT );
		m_texture.Activate();

		if ( l_bChanged )
		{
			m_texture.UploadAsync();
		}

		DoSubRender();
		m_texture.Deactivate();

		if ( m_bToSave )
		{
			DoSaveFrame();
			m_frameBufferSave.Deactivate();
			m_frameBufferOverlays.Activate();
			m_textureSave.Activate();
			DoSubRender();
			m_textureSave.Deactivate();
		}

		m_frameBufferOverlays.Deactivate();
		//	m_overlayStatus.Draw( m_frameBufferOverlays );
		m_openGl.Viewport( 0, 0, m_sizeDisplay.x, m_sizeDisplay.y );
		m_openGl.MatrixMode( GL_PROJECTION );
		m_openGl.LoadIdentity();
		m_openGl.Ortho( 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 );
		m_openGl.MatrixMode( GL_MODELVIEW );
		m_openGl.LoadIdentity();
		m_openGl.Clear( GL_COLOR_BUFFER_BIT );
		m_textureOverlays.Activate();
		DoSubRender();
		m_textureOverlays.Deactivate();
	}

	void ProceduralGenerator::GlPostRender()
	{
		DoGlPostRender();
		SetRendered( true );
	}

	void ProceduralGenerator::GlCleanup()
	{
		m_texture.Cleanup();
		m_textureOverlays.Cleanup();
		m_textureSave.Cleanup();
		m_overlayStatus.Cleanup();
		m_frameBuffer.Cleanup();
		m_frameBufferOverlays.Cleanup();
		m_frameBufferSave.Cleanup();

		m_vertexBuffer.Destroy();
		m_indexBuffer.Destroy();
		m_texture.Destroy();
		m_textureOverlays.Destroy();
		m_textureSave.Destroy();
		m_frameBuffer.Destroy();
		m_frameBufferOverlays.Destroy();
		m_frameBufferSave.Destroy();

		DoGlCleanup();
		m_openGl.Cleanup();
	}

	void ProceduralGenerator::WaitForStepEnd()
	{
		std::size_t l_uiThreadCount = m_arraySlaveThreads.size();

		while ( m_uiThreadEndedCount < l_uiThreadCount && ! IsStopped() )
		{
			wxMilliSleep( 1 );
		}
	}

	void ProceduralGenerator::Resize( int p_iWidth, int p_iHeight )
	{
		Resize( wxSize( p_iWidth, p_iHeight ) );
	}

	void ProceduralGenerator::Resize( const wxSize & p_size )
	{
		m_sizeImage = p_size;
		m_sizeDisplay = p_size;
		wxSize l_sizeConfig( 0, 0 );
		DoInitBuffers( p_size );

		if ( m_bNeedsConfigPanel )
		{
			l_sizeConfig = wxSize( 150, 0 );
		}

		wxDisplay l_display;
		wxSize l_maxSize = l_display.GetClientArea().GetSize() + m_pMainFrame->GetClientSize() - m_pMainFrame->GetSize() - wxSize( l_sizeConfig.x, 0 );
		double l_dRatio = double( m_sizeDisplay.x ) / double( m_sizeDisplay.y );
		bool l_bChanged = false;
		l_maxSize = wxSize(	std::min< int >( l_maxSize.x, int( l_maxSize.y * l_dRatio ) ),
							std::min< int >( l_maxSize.y, int( l_maxSize.x / l_dRatio ) ) );

		while ( m_sizeDisplay.x > l_maxSize.x || m_sizeDisplay.y > l_maxSize.y )
		{
			if ( m_sizeDisplay.x > l_maxSize.x )
			{
				m_sizeDisplay.x = l_maxSize.x;
				m_sizeDisplay.y = int( m_sizeDisplay.x / l_dRatio );
			}
			else if ( m_sizeDisplay.y > l_maxSize.y )
			{
				m_sizeDisplay.y = l_maxSize.y;
				m_sizeDisplay.x = int( m_sizeDisplay.y * l_dRatio );
			}

			l_bChanged = true;
		}

		wxCommandEvent l_event( wxEVT_NULL, eGUI_EVENT_RESIZE_PARENT );
		l_event.SetExtraLong( m_sizeDisplay.x + l_sizeConfig.x );
		l_event.SetInt( m_sizeDisplay.y );
		m_pMainFrame->GetEventHandler()->AddPendingEvent( l_event );
		m_texture.Resize( m_sizeImage );
		m_frameBuffer.Resize( m_sizeImage );
		m_textureOverlays.Resize( m_sizeImage );
		m_frameBufferOverlays.Resize( m_sizeImage );
		m_textureSave.Resize( m_sizeImage );
		m_frameBufferSave.Resize( m_sizeImage );
		DoResize( p_size );
	}

	void ProceduralGenerator::SetSaveFrame()
	{
		m_bToSave = true;
	}

	void ProceduralGenerator::ShowMessageBox( wxString const & p_title, wxString const & p_message, int p_flags )
	{
		wxCommandEvent l_event( wxEVT_NULL, eGUI_EVENT_SHOW_MESSAGE_BOX );
		l_event.SetString( p_title + wxT( "||" ) + p_message );
		l_event.SetInt( p_flags );
		m_pMainFrame->GetEventHandler()->AddPendingEvent( l_event );
	}

	void ProceduralGenerator::DoInitBuffers( wxSize const & p_size )
	{
		Size l_size( p_size.x, p_size.y );
		m_frontBuffer.init( l_size );
		m_saveBuffer.init( l_size );
	}

	void ProceduralGenerator::DoCleanup()
	{
		ClearAllThreads();
		m_frontBuffer.clear();
		m_backBuffer.clear();
		m_saveBuffer .clear();
	}

	wxSize ProceduralGenerator::DoLoadImage( const wxString & p_strImagePath, wxImage & p_image, PixelBuffer & p_buffer )
	{
		wxSize l_sizeReturn;

		if ( p_image.LoadFile( p_strImagePath ) )
		{
			wxDisplay l_display;
			wxSize l_sizeConfig;

			if ( m_bNeedsConfigPanel )
			{
				l_sizeConfig = wxSize( 150, 0 );
			}

			l_sizeReturn.x = p_image.GetWidth();
			l_sizeReturn.y = p_image.GetHeight();
			p_image = p_image.Mirror( false );
			p_buffer.init( Size( l_sizeReturn.x, l_sizeReturn.y ) );

			if ( !p_image.HasAlpha() )
			{
				p_image.InitAlpha();
			}

			if ( p_image.HasAlpha() )
			{
				p_buffer.set< PG_UINT8_RGBA >( p_image.GetData(), p_image.GetAlpha() );
			}
			else
			{
				p_buffer.set< PG_UINT8_RGB >( p_image.GetData(), NULL );
			}
		}
		else
		{
			ShowMessageBox( _( "Image loading error" ), _( "No handler found for this image format" ), wxOK | wxCENTRE | wxICON_ERROR );
		}

		return l_sizeReturn;
	}

	void ProceduralGenerator::DoSubRender()
	{
		if ( m_vertexBuffer.Activate() )
		{
			if ( m_indexBuffer.Activate() )
			{
				m_openGl.DrawElements( GL_TRIANGLES, GlIndexBuffer::Size, GL_UNSIGNED_INT, 0 );
				m_indexBuffer.Deactivate();
			}

			m_vertexBuffer.Deactivate();
		}
	}

	void ProceduralGenerator::DoSaveFrame()
	{
		if ( m_bToSave )
		{
			m_frameBufferSave.DownloadAsync( GL_COLOR_ATTACHMENT0, m_frontBuffer );
			m_saveBuffer = m_frontBuffer;
			m_bToSave = false;
		}
	}

	void ProceduralGenerator::OnThreadEnd( wxCommandEvent & p_event )
	{
		m_uiThreadEndedCount++;
		p_event.Skip();
	}

	void ProceduralGenerator::OnStop( wxCommandEvent & p_event )
	{
		m_bStopped = true;

		if ( m_pfnCpuStepEndHandler )
		{
			m_pMainFrame->GetEventHandler()->Disconnect( eEVENT_CPU_STEP_END, wxEVT_NULL );
		}

		p_event.Skip();
	}

	void ProceduralGenerator::OnMouseMove( wxMouseEvent & p_event )
	{
		m_ptMousePosition = p_event.GetPosition();
		p_event.Skip();
	}
}
