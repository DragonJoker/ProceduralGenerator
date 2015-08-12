#include "ShaderWebcam.h"

namespace ShaderWebcam
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
		m_capture = std::make_shared< cv::VideoCapture >( 0 );

		if ( m_capture->isOpened() )
		{
			int l_iFps = int( m_capture->get( cv::CAP_PROP_FPS ) );

			if ( l_iFps > 0 )
			{
				m_frameTime = std::chrono::milliseconds( int( 1000.0 / l_iFps ) );
			}
			else
			{
				m_capture->set( cv::CAP_PROP_FPS, 30 );
				l_iFps = int( m_capture->get( cv::CAP_PROP_FPS ) );

				if ( l_iFps > 0 )
				{
					m_frameTime = std::chrono::milliseconds( int( 1000.0 / l_iFps ) );
				}
			}

			m_capture->set( cv::CAP_PROP_FRAME_WIDTH, 2000 );
			m_capture->set( cv::CAP_PROP_FRAME_HEIGHT, 2000 );
		}
		else
		{
			ShowMessageBox( _( "Capture device" ), _( "No capture device available" ) );
		}

		int l_width = int( m_capture->get( cv::CAP_PROP_FRAME_WIDTH ) );
		int l_height = int( m_capture->get( cv::CAP_PROP_FRAME_HEIGHT ) );
		ProceduralTextures::Size l_size( l_width, l_height );

		m_cpuStep = std::make_shared< CpuStep >( shared_from_this(), m_capture, l_size );
		m_gpuStep = std::make_shared< GpuStep >( shared_from_this(), l_size, p_bordersSize );
	}

	void Generator::DoDestroy()
	{
		m_cpuStep.reset();
		m_gpuStep.reset();

		if ( m_capture && m_capture->isOpened() )
		{
			m_capture->release();
		}

		m_capture.reset();
	}

	void Generator::DoGeneratePanel()
	{
		using namespace ProceduralTextures;

		String l_sepChoices[] =
		{
			_( "Not separated" ),
			_( "Horizontally" ),
			_( "Vertically" ),
		};
		String l_shaChoices[] =
		{
			_( "New..." ),
		};

		m_staticSeparator = std::make_shared< StaticCtrl >( m_options, _( "Separation" ), Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER );
		m_comboSeparator = std::make_shared< ComboBoxCtrl >( m_options, l_sepChoices, 0, eID_SEPTYPE, Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY );
		m_comboSeparator->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnSepType, this, std::placeholders::_1 ) );
		m_sliderOffset = std::make_shared< SliderCtrl >( m_options, Range( 0, 100 ), 50, eID_SEPOFFSET, Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0 );
		m_sliderOffset->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::OnSepOffset, this, std::placeholders::_1 ) );
		m_buttonReset = std::make_shared< ButtonCtrl >( m_options, _( "Reset Time" ), eID_RESETTIME, Position( 10, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0 );
		m_buttonReset->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnResetTime, this ) );
		m_staticShaders = std::make_shared< StaticCtrl >( m_options, _( "Shaders" ), Position( 10, 10 + DEFAULT_HEIGHT * 4 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER );
		m_comboShaders = std::make_shared< ComboBoxCtrl >( m_options, l_shaChoices, -1, eID_SHADERS, Position( 10, 10 + DEFAULT_HEIGHT * 5 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY );
		m_comboShaders->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnSelectShader, this, std::placeholders::_1 ) );
		m_buttonVertexFile = std::make_shared< ButtonCtrl >( m_options, _( "Vertex" ), eID_VERTEXFILE, Position( 10, 10 + DEFAULT_HEIGHT * 6 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonVertexFile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnVertexShaderPath, this ) );
		m_buttonFragmentFile = std::make_shared< ButtonCtrl >( m_options, _( "Pixel" ), eID_FRAGMENTFILE, Position( 10, 10 + DEFAULT_HEIGHT * 7 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonFragmentFile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnFragmentShaderPath, this ) );
		m_buttonCompile = std::make_shared< ButtonCtrl >( m_options, _( "Compile" ), eID_COMPILE, Position( 10, 10 + DEFAULT_HEIGHT * 8 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonCompile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnShaderCompile, this ) );
		m_buttonCompilerLog = std::make_shared< ButtonCtrl >( m_options, _( "Compiler log" ), eID_COMPILERLOG, Position( 10, 10 + DEFAULT_HEIGHT * 9 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonCompilerLog->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnCompilerLog, this ) );
		m_buttonRemove = std::make_shared< ButtonCtrl >( m_options, _( "Remove" ), eID_REMOVE, Position( 10, 10 + DEFAULT_HEIGHT * 10 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonRemove->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnRemove, this ) );

		m_arrayControls.push_back( m_staticSeparator );
		m_arrayControls.push_back( m_comboSeparator );
		m_arrayControls.push_back( m_sliderOffset );
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

	void Generator::OnSepType( int p_value )
	{
		m_gpuStep->SetSepType( eSEPARATION( p_value ) );

		if ( p_value )
		{
			m_sliderOffset->Show();
		}
		else
		{
			m_sliderOffset->Hide();
		}
	}

	void Generator::OnSepOffset( int p_value )
	{
		m_gpuStep->SetSepOffset( p_value );
	}

	void Generator::OnSelectShader( uint32_t p_value )
	{
		if ( p_value == m_comboShaders->GetItemCount() - 1 )
		{
			m_gpuStep->AddEffect();
			ProceduralTextures::StringStream l_strPage;
			l_strPage << _T( "Shader " );
			l_strPage << m_comboShaders->GetItemCount();
			m_comboShaders->SetItemText( p_value, l_strPage.str() );
			m_comboShaders->AppendItem( _( "New..." ) );
			m_comboShaders->SetSelected( p_value );
		}
		else if ( p_value >= 0 )
		{
			m_comboShaders->SetSelected( p_value );
			m_buttonRemove->Show();
		}
		else
		{
			m_comboShaders->SetSelected( -1 );
			m_buttonRemove->Hide();
		}

		if ( p_value < m_comboShaders->GetItemCount() )
		{
			m_buttonVertexFile->Show();
			m_buttonFragmentFile->Show();
			m_gpuStep->SelectEffect( p_value );
		}
		else
		{
			m_buttonVertexFile->Hide();
			m_buttonFragmentFile->Hide();
		}
	}

	void Generator::OnRemove()
	{
		size_t l_uiToRemove = size_t( m_comboShaders->GetSelected() );

		if ( l_uiToRemove > 0 && l_uiToRemove < m_comboShaders->GetItemCount() - 1 )
		{
			m_gpuStep->RemoveEffect();
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
		m_gpuStep->CompileEffect();
		m_buttonCompile->Hide();
		m_buttonCompilerLog->Show();
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
