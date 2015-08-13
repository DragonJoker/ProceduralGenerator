#include "GpuProgramming.h"

namespace GpuProgramming
{
	namespace
	{
		/*!
		@author
			Sylvain DOREMUS
		@date
			23/05/2012
		@brief
			The controls IDs
		*/
		typedef enum
		{
			eID_ANY = -1,
			eID_SEPTYPE = 52,
			eID_RESETTIME = 53,
			eID_SHADERS = 54,
			eID_REMOVE = 55,
			eID_VERTEXFILE = 56,
			eID_FRAGMENTFILE = 57,
			eID_COMPILE = 58,
			eID_COMPILERLOG = 59,
			eID_HELP_OPEN = 62,
		}	eID;
	}

	Generator::Generator()
		: ProceduralTextures::Generator< CpuStep, GpuStep >( true, ProceduralTextures::DEFAULT_FRAME_TIME )
	{
	}

	Generator::~Generator()
	{
	}

	void Generator::DoCreate( ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize )
	{
		m_cpuStep = std::make_shared< CpuStep >( shared_from_this(), p_size );
		m_gpuStep = std::make_shared< GpuStep >( shared_from_this(), p_size, p_bordersSize );
	}

	void Generator::DoDestroy()
	{
		m_cpuStep.reset();
		m_gpuStep.reset();
	}

