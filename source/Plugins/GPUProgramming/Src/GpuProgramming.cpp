#include "GpuProgramming.h"

#include <cmath>

namespace ProceduralTextures
{
	wxString g_shaChoices[] =
	{
		wxT( "New..." )
	};

	GpuProgramming::GpuProgramming( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame, true, 20 )
		,	m_pSelectedEffect( NULL )
		,	m_specButtonImage( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Image" ) ) )
		,	m_specButtonReset( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Reset Time" ) ) )
		,	m_specStaticShaders( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Shaders" ) ) )
		,	m_specComboShaders( SpecificControlParameters< eCONTROL_TYPE_COMBO >( wxT( "" ), g_shaChoices ) )
		,	m_specButtonVertexFile( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Vertex" ) ) )
		,	m_specButtonFragmentFile( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Pixel" ) ) )
		,	m_specButtonCompile( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Compile" ) ) )
		,	m_specButtonCompilerLog( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Compiler log" ) ) )
		,	m_specButtonRemove( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Remove" ) ) )
		,	m_frameBuffer1( &m_openGl )
		,	m_frameBuffer2( &m_openGl )
		,	m_texture1( &m_openGl )
		,	m_texture2( &m_openGl )
	{
		m_specButtonVertexFile.m_strWildcard = _( "Vertex shader GLSL files (*.glsl;*.vert )|*.glsl;*.vert" );
		m_specButtonFragmentFile.m_strWildcard = _( "Fragment shader GLSL files (*.glsl;*.frag )|*.glsl;*.frag" );
		m_texture2.Resize( m_sizeImage );
		m_frameBuffer1.Resize( m_sizeImage );
		m_frameBuffer2.Resize( m_sizeImage );
		m_frameBuffer1.AttachTexture( GL_COLOR_ATTACHMENT0, &m_texture1 );
		m_frameBuffer2.AttachTexture( GL_COLOR_ATTACHMENT0, &m_texture2 );
		wxSystemOptions::SetOption( wxT( "msw.remap" ), 0 );
		wxInitAllImageHandlers();
		DoGeneratePanel();
	}

	GpuProgramming::~GpuProgramming()
	{
		for ( EffectPtrArray::iterator l_it = m_arrayEffects.begin() ; l_it != m_arrayEffects.end() ; ++l_it )
		{
			delete *l_it;
		}

		m_arrayEffects.clear();
		wxImage::CleanUpHandlers();
		m_initialised = false;
		DoCleanup();
	}

	void GpuProgramming::SetImagePath( const wxString & p_strImagePath )
	{
		m_initialised = false;

		if ( m_image.LoadFile( p_strImagePath ) )
		{
			wxDisplay l_display;
			wxSize l_size;
			l_size.x = m_image.GetWidth();
			l_size.y = m_image.GetHeight();

			if ( l_size != m_sizeImage )
			{
				m_image = m_image.Rescale( m_sizeImage.x, m_sizeImage.y, wxIMAGE_QUALITY_HIGH ).Mirror( false );
			}
			else
			{
				m_image = m_image.Mirror( false );
			}

			m_backBuffer.init( Size( m_sizeImage.x, m_sizeImage.y ) );

			if ( m_image.HasAlpha() )
			{
				m_backBuffer.set< PG_UINT8_RGBA >( m_image.GetData(), m_image.GetAlpha() );
			}
			else
			{
				m_backBuffer.set< PG_UINT8_RGB >( m_image.GetData(), NULL );
			}

			Resize( m_sizeImage );

			for ( EffectPtrArray::iterator l_it = m_arrayEffects.begin() ; l_it != m_arrayEffects.end() ; ++l_it )
			{
				( *l_it )->Compile();
			}
		}
		else
		{
			ShowMessageBox( _( "Image loading error" ), _( "No handler found for this image format" ), wxOK | wxCENTRE | wxICON_ERROR );
		}

		m_initialised = true;
		SetRendered( true );
	}

	void GpuProgramming::ResetTime()
	{
		for ( EffectPtrArray::iterator l_it = m_arrayEffects.begin() ; l_it != m_arrayEffects.end() ; ++l_it )
		{
			( *l_it )->ResetTime();
		}
	}

	bool GpuProgramming::EffectAdd()
	{
		Effect * l_pEffect = new Effect( &m_openGl, m_sizeImage.x, m_sizeImage.y );
		m_arrayEffects.push_back( l_pEffect );
		return true;
	}

	bool GpuProgramming::EffectRemove( size_t p_uiIndex )
	{
		bool l_bReturn = p_uiIndex < m_arrayEffects.size();

		if ( l_bReturn )
		{
			Effect * l_pEffect = m_arrayEffects[p_uiIndex];
			std::vector < Effect * >::iterator l_it = m_arrayEffects.begin();
			m_arrayEffects.erase( l_it + p_uiIndex );
			delete l_pEffect;
		}

		return l_bReturn;
	}

	void GpuProgramming::EffectSetVertexFile( size_t p_uiIndex, const wxString & p_strPath )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->SetVertexFile( p_strPath );
		}
	}

	void GpuProgramming::EffectSetFragmentFile( size_t p_uiIndex, const wxString & p_strPath )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->SetFragmentFile( p_strPath );
		}
	}

	void GpuProgramming::EffectCompile( size_t p_uiIndex )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->Compile();
		}
	}

	wxString GpuProgramming::EffectGetCompilerLog( size_t p_uiIndex, eSHADER_OBJECT_TYPE p_eType )
	{
		wxString l_strReturn;

		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->GetCompilerLog( p_eType );
		}

		return l_strReturn;
	}

	wxString GpuProgramming::EffectGetLinkerLog( size_t p_uiIndex )
	{
		wxString l_strReturn;

		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->GetLinkerLog();
		}

		return l_strReturn;
	}

