#include "ShaderWebcam.h"

#include <cmath>

namespace ProceduralTextures
{
	ShaderWebcam::Thread :: Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
		,	m_pCapture( NULL )
		,	m_pBuffer( NULL )
	{
	}

	ShaderWebcam::Thread :: ~Thread()
	{
	}

	void ShaderWebcam::Thread :: Initialise( cv::VideoCapture * p_pCapture, PixelBuffer * p_pBuffer )
	{
		m_pCapture = p_pCapture;
		m_pBuffer = p_pBuffer;
	}

	void ShaderWebcam::Thread :: Step()
	{
		if ( m_pCapture->grab() )
		{
			wxSize l_sizeReturn;
			cv::Mat l_frame;
			m_pCapture->retrieve( l_frame );
			l_sizeReturn.x = l_frame.cols;
			l_sizeReturn.y = l_frame.rows;
			cv::cvtColor( l_frame, l_frame, cv::COLOR_BGR2RGB );
			m_pBuffer->set< PG_UINT8_RGB >( ( uint8_t * )l_frame.ptr(), NULL );
			m_pBuffer->flip();
		}
	}

	//*************************************************************************************************

	ShaderWebcam :: ShaderWebcam( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame, true, 10 )
		,	m_iSeparationOffset( 50 )
		,	m_eSeparationType( eSEPARATION_NONE )
		,	m_pSelectedEffect( NULL )
		,	m_pCapture( NULL )
		,	m_specStaticSeparator( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Separation" ) ) )
		,	m_specComboSeparator( SpecificControlParameters< eCONTROL_TYPE_COMBO >() )
		,	m_specSliderOffset( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 100, 50 ) )
		,	m_specButtonReset( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Reset Time" ) ) )
		,	m_specStaticShaders( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Shaders" ) ) )
		,	m_specComboShaders( SpecificControlParameters< eCONTROL_TYPE_COMBO >() )
		,	m_specButtonVertexFile( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Vertex" ) ) )
		,	m_specButtonFragmentFile( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Fragment" ) ) )
		,	m_specButtonCompile( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Compile" ) ) )
		,	m_specButtonCompilerLog( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Compiler log" ) ) )
		,	m_specButtonRemove( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Remove" ) ) )
		,	m_frameBuffer1( &m_openGl )
		,	m_frameBuffer2( &m_openGl )
		,	m_texture1( &m_openGl )
		,	m_texture2( &m_openGl )
	{
		m_specButtonVertexFile.m_strWildcard = _( "Vertex shader GLSL files (*.glsl;*.vert)|*.glsl;*.vert" );
		m_specButtonFragmentFile.m_strWildcard = _( "Fragment shader GLSL files (*.glsl;*.frag)|*.glsl;*.frag" );
		wxString l_sepChoices[] =
		{
			_( "Not separated"	)
			,	_( "Horizontally"	)
			,	_( "Vertically"	)
		};
		wxString l_shaChoices[] =
		{
			_( "Shader 1"	)
			,	_( "New..."	)
		};
		m_specComboSeparator.Initialise( l_sepChoices[0], l_sepChoices );
		m_specComboShaders.Initialise( wxT( "" ), l_shaChoices );
		m_pCapture = new cv::VideoCapture( 0 );
		int l_iWidth = p_width;
		int l_iHeight = p_height;

		if ( m_pCapture->isOpened() )
		{
			int l_iFps = int( m_pCapture->get( cv::CAP_PROP_FPS ) );

			if ( l_iFps > 0 )
			{
				m_sleepTime = int( 1000.0 / l_iFps );
			}
			else
			{
				m_sleepTime = 10;
			}

			m_pCapture->set( cv::CAP_PROP_FRAME_WIDTH, l_iWidth );
			m_pCapture->set( cv::CAP_PROP_FRAME_HEIGHT, l_iHeight );
			l_iWidth = int( m_pCapture->get( cv::CAP_PROP_FRAME_WIDTH ) );
			l_iHeight = int( m_pCapture->get( cv::CAP_PROP_FRAME_HEIGHT ) );
			Resize( l_iWidth, l_iHeight );
		}
		else
		{
			ShowMessageBox( _( "Capture device" ), _( "No capture device available" ), wxOK | wxCENTRE | wxICON_INFORMATION );
		}

		m_frameBuffer1.AttachTexture( GL_COLOR_ATTACHMENT0, & m_texture1 );
		m_frameBuffer2.AttachTexture( GL_COLOR_ATTACHMENT0, & m_texture2 );
		DoGeneratePanel();

		if ( m_pCapture )
		{
			EffectAdd();
			ShaderWebcam::Thread * l_pThread = CreateThread< ShaderWebcam::Thread >( l_iWidth, 0, l_iHeight, l_iHeight, m_pxColour );
			l_pThread->Initialise( m_pCapture, &m_backBuffer );
			l_pThread->Run();
			m_initialised = true;
		}
	}

	ShaderWebcam :: ~ShaderWebcam()
	{
		m_initialised = false;
		DoCleanup();

		for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
		{
			delete m_arrayEffects[i];
		}

		m_arrayEffects.clear();

		if ( m_pCapture )
		{
			if ( m_pCapture->isOpened() )
			{
				m_pCapture->release();
			}

			delete m_pCapture;
		}
	}

	void ShaderWebcam :: ResetTime()
	{
		for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
		{
			m_arrayEffects[i]->ResetTime();
		}
	}

	bool ShaderWebcam :: EffectAdd()
	{
		Effect * l_pEffect = new Effect( &m_openGl, &m_image );
		m_arrayEffects.push_back( l_pEffect );
		return true;
	}

	bool ShaderWebcam :: EffectRemove( size_t p_uiIndex )
	{
		bool l_bReturn = p_uiIndex < m_arrayEffects.size();

		if ( l_bReturn )
		{
			Effect * l_pEffect = m_arrayEffects[p_uiIndex];
			std::vector <Effect *>::iterator l_it = m_arrayEffects.begin();
			m_arrayEffects.erase( l_it + p_uiIndex );
			delete l_pEffect;
		}

		return l_bReturn;
	}

	void ShaderWebcam :: EffectSetVertexFile( size_t p_uiIndex, const wxString & p_strPath )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->SetVertexFile( p_strPath );
		}
	}

	void ShaderWebcam :: EffectSetFragmentFile( size_t p_uiIndex, const wxString & p_strPath )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->SetFragmentFile( p_strPath );
		}
	}

	void ShaderWebcam :: EffectSetImagePath( size_t p_uiIndex, size_t p_iImage, const wxString & p_strImagePath )
	{
		m_initialised = false;

		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->SetImagePath( p_iImage, p_strImagePath );
		}

		m_initialised = true;
	}

	void ShaderWebcam :: EffectCompile( size_t p_uiIndex )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->Compile();
		}
	}

