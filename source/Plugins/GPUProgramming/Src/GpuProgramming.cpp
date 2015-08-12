#include "GpuProgramming.h"

namespace GpuProgramming
{
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

		m_buttonReset = std::make_shared< ButtonCtrl >( m_options, _( "Reset Time" ), eID_RESETTIME, Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, true );
		m_buttonReset->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnResetTime, this ) );
		m_staticShaders = std::make_shared< StaticCtrl >( m_options, _( "Shaders" ), Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER );
		m_comboShaders = std::make_shared< ComboBoxCtrl >( m_options, g_shaChoices, -1, eID_SHADERS, Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY, true );
		m_comboShaders->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnSelectShader, this, std::placeholders::_1 ) );
		m_buttonVertexFile = std::make_shared< ButtonCtrl >( m_options, _T( "Vertex" ), eID_VERTEXFILE, Position( 10, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonVertexFile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnVertexShaderPath, this ) );
		m_buttonFragmentFile = std::make_shared< ButtonCtrl >( m_options, _T( "Pixel" ), eID_FRAGMENTFILE, Position( 10, 10 + DEFAULT_HEIGHT * 4 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonFragmentFile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnFragmentShaderPath, this ) );
		m_buttonCompile = std::make_shared< ButtonCtrl >( m_options, _( "Compile" ), eID_COMPILE, Position( 10, 10 + DEFAULT_HEIGHT * 5 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonCompile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnShaderCompile, this ) );
		m_buttonCompilerLog = std::make_shared< ButtonCtrl >( m_options, _( "Compiler Log" ), eID_COMPILERLOG, Position( 10, 10 + DEFAULT_HEIGHT * 6 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonCompilerLog->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnCompilerLog, this ) );
		m_buttonRemove = std::make_shared< ButtonCtrl >( m_options, _( "Remove" ), eID_REMOVE, Position( 10, 10 + DEFAULT_HEIGHT * 7 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonRemove->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnRemove, this ) );

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
}