	void GpuProgramming::DoGlInitialise()
	{
		m_texture1.Create();
		m_texture2.Create();
		m_frameBuffer1.Create();
		m_frameBuffer2.Create();
	}

	void GpuProgramming::DoGlPreRender()
	{
		m_texture1.Initialise( &m_backBuffer );
		m_texture2.Initialise( m_sizeImage );
		m_frameBuffer1.Initialise( wxSize( m_backBuffer.dimensions()[0], m_backBuffer.dimensions()[1] ) );
		m_frameBuffer2.Initialise( m_sizeImage );

		for ( EffectPtrArray::iterator l_it = m_arrayEffects.begin() ; l_it != m_arrayEffects.end() ; ++l_it )
		{
			if ( !( *l_it )->IsInitialised() )
			{
				( *l_it )->Initialise();
			}
		}

		m_openGl.Viewport( 0, 0, m_sizeImage.x, m_sizeImage.y );
		m_openGl.MatrixMode( GL_PROJECTION );
		m_openGl.LoadIdentity();
		m_openGl.Ortho( 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 );
		m_openGl.MatrixMode( GL_MODELVIEW );
		m_openGl.LoadIdentity();
	}

	void GpuProgramming::DoGlRender( bool & p_bChanged )
	{
		GlFrameBuffer * l_pFrameBufferTexture = &m_frameBuffer1;
		GlFrameBuffer * l_pFrameBufferRender = &m_frameBuffer2;
		m_texture1.Activate();
		m_texture1.UploadAsync();
		m_texture1.Deactivate();

		for ( EffectPtrArray::iterator l_it = m_arrayEffects.begin() ; l_it != m_arrayEffects.end() ; ++l_it )
		{
			if ( l_pFrameBufferRender->Activate() )
			{
				if ( ( *l_it )->Activate( m_ptMousePosition ) )
				{
					m_openGl.Clear( GL_COLOR_BUFFER_BIT );
					l_pFrameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Activate();
					DoSubRender();
					l_pFrameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Deactivate();
					( *l_it )->Deactivate();
				}

				l_pFrameBufferRender->Deactivate();
			}

			std::swap( l_pFrameBufferTexture, l_pFrameBufferRender );
		}

		m_frameBuffer.Activate();
		m_openGl.Clear( GL_COLOR_BUFFER_BIT );
		l_pFrameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Activate();
		DoSubRender();
		l_pFrameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Deactivate();
		m_frameBuffer.Deactivate();
		p_bChanged = false;
	}

	void GpuProgramming::DoGlPostRender()
	{
	}