	wxString ShaderWebcam :: EffectGetCompilerLog( size_t p_uiIndex, eSHADER_OBJECT_TYPE p_eType )
	{
		wxString l_strReturn;

		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->GetCompilerLog( p_eType );
		}

		return l_strReturn;
	}

	wxString ShaderWebcam :: EffectGetLinkerLog( size_t p_uiIndex )
	{
		wxString l_strReturn;

		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->GetLinkerLog();
		}

		return l_strReturn;
	}

	bool ShaderWebcam :: EffectActivate( size_t p_uiIndex )
	{
		bool l_bReturn = p_uiIndex < m_arrayEffects.size();

		if ( l_bReturn )
		{
			l_bReturn = m_arrayEffects[p_uiIndex]->Activate( m_eSeparationType, m_iSeparationOffset );
		}

		return l_bReturn;
	}

	void ShaderWebcam :: EffectDeactivate( size_t p_uiIndex )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->Deactivate();
		}
	}

	void ShaderWebcam :: DoGlInitialise()
	{
		m_texture1.Create();
		m_texture2.Create();
		m_frameBuffer1.Create();
		m_frameBuffer2.Create();
	}

	void ShaderWebcam :: DoGlPreRender()
	{
		m_texture1.Initialise( &m_backBuffer );
		m_texture2.Initialise( m_sizeImage );
		m_frameBuffer1.Initialise( m_sizeImage );
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

	void ShaderWebcam :: DoGlRender( bool & p_bChanged )
	{
		GlFrameBuffer * l_pFrameBufferTexture = & m_frameBuffer1;
		GlFrameBuffer * l_pFrameBufferRender = & m_frameBuffer2;
		m_texture1.Activate();
		m_texture1.UploadAsync();
		m_texture1.Deactivate();

		for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
		{
			if ( l_pFrameBufferRender->Activate() )
			{
				if ( EffectActivate( i ) )
				{
					m_openGl.Clear( GL_COLOR_BUFFER_BIT );
					l_pFrameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Activate();
					DoSubRender();
					l_pFrameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Deactivate();
					EffectDeactivate( i );
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

	void ShaderWebcam :: DoGlPostRender()
	{
	}

	void ShaderWebcam :: DoGlCleanup()
	{
		for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
		{
			m_arrayEffects[i]->Cleanup();
			delete m_arrayEffects[i];
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

	void ShaderWebcam :: DoResize( const wxSize & p_size )
	{
		m_frameBuffer1.Resize( m_sizeImage );
		m_frameBuffer2.Resize( m_sizeImage );
		m_texture1.Resize( m_sizeImage );
		m_texture2.Resize( m_sizeImage );
		m_backBuffer.init( Size( m_sizeImage.x, m_sizeImage.y ) );
	}

	void ShaderWebcam :: DoGeneratePanel()
	{
		int iWidth = 150;
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 0	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specStaticSeparator,		0,					eEVENT_TYPE_NONE,				NULL,															true	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_SEPTYPE,		wxPoint( 10, wxDEFAULT_HEIGHT * 1	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specComboSeparator,		wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED,	wxCommandEventHandler(	ShaderWebcam::OnSepType	),	true	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,		eID_SEPOFFSET,		wxPoint( 10, wxDEFAULT_HEIGHT * 2	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specSliderOffset,			wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	ShaderWebcam::OnSepOffset	),	true	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_RESETTIME,		wxPoint( 10, wxDEFAULT_HEIGHT * 3	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specButtonReset,			0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	ShaderWebcam::OnResetTime	),	true	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 4	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specStaticShaders,		0,					eEVENT_TYPE_NONE,				NULL,															true	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_SHADERS,		wxPoint( 10, wxDEFAULT_HEIGHT * 5	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specComboShaders,			wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED,	wxCommandEventHandler(	ShaderWebcam::OnSelectShader	),	true	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_VERTEXFILE,		wxPoint( 10, wxDEFAULT_HEIGHT * 6	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specButtonVertexFile,		0,					eEVENT_TYPE_LOAD_TEXT_FILEA,	wxCommandEventHandler(	ShaderWebcam::OnVertexShaderPath	),	false	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_FRAGMENTFILE,	wxPoint( 10, wxDEFAULT_HEIGHT * 7	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specButtonFragmentFile,	0,					eEVENT_TYPE_LOAD_TEXT_FILEB,	wxCommandEventHandler(	ShaderWebcam::OnFragmentShaderPath	),	false	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_COMPILE,		wxPoint( 10, wxDEFAULT_HEIGHT * 8	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specButtonCompile,		0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	ShaderWebcam::OnShaderCompile	),	false	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_COMPILERLOG,	wxPoint( 10, wxDEFAULT_HEIGHT * 9	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specButtonCompilerLog,	0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	ShaderWebcam::OnCompilerLog	),	false	) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_REMOVE,			wxPoint( 10, wxDEFAULT_HEIGHT * 10	), wxSize( iWidth -  20, wxDEFAULT_HEIGHT	), &m_specButtonRemove,			0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	ShaderWebcam::OnRemove	),	false	) );
	}

	void ShaderWebcam :: OnSepType( wxCommandEvent	& p_event )
	{
		SetSeparationType( eSEPARATION( p_event.GetInt() ) );

		if ( p_event.GetInt() != 0 )
		{
			m_specSliderOffset.m_pControl->Show();
		}
		else
		{
			m_specSliderOffset.m_pControl->Hide();
		}
	}

	void ShaderWebcam :: OnSepOffset( wxCommandEvent & p_event )
	{
		SetSeparationOffset( p_event.GetInt() );
	}

	void ShaderWebcam :: OnResetTime( wxCommandEvent & WXUNUSED( p_event ) )
	{
		ResetTime();
	}

	void ShaderWebcam :: OnSelectShader( wxCommandEvent & p_event )
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
		}
		else
		{
			m_specButtonVertexFile.m_pControl->Hide();
			m_specButtonFragmentFile.m_pControl->Hide();
		}
	}

	void ShaderWebcam :: OnRemove( wxCommandEvent & WXUNUSED( p_event ) )
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

	void ShaderWebcam :: OnShaderCompile( wxCommandEvent & WXUNUSED( p_event ) )
	{
		if ( m_pSelectedEffect )
		{
			m_pSelectedEffect->Compile();
			m_specButtonCompile.m_pControl->Hide();
			m_specButtonCompilerLog.m_pControl->Show();
		}
	}

	void ShaderWebcam :: OnCompilerLog( wxCommandEvent & WXUNUSED( p_event ) )
	{
		if ( m_pSelectedEffect )
		{
			wxString l_strVertexLog		= m_pSelectedEffect->GetCompilerLog( eSHADER_OBJECT_TYPE_VERTEX	);
			wxString l_strFragmentLog	= m_pSelectedEffect->GetCompilerLog( eSHADER_OBJECT_TYPE_PIXEL	);
			wxString l_strLinkerLog		= m_pSelectedEffect->GetLinkerLog();
			wxString l_strText;

			if ( ! l_strVertexLog.empty() )
			{
				l_strText += _( "Vertex Shader :" ) + wxString( wxT( "\n" ) ) + l_strVertexLog;
			}

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

	void ShaderWebcam :: OnVertexShaderPath( wxCommandEvent & p_event )
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

	void ShaderWebcam :: OnFragmentShaderPath( wxCommandEvent & p_event )
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
}
