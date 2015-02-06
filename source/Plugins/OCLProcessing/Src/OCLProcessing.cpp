#include "OCLProcessing.h"

#include <Random.h>

namespace ProceduralTextures
{
	OCLProcessing::OCLProcessing( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame, true, 20 )
		,	m_iSeparationOffset( 50 )
		,	m_eSeparationType( eSEPARATION_NONE )
		,	m_pSelectedEffect( NULL )
		,	m_specButtonImage( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Image" ) ) )
		,	m_specStaticSeparator( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Separation" ) ) )
		,	m_specComboSeparator( SpecificControlParameters< eCONTROL_TYPE_COMBO >() )
		,	m_specSliderOffset( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 100, 50 ) )
		,	m_specButtonReset( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Reset Time" ) ) )
		,	m_specStaticPrograms( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Programs" ) ) )
		,	m_specComboPrograms( SpecificControlParameters< eCONTROL_TYPE_COMBO >() )
		,	m_specButtonFile( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Program" ) ) )
		,	m_specStaticKernel( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Kernels" ) ) )
		,	m_specComboKernels( SpecificControlParameters< eCONTROL_TYPE_COMBO >() )
		,	m_specButtonCompilerLog( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Compiler log" ) ) )
		,	m_specButtonRemove( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Remove" ) ) )
		,	m_outputTexture( &m_openGl )
		,	m_inputBuffer( Size( p_width, p_height ) )
		,	m_outputBuffer( Size( p_width, p_height ) )
	{
		m_specButtonFile.m_strWildcard = _( "OpenCL files (*.cl)|*.cl" );
		wxString l_sepChoices[] =
		{
			_( "Not separated" )
			,	_( "Horizontally" )
			,	_( "Vertically" )
		};
		wxString l_progChoices[] =
		{
			_( "Program 1" )
			,	_( "New..." )
		};
		m_specComboSeparator.Initialise( l_sepChoices[0], l_sepChoices );
		m_specComboPrograms.Initialise( wxT( "" ), l_progChoices );
		m_outputTexture.Resize( m_sizeImage );
		wxSystemOptions::SetOption( wxT( "msw.remap" ), 0 );
		wxInitAllImageHandlers();
		std::vector< cl::Device > l_arrayDevices;
		std::vector< cl::Platform > l_arrayPlatforms;
		std::string l_prog;
		cl_int l_iErr;
		cl::Platform::get( &l_arrayPlatforms );
		bool l_bContinue = ocl::CheckErr( l_arrayPlatforms.size() != 0 ? CL_SUCCESS : -1, "cl::Platform::get" );

		if ( l_bContinue )
		{
			TRACE( wxT( "Platform count is: %d\n" ), uint32_t( l_arrayPlatforms.size() ) );
			std::string l_strInfo;
			m_clPlatform = l_arrayPlatforms[0];
			m_clPlatform.getInfo( ( cl_platform_info )CL_PLATFORM_NAME, &l_strInfo );
			TRACE( "  Name    : %s\n", l_strInfo.c_str() );
			m_clPlatform.getInfo( ( cl_platform_info )CL_PLATFORM_VENDOR, &l_strInfo );
			TRACE( "  Vendor  : %s\n", l_strInfo.c_str() );
			m_clPlatform.getInfo( ( cl_platform_info )CL_PLATFORM_VERSION, &l_strInfo );
			TRACE( "  Version : %s\n", l_strInfo.c_str() );
			cl_context_properties l_props[3] = { CL_CONTEXT_PLATFORM, ( cl_context_properties )( m_clPlatform )(), 0 };
			m_clContext = cl::Context( CL_DEVICE_TYPE_GPU, l_props, NULL, NULL, &l_iErr );
			l_bContinue = ocl::CheckErr( l_iErr, "Context::Context()" );
		}

		if ( l_bContinue )
		{
			l_arrayDevices = m_clContext.getInfo< CL_CONTEXT_DEVICES >();
			l_bContinue = ocl::CheckErr( l_arrayDevices.size() > 0 ? CL_SUCCESS : -1, "l_arrayDevices.size() > 0" );
		}

		if ( l_bContinue )
		{
			m_clDevice = l_arrayDevices[0];
			m_clQueue = cl::CommandQueue( m_clContext, m_clDevice, 0, &l_iErr );
			l_bContinue = ocl::CheckErr( l_iErr, "CommandQueue::CommandQueue()" );
		}

		DoGeneratePanel();
	}

	OCLProcessing::~OCLProcessing()
	{
		for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
		{
			delete m_arrayEffects[i];
		}

		m_arrayEffects.clear();
		wxImage::CleanUpHandlers();
		m_initialised = false;
		DoCleanup();
	}

	void OCLProcessing::SetImagePath( const wxString & p_strImagePath )
	{
		m_initialised = false;
		wxSize l_size = DoLoadImage( p_strImagePath, m_image, m_inputBuffer );
		Resize( l_size );

		if ( m_arrayEffects.size() == 0 )
		{
			EffectAdd();
		}
		else
		{
			for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
			{
				m_arrayEffects[i]->UpdateImages();
			}
		}

		m_initialised = true;
	}

	void OCLProcessing::ResetTime()
	{
		for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
		{
			m_arrayEffects[i]->ResetTime();
		}
	}

	bool OCLProcessing::EffectAdd()
	{
		Effect * l_pEffect = new Effect( m_clContext, m_clDevice, m_clQueue, &m_image, &m_outputBuffer );
		m_arrayEffects.push_back( l_pEffect );
		return true;
	}

	bool OCLProcessing::EffectRemove( size_t p_uiIndex )
	{
		bool l_bReturn = p_uiIndex < m_arrayEffects.size();

		if ( l_bReturn )
		{
			Effect * l_pEffect = m_arrayEffects[p_uiIndex];
			std::vector< Effect * >::iterator l_it = m_arrayEffects.begin();
			m_arrayEffects.erase( l_it + p_uiIndex );
			delete l_pEffect;
		}

		return l_bReturn;
	}

	void OCLProcessing::EffectLoadFile( size_t p_uiIndex, const wxString & p_strPath )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->LoadFile( p_strPath );
		}
	}

	void OCLProcessing::EffectLoadKernel( size_t p_uiIndex, const wxString & p_strName )
	{
		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->LoadKernel( p_strName );
		}
	}

	void OCLProcessing::EffectSetImagePath( size_t p_uiIndex, size_t p_iImage, const wxString & p_strImagePath )
	{
		m_initialised = false;

		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->SetImagePath( p_iImage, p_strImagePath );
		}

		m_initialised = true;
	}

	wxString OCLProcessing::EffectGetCompilerLog( size_t p_uiIndex )
	{
		wxString l_strReturn;

		if ( p_uiIndex < m_arrayEffects.size() )
		{
			m_arrayEffects[p_uiIndex]->GetCompilerLog();
		}

		return l_strReturn;
	}

	bool OCLProcessing::EffectApply( size_t p_uiIndex )
	{
		bool l_bReturn = p_uiIndex < m_arrayEffects.size();

		if ( l_bReturn )
		{
			l_bReturn = m_arrayEffects[p_uiIndex]->Apply( m_eSeparationType, m_iSeparationOffset );
		}

		return l_bReturn;
	}

	void OCLProcessing::DoGlInitialise()
	{
		m_outputTexture.Initialise();
	}

	void OCLProcessing::DoGlPreRender()
	{
		std::memcpy( m_outputBuffer.ptr(), m_inputBuffer.const_ptr(), m_inputBuffer.size() );
		m_outputTexture.Initialise( &m_outputBuffer );

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

	void OCLProcessing::DoGlRender( bool & p_bChanged )
	{
		for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
		{
			EffectApply( i );
		}

		m_frameBuffer.Activate();
		m_openGl.Clear( GL_COLOR_BUFFER_BIT );
		m_outputTexture.Activate();
		m_outputTexture.UploadGlImage();
		DoSubRender();
		m_outputTexture.Deactivate();
		m_frameBuffer.Deactivate();
		p_bChanged = false;
	}

	void OCLProcessing::DoGlPostRender()
	{
	}

	void OCLProcessing::DoGlCleanup()
	{
		for ( size_t i = 0 ; i < m_arrayEffects.size() ; i++ )
		{
			m_arrayEffects[i]->Cleanup();
			delete m_arrayEffects[i];
		}

		m_arrayEffects.clear();
		m_outputTexture.Cleanup();
	}

	void OCLProcessing::DoResize( const wxSize & WXUNUSED( p_size ) )
	{
		m_outputTexture.Resize( m_sizeImage );
		m_outputBuffer.init( Size( m_sizeImage.x, m_sizeImage.y ) );
		m_backBuffer.init( Size( m_sizeImage.x, m_sizeImage.y ) );
	}

	void OCLProcessing::DoGeneratePanel()
	{
		int iWidth = 150;
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_IMAGEPATH,		wxPoint( 10, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonImage,			0,					eEVENT_TYPE_LOAD_IMAGEA,		wxCommandEventHandler(	OCLProcessing::OnImagePath ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 1 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticSeparator,		0,					eEVENT_TYPE_NONE,				NULL,														false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_SEPTYPE,		wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboSeparator,		wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED,	wxCommandEventHandler(	OCLProcessing::OnSepType ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,		eID_SEPOFFSET,		wxPoint( 10, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specSliderOffset,			wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	OCLProcessing::OnSepOffset ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_RESETTIME,		wxPoint( 10, wxDEFAULT_HEIGHT * 4 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonReset,			0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	OCLProcessing::OnResetTime ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 5 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticPrograms,		0,					eEVENT_TYPE_NONE,				NULL,														false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_PROGRAMS,		wxPoint( 10, wxDEFAULT_HEIGHT * 6 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboPrograms,		wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED,	wxCommandEventHandler(	OCLProcessing::OnSelectProgram ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_FILE,			wxPoint( 10, wxDEFAULT_HEIGHT * 7 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonFile,			0,					eEVENT_TYPE_LOAD_TEXT_FILEA,	wxCommandEventHandler(	OCLProcessing::OnFilePath ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 8 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticKernel,			0,					eEVENT_TYPE_NONE,				NULL,														false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_KERNEL,			wxPoint( 10, wxDEFAULT_HEIGHT * 9 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboKernels,			wxCB_READONLY,		eEVENT_TYPE_TEXT_UPDATED,		wxCommandEventHandler(	OCLProcessing::OnSelectKernel ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_COMPILERLOG,	wxPoint( 10, wxDEFAULT_HEIGHT * 10 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonCompilerLog,	0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	OCLProcessing::OnCompilerLog ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_REMOVE,			wxPoint( 10, wxDEFAULT_HEIGHT * 11 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonRemove,			0,					eEVENT_TYPE_BUTTON_CLIKED,		wxCommandEventHandler(	OCLProcessing::OnRemove ),	false ) );
	}

	void OCLProcessing::OnSepType( wxCommandEvent	& p_event )
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

	void OCLProcessing::OnSepOffset( wxCommandEvent & p_event )
	{
		SetSeparationOffset( p_event.GetInt() );
	}

	void OCLProcessing::OnResetTime( wxCommandEvent & WXUNUSED( p_event ) )
	{
		ResetTime();
	}

	void OCLProcessing::OnSelectProgram( wxCommandEvent & p_event )
	{
		unsigned int l_uiSelection = ( unsigned int )p_event.GetSelection();

		if ( l_uiSelection == m_specComboPrograms.m_pControl->GetCount() - 1 )
		{
			if ( EffectAdd() )
			{
				wxString l_strPage = wxT( "Program " );
				l_strPage << m_specComboPrograms.m_pControl->GetCount();
				m_specComboPrograms.m_pControl->SetString( l_uiSelection, l_strPage );
				m_specComboPrograms.m_pControl->Append( _( "New..." ) );
				m_specComboPrograms.m_pControl->SetSelection( l_uiSelection );
			}
			else
			{
				l_uiSelection = m_specComboPrograms.m_pControl->GetCount();
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

		if ( l_uiSelection < m_specComboPrograms.m_pControl->GetCount() )
		{
			m_specButtonFile.m_pControl->Show();
			m_specComboKernels.m_pControl->Show();
			m_pSelectedEffect = m_arrayEffects[l_uiSelection];
		}
		else
		{
			m_specButtonFile.m_pControl->Hide();
			m_specComboKernels.m_pControl->Hide();
		}
	}

	void OCLProcessing::OnRemove( wxCommandEvent & WXUNUSED( p_event ) )
	{
		size_t l_uiToRemove = size_t( m_specComboPrograms.m_pControl->GetSelection() );

		if ( l_uiToRemove > 0 && l_uiToRemove < m_specComboPrograms.m_pControl->GetCount() - 1 )
		{
			if ( EffectRemove( l_uiToRemove ) )
			{
				m_specComboPrograms.m_pControl->Delete( int( l_uiToRemove ) );
				m_specButtonFile.m_pControl->Hide();
				m_specComboKernels.m_pControl->Hide();
				m_specButtonCompilerLog.m_pControl->Hide();
				m_specButtonRemove.m_pControl->Hide();
			}
		}

		m_pSelectedEffect = NULL;
	}

	void OCLProcessing::OnCompilerLog( wxCommandEvent & WXUNUSED( p_event ) )
	{
		if ( m_pSelectedEffect )
		{
			wxString l_strLog = m_pSelectedEffect->GetCompilerLog();
			wxString l_strText;

			if ( !l_strLog.empty() )
			{
				l_strText += _( "Program :" ) + wxString( wxT( "\n" ) ) + l_strLog;
			}

			if ( l_strText.empty() )
			{
				l_strText = _( "No error" );
			}

			ShowMessageBox( _( "Compile and Link result" ), l_strText, wxOK | wxCENTRE | wxICON_INFORMATION );
		}
	}

	void OCLProcessing::OnFilePath( wxCommandEvent & p_event )
	{
		wxString l_strFile = p_event.GetString();

		if ( m_pSelectedEffect && !l_strFile.empty() )
		{
			wxString l_strFile = p_event.GetString();

			if ( m_pSelectedEffect->LoadFile( l_strFile ) )
			{
				wxArrayString l_arrayChoices;
				m_specComboKernels.m_pControl->Clear();

				for ( size_t i = 0; i < m_pSelectedEffect->GetKernelCount(); ++i )
				{
					m_specComboKernels.m_pControl->Append( wxString( m_pSelectedEffect->GetKernelName( i ).c_str(), wxMBConvLibc() ) );
				}

				m_specComboKernels.m_pControl->Show();
			}
			else
			{
				m_specComboKernels.m_pControl->Hide();
			}

			m_specButtonCompilerLog.m_pControl->Show();
		}
	}

	void OCLProcessing::OnSelectKernel( wxCommandEvent & p_event )
	{
		unsigned int l_uiSelection = ( unsigned int )m_specComboKernels.m_pControl->GetCurrentSelection();

		if ( l_uiSelection < m_specComboKernels.m_pControl->GetCount() && l_uiSelection < m_pSelectedEffect->GetKernelCount() )
		{
			m_pSelectedEffect->LoadKernel( m_specComboKernels.m_pControl->GetString( l_uiSelection ) );
		}
	}

	void OCLProcessing::OnImagePath( wxCommandEvent & p_event )
	{
		wxString l_strImagePath = p_event.GetString();

		if ( ! l_strImagePath.empty() )
		{
			SetImagePath( l_strImagePath );
			m_specButtonReset.m_pControl->Show();
			m_specComboPrograms.m_pControl->Show();
			m_specStaticSeparator.m_pControl->Show();
			m_specComboSeparator.m_pControl->Show();
		}
	}
}