	void GpuProgramming::DoGlCleanup()
	{
		for ( EffectPtrArray::iterator l_it = m_arrayEffects.begin() ; l_it != m_arrayEffects.end() ; ++l_it )
		{
			( *l_it )->Cleanup();
			delete( *l_it );
		}

		m_arrayEffects.clear();
		m_frameBuffer1.Cleanup();
		m_frameBuffer2.Cleanup();
		m_texture1.Cleanup();
		m_texture2.Cleanup();
		m_frameBuffer1.Destroy();
		m_frameBuffer2.Destroy();
		m_texture1.Destroy();
		m_texture2.Destroy();
	}

	void GpuProgramming::DoResize( const wxSize & WXUNUSED( p_size ) )
	{
		m_frameBuffer1.Resize( m_sizeImage );
		m_frameBuffer2.Resize( m_sizeImage );
		m_texture1.Resize( m_sizeImage );
		m_texture2.Resize( m_sizeImage );
	}

	void GpuProgramming::DoGeneratePanel()
	{
		int iWidth = 150;
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_IMAGEPATH,		wxPoint( 10, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonImage,			0,					eEVENT_TYPE_LOAD_IMAGEA,		wxCommandEventHandler(	GpuProgramming::OnImagePath ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_RESETTIME,		wxPoint( 10, wxDEFAULT_HEIGHT * 1 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonReset,			0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	GpuProgramming::OnResetTime ),	true ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticShaders,		0,					eEVENT_TYPE_NONE,				NULL,																true ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_SHADERS,		wxPoint( 10, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboShaders,			wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED,	wxCommandEventHandler(	GpuProgramming::OnSelectShader ),	true ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_VERTEXFILE,		wxPoint( 10, wxDEFAULT_HEIGHT * 4 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonVertexFile,		0,					eEVENT_TYPE_LOAD_TEXT_FILEA,	wxCommandEventHandler(	GpuProgramming::OnVertexShaderPath ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_FRAGMENTFILE,	wxPoint( 10, wxDEFAULT_HEIGHT * 5 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonFragmentFile,	0,					eEVENT_TYPE_LOAD_TEXT_FILEB,	wxCommandEventHandler(	GpuProgramming::OnFragmentShaderPath ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_COMPILE,		wxPoint( 10, wxDEFAULT_HEIGHT * 6 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonCompile,		0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	GpuProgramming::OnShaderCompile ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_COMPILERLOG,	wxPoint( 10, wxDEFAULT_HEIGHT * 7 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonCompilerLog,	0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	GpuProgramming::OnCompilerLog ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_REMOVE,			wxPoint( 10, wxDEFAULT_HEIGHT * 8 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonRemove,			0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	GpuProgramming::OnRemove ),	false ) );
	}

	void GpuProgramming::OnResetTime( wxCommandEvent & WXUNUSED( p_event ) )
	{
		ResetTime();
	}

	void GpuProgramming::OnSelectShader( wxCommandEvent & p_event )
	{
		unsigned int l_uiSelection = ( unsigned int )p_event.GetSelection();

		if ( l_uiSelection == m_specComboShaders.m_pControl->GetCount() - 1 )
		{
			if ( EffectAdd() )
			{
				wxString l_strPage = wxT( "Shader " );
				l_strPage << m_specComboShaders.m_pControl->GetCount();
				m_specComboShaders.m_pControl->SetString( l_uiSelection, l_strPage );
				m_specComboShaders.m_pControl->Append( _( "New..." ) );
				m_specComboShaders.m_pControl->SetSelection( l_uiSelection );
			}
			else
			{
				l_uiSelection = m_specComboShaders.m_pControl->GetCount();
			}
		}
		else if ( l_uiSelection > 0 )
		{
			m_specButtonRemove.m_pControl->Show();
		}
		else
		{
			m_specButtonRemove.m_pControl->Hide();
		}

		if ( l_uiSelection < m_specComboShaders.m_pControl->GetCount() )
		{
			m_specButtonVertexFile.m_pControl->Show();
			m_specButtonFragmentFile.m_pControl->Show();
			m_pSelectedEffect = m_arrayEffects[l_uiSelection];
			m_initialised = true;
		}
		else
		{
			m_specButtonVertexFile.m_pControl->Hide();
			m_specButtonFragmentFile.m_pControl->Hide();
		}
	}

	void GpuProgramming::OnRemove( wxCommandEvent & WXUNUSED( p_event ) )
	{
		size_t l_uiToRemove = size_t( m_specComboShaders.m_pControl->GetSelection() );

		if ( l_uiToRemove > 0 && l_uiToRemove < m_specComboShaders.m_pControl->GetCount() - 1 )
		{
			if ( EffectRemove( l_uiToRemove ) )
			{
				m_specComboShaders.m_pControl->Delete( int( l_uiToRemove ) );
				m_specButtonVertexFile.m_pControl->Hide();
				m_specButtonFragmentFile.m_pControl->Hide();
				m_specButtonCompile.m_pControl->Hide();
				m_specButtonCompilerLog.m_pControl->Hide();
				m_specButtonRemove.m_pControl->Hide();
			}
		}

		m_pSelectedEffect = NULL;
	}

	void GpuProgramming::OnShaderCompile( wxCommandEvent & WXUNUSED( p_event ) )
	{
		if ( m_pSelectedEffect )
		{
			m_pSelectedEffect->Compile();
			m_specButtonCompilerLog.m_pControl->Show();
		}
	}

	void GpuProgramming::OnCompilerLog( wxCommandEvent & WXUNUSED( p_event ) )
	{
		if ( m_pSelectedEffect )
		{
			wxString l_strFragmentLog	= m_pSelectedEffect->GetCompilerLog( eSHADER_OBJECT_TYPE_PIXEL );
			wxString l_strLinkerLog		= m_pSelectedEffect->GetLinkerLog();
			wxString l_strText;
#if !defined( NDEBUG )
			wxString l_strVertexLog		= m_pSelectedEffect->GetCompilerLog( eSHADER_OBJECT_TYPE_VERTEX );

			if ( ! l_strVertexLog.empty() )
			{
				l_strText += _( "Vertex Shader :" ) + wxString( wxT( "\n" ) ) + l_strVertexLog;
			}

#endif

			if ( ! l_strFragmentLog.empty() )
			{
				if ( ! l_strText.empty() )
				{
					l_strText += wxT( "\n\n" );
				}

				l_strText += _( "Fragment Shader :" ) + wxString( wxT( "\n" ) ) + l_strFragmentLog + wxT( "\n\n" );
			}

			if ( ! l_strLinkerLog.empty() )
			{
				if ( ! l_strText.empty() )
				{
					l_strText += wxT( "\n\n" );
				}

				l_strText += _( "Linker :" ) + wxString( wxT( "\n" ) ) + l_strLinkerLog + wxT( "\n\n" );
			}

			if ( l_strText.empty() )
			{
				l_strText = _( "No error" );
			}

			ShowMessageBox( _( "Compile and Link result" ), l_strText, wxOK | wxCENTRE | wxICON_INFORMATION );
		}
	}

	void GpuProgramming::OnVertexShaderPath( wxCommandEvent & p_event )
	{
		wxString l_strFile = p_event.GetString();

		if ( m_pSelectedEffect && ! l_strFile.empty() )
		{
			wxString l_strFile = p_event.GetString();
			m_specButtonCompile.m_pControl->Show();
			m_specButtonCompilerLog.m_pControl->Hide();
			m_pSelectedEffect->SetVertexFile( l_strFile );
		}
	}

	void GpuProgramming::OnFragmentShaderPath( wxCommandEvent & p_event )
	{
		wxString l_strFile = p_event.GetString();

		if ( m_pSelectedEffect && ! l_strFile.empty() )
		{
			wxString l_strFile = p_event.GetString();
			m_specButtonCompile.m_pControl->Show();
			m_specButtonCompilerLog.m_pControl->Hide();
			m_pSelectedEffect->SetFragmentFile( l_strFile );
		}
	}

	void GpuProgramming::OnImagePath( wxCommandEvent & p_event )
	{
		wxString l_strImagePath = p_event.GetString();

		if ( ! l_strImagePath.empty() )
		{
			SetImagePath( l_strImagePath );
			m_specButtonReset.m_pControl->Show();
			m_specComboShaders.m_pControl->Show();
		}
	}
}