	void Generator::DoGeneratePanel()
	{
		using namespace ProceduralTextures;

		String g_shaChoices[] =
		{
			_T( "New..." )
		};
		StringStream l_helpText;
		l_helpText << _( "Vertex attributes filled" ) << std::endl;
		l_helpText << _T( " - vertex: vec2\n    " ) << _( "Vertex position" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - texture: vec2\n    " ) << _( "Vertex texture coordinates" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << std::endl;
		l_helpText << _( "Additional uniform variables" ) << std::endl;
		l_helpText << _T( " - pg_width: int\n    " ) << _( "Window width" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - pg_height: int\n    " ) << _( "Window height" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - pg_time: int\n    " ) << _( "Elapsed time, in ms" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - pg_mouse: ivec2\n    " ) << _( "The mouse position" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - pg_texture: sampler2D\n    " ) << _( "Selected texture" ) << std::endl;

		uint32_t l_index = 0;
		int l_helpPanelWidth = 200;
		int l_helpPanelHeight = 300;

		m_buttonHelpOpen = std::make_shared< ButtonCtrl >( m_options, _( "PGEN GLSL Help" ), eID_HELP_OPEN, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), 0, true );
		m_buttonHelpOpen->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnHelpOpen, this ) );
		m_panelHelp = std::make_shared< StaticCtrl >( nullptr, String(), Position( CONFIG_PANEL_WIDTH, 40 ), Size( l_helpPanelWidth, l_helpPanelHeight ), 0, false );
		m_panelHelp->ConnectNC( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, std::bind( &Generator::OnHelpPanelClick, this, std::placeholders::_1, std::placeholders::_2 ) );
		m_panelHelp->SetCatchesMouseEvents( true );
		m_panelHelp->SetBackgroundColour( Colour( 0.0, 0.0, 0.0, 0.5 ) );
		m_panelHelp->SetForegroundColour( Colour( 1.0, 1.0, 1.0, 1.0 ) );
		m_panelHelp->SetBackgroundBorders( Point4i( 1, 1, 1, 1 ) );
		m_panelHelpText = std::make_shared< StaticCtrl >( m_panelHelp, l_helpText.str(), Position( 5, 5 ), Size( l_helpPanelWidth - 10, l_helpPanelHeight - 10 ), eSTATIC_STYLE_VALIGN_TOP | eSTATIC_STYLE_HALIGN_LEFT );
		m_panelHelpText->ConnectNC( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, std::bind( &Generator::OnHelpPanelClick, this, std::placeholders::_1, std::placeholders::_2 ) );
		m_panelHelpText->SetCatchesMouseEvents( true );

		m_buttonReset = std::make_shared< ButtonCtrl >( m_options, _( "Reset Time" ), eID_RESETTIME, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, true );
		m_buttonReset->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnResetTime, this ) );
		m_staticShaders = std::make_shared< StaticCtrl >( m_options, _( "Shaders" ), Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER );
		m_comboShaders = std::make_shared< ComboBoxCtrl >( m_options, g_shaChoices, -1, eID_SHADERS, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY, true );
		m_comboShaders->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnSelectShader, this, std::placeholders::_1 ) );
		m_buttonVertexFile = std::make_shared< ButtonCtrl >( m_options, _T( "Vertex" ), eID_VERTEXFILE, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonVertexFile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnVertexShaderPath, this ) );
		m_buttonFragmentFile = std::make_shared< ButtonCtrl >( m_options, _T( "Pixel" ), eID_FRAGMENTFILE, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonFragmentFile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnFragmentShaderPath, this ) );
		m_buttonCompile = std::make_shared< ButtonCtrl >( m_options, _( "Compile" ), eID_COMPILE, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonCompile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnShaderCompile, this ) );
		m_buttonCompilerLog = std::make_shared< ButtonCtrl >( m_options, _( "Compiler Log" ), eID_COMPILERLOG, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonCompilerLog->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnCompilerLog, this ) );
		m_buttonRemove = std::make_shared< ButtonCtrl >( m_options, _( "Remove" ), eID_REMOVE, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonRemove->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnRemove, this ) );

		m_arrayControls.push_back( m_buttonHelpOpen );
		m_arrayControls.push_back( m_panelHelp );
		m_arrayControls.push_back( m_panelHelpText );
		m_arrayControls.push_back( m_buttonReset );
		m_arrayControls.push_back( m_staticShaders );
		m_arrayControls.push_back( m_comboShaders );
		m_arrayControls.push_back( m_buttonVertexFile );
		m_arrayControls.push_back( m_buttonFragmentFile );
		m_arrayControls.push_back( m_buttonCompile );
		m_arrayControls.push_back( m_buttonCompilerLog );
		m_arrayControls.push_back( m_buttonRemove );
	}

	void Generator::OnResetTime()
	{
		m_gpuStep->ResetTime();
	}

	void Generator::OnSelectShader( uint32_t p_index )
	{
		if ( p_index == m_comboShaders->GetItemCount() - 1 )
		{
			m_gpuStep->AddEffect();
			ProceduralTextures::StringStream l_strPage;
			l_strPage << _T( "Shader " );
			l_strPage << m_comboShaders->GetItemCount();
			m_comboShaders->SetItemText( p_index, l_strPage.str() );
			m_comboShaders->AppendItem( _( "New..." ) );
			m_comboShaders->SetSelected( p_index );
		}
		else if ( p_index >= 0 )
		{
			m_comboShaders->SetSelected( p_index );
			m_buttonRemove->Show();
		}
		else
		{
			m_comboShaders->SetSelected( -1 );
			m_buttonRemove->Hide();
		}

		if ( p_index < m_comboShaders->GetItemCount() )
		{
			m_buttonVertexFile->Show();
			m_buttonFragmentFile->Show();
			m_gpuStep->SelectEffect( p_index );
		}
		else
		{
			m_buttonVertexFile->Hide();
			m_buttonFragmentFile->Hide();
		}
	}

	void Generator::OnRemove()
	{
		uint32_t l_uiToRemove = uint32_t( m_comboShaders->GetSelected() );

		if ( l_uiToRemove > 0 && l_uiToRemove < m_comboShaders->GetItemCount() - 1 )
		{
			m_gpuStep->RemoveEffect( l_uiToRemove );
			m_comboShaders->RemoveItem( int( l_uiToRemove ) );
			m_buttonVertexFile->Hide();
			m_buttonFragmentFile->Hide();
			m_buttonCompile->Hide();
			m_buttonCompilerLog->Hide();
			m_buttonRemove->Hide();
		}
	}

	void Generator::OnShaderCompile()
	{
		if ( m_gpuStep->CompileEffect() )
		{
			m_buttonCompilerLog->Show();
		}
	}

	void Generator::OnCompilerLog()
	{
		ShowMessageBox( _( "Compile and Link result" ), m_gpuStep->GetCompilerLog() );
	}

	void Generator::OnVertexShaderPath()
	{
		ProceduralTextures::String l_path;

		if ( OpenFile( l_path, _( "Choose a vertex shader file" ), _( "Vertex shader GLSL files (*.glsl;*.vert)|*.glsl;*.vert" ) ) )
		{
			size_t l_index = size_t( m_comboShaders->GetSelected() );

			if ( l_index < m_comboShaders->GetItemCount() - 1 && !l_path.empty() )
			{
				m_buttonCompile->Show();
				m_buttonCompilerLog->Hide();
				m_gpuStep->SetVertexShaderPath( l_path );
			}
		}
	}

	void Generator::OnFragmentShaderPath()
	{
		ProceduralTextures::String l_path;

		if ( OpenFile( l_path, _( "Choose a fragment shader file" ), _( "Fragment shader GLSL files (*.glsl;*.frag)|*.glsl;*.frag" ) ) )
		{
			size_t l_index = size_t( m_comboShaders->GetSelected() );

			if ( l_index < m_comboShaders->GetItemCount() - 1 && !l_path.empty() )
			{
				m_buttonCompile->Show();
				m_buttonCompilerLog->Hide();
				m_gpuStep->SetFragmentShaderPath( l_path );
			}
		}
	}

	void Generator::OnHelpOpen()
	{
		m_panelHelp->SetVisible( !m_panelHelp->IsVisible() );
	}

	void Generator::OnHelpClose()
	{
		m_panelHelp->Hide();
	}

	void Generator::OnHelpPanelClick( std::shared_ptr< ProceduralTextures::Control > p_static, ProceduralTextures::MouseEvent const & p_event )
	{
		if ( p_event.GetMouseEventType() == ProceduralTextures::eMOUSE_EVENT_MOUSE_BUTTON_RELEASED && ( p_static == m_panelHelpText || p_static == m_panelHelp ) )
		{
			m_panelHelp->Hide();
		}
	}
}
