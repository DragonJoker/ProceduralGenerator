#include "OCLProcessing.h"

#include "OclEffect.h"

namespace OCLProcessing
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
			eID_IMAGEPATH = 52,
			eID_SEPTYPE = 53,
			eID_SEPOFFSET = 54,
			eID_RESETTIME = 55,
			eID_PROGRAMS = 56,
			eID_REMOVE = 57,
			eID_FILE = 58,
			eID_KERNEL = 59,
			eID_COMPILERLOG = 60,
			eID_HELP_OPEN = 61,
		}	eID;
	}

	Generator::Generator()
		: ProceduralTextures::Generator< ProceduralTextures::DefaultCpuStep, GpuStep >( true, ProceduralTextures::DEFAULT_FRAME_TIME )
	{
	}

	Generator::~Generator()
	{
	}

	void Generator::DoCreate( ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize )
	{
		m_cpuStep = std::make_shared< ProceduralTextures::DefaultCpuStep >( shared_from_this(), p_size );
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

		String l_sepChoices[] =
		{
			_( "Not separated" ),
			_( "Horizontally" ),
			_( "Vertically" ),
		};
		String l_progChoices[] =
		{
			_( "Program 1" ),
			_( "New..." ),
		};
		StringStream l_helpText;
		l_helpText << _( "Kernel parameters" ) << std::endl;
		l_helpText << _T( " - pg_time[opt]: int\n    " ) << _( "Elapsed time, in ms" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - pg_sep_type[opt]: int\n    " ) << _( "Separation type" ) << std::endl;
		l_helpText << _T( "       0 =>" ) + _( "Not separated" ) << std::endl;
		l_helpText << _T( "       1 =>" ) + _( "Horizontally" ) << std::endl;
		l_helpText << _T( "       2 =>" ) + _( "Vertically" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - pg_sep_offset[opt]: int\n    " ) << _( "Separator position." ) << std::endl;
		l_helpText << _T( "       0 <= position <= width" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - pg_srcImg[in]: image2d_t\n    " ) << _( "The input image." ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _T( " - pg_dstImg[out]: image2d_t\n    " ) << _( "The output image." ) << std::endl;
		l_helpText << std::endl;
		l_helpText << std::endl;
		l_helpText << _( "To help the plug-in to parse your kernels," ) << std::endl;
		l_helpText << _( "please define the following macros:" ) << std::endl;
		l_helpText << _( "  #define KERNEL_NAME(name) name" ) << std::endl;
		l_helpText << _( "  #define KERNEL_PARAM(name) name" ) << std::endl;
		l_helpText << std::endl;
		l_helpText << _( "And define your kernels like this:" ) << std::endl;
		l_helpText << _T( "  __kernel void KERNEL_NAME(Kernel)(" ) << std::endl;
		l_helpText << _T( "  	__read_only  image2d_t KERNEL_PARAM(pg_srcImg)," ) << std::endl;
		l_helpText << _T( "  	__write_only image2d_t KERNEL_PARAM(pg_dstImg)," ) << std::endl;
		l_helpText << _T( "  	int KERNEL_PARAM(pg_sep_type)," ) << std::endl;
		l_helpText << _T( "  	int KERNEL_PARAM(pg_sep_offset)" ) << std::endl;
		l_helpText << _T( "  	)" ) << std::endl;
		l_helpText << _T( "  {" ) << std::endl;
		l_helpText << _T( "  	..." ) << std::endl;
		l_helpText << _T( "  }" ) << std::endl;

		uint32_t l_index = 0;
		int l_helpPanelWidth = 360;
		int l_helpPanelHeight = 450;

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

		m_buttonImage = std::make_shared< ButtonCtrl >( m_options, _( "Image" ), eID_IMAGEPATH, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ) );
		m_buttonImage->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnImage, this ) );
		m_staticSeparator = std::make_shared< StaticCtrl >( m_options, _( "Separation" ), Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER, false );
		m_comboSeparator = std::make_shared< ComboBoxCtrl >( m_options, l_sepChoices, 0, eID_SEPTYPE, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY, false );
		m_comboSeparator->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnSepType, this, std::placeholders::_1 ) );
		m_sliderOffset = std::make_shared< SliderCtrl >( m_options, Range( 0, 100 ), 50, eID_SEPOFFSET, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), 0, false );
		m_sliderOffset->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::OnSepOffset, this, std::placeholders::_1 ) );
		m_sliderOffset->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::OnSepOffset, this, std::placeholders::_1 ) );
		m_buttonReset = std::make_shared< ButtonCtrl >( m_options, _( "Reset Time" ), eID_RESETTIME, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), 0, false );
		m_buttonReset->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnResetTime, this ) );
		m_staticPrograms = std::make_shared< StaticCtrl >( m_options, _( "Programs" ), Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER, false );
		m_comboPrograms = std::make_shared< ComboBoxCtrl >( m_options, l_progChoices, -1, eID_PROGRAMS, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY, false );
		m_comboPrograms->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnSelectProgram, this, std::placeholders::_1 ) );
		m_buttonFile = std::make_shared< ButtonCtrl >( m_options, _( "Program" ), eID_FILE, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), 0, false );
		m_buttonFile->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnFilePath, this ) );
		m_staticKernel = std::make_shared< StaticCtrl >( m_options, _( "Kernels" ), Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER, false );
		m_comboKernels = std::make_shared< ComboBoxCtrl >( m_options, StringArray(), -1, eID_KERNEL, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY, false );
		m_comboKernels->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnSelectKernel, this, std::placeholders::_1 ) );
		m_buttonCompilerLog = std::make_shared< ButtonCtrl >( m_options, _( "Compiler log" ), eID_COMPILERLOG, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), 0, false );
		m_buttonCompilerLog->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnCompilerLog, this ) );
		m_buttonRemove = std::make_shared< ButtonCtrl >( m_options, _( "Remove" ), eID_REMOVE, Position( 10, 10 + DEFAULT_HEIGHT * l_index++ ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), 0, false );
		m_buttonRemove->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnRemove, this ) );

		m_arrayControls.push_back( m_buttonHelpOpen );
		m_arrayControls.push_back( m_panelHelp );
		m_arrayControls.push_back( m_panelHelpText );
		m_arrayControls.push_back( m_buttonImage );
		m_arrayControls.push_back( m_staticSeparator );
		m_arrayControls.push_back( m_comboSeparator );
		m_arrayControls.push_back( m_sliderOffset );
		m_arrayControls.push_back( m_buttonReset );
		m_arrayControls.push_back( m_staticPrograms );
		m_arrayControls.push_back( m_comboPrograms );
		m_arrayControls.push_back( m_buttonFile );
		m_arrayControls.push_back( m_staticKernel );
		m_arrayControls.push_back( m_comboKernels );
		m_arrayControls.push_back( m_buttonCompilerLog );
		m_arrayControls.push_back( m_buttonRemove );
	}

	void Generator::OnResetTime()
	{
		m_gpuStep->ResetTime();
	}

	void Generator::OnSepType( int p_value )
	{
		m_gpuStep->SetSeparationType( eSEPARATION( p_value ) );

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
		m_gpuStep->SetSeparationOffset( p_value );
	}

	void Generator::OnSelectProgram( uint32_t p_value )
	{
		unsigned int l_uiSelection = ( unsigned int )p_value;

		if ( l_uiSelection == m_comboPrograms->GetItemCount() - 1 )
		{
			m_gpuStep->AddEffect();
			ProceduralTextures::StringStream l_strPage;
			l_strPage << _T( "Program " );
			l_strPage << m_comboPrograms->GetItemCount();
			m_comboPrograms->SetItemText( l_uiSelection, l_strPage.str() );
			m_comboPrograms->AppendItem( _( "New..." ) );
			m_comboPrograms->SetSelected( l_uiSelection );
		}
		else if ( l_uiSelection >= 0 )
		{
			m_comboPrograms->SetSelected( p_value );
			m_buttonRemove->Show();
		}
		else
		{
			m_comboPrograms->SetSelected( -1 );
			m_buttonRemove->Hide();
		}

		if ( l_uiSelection < m_comboPrograms->GetItemCount() )
		{
			m_buttonFile->Show();
			m_gpuStep->SelectEffect( p_value );
		}
		else
		{
			m_buttonFile->Hide();
			m_comboKernels->Hide();
		}
	}

	void Generator::OnRemove()
	{
		uint32_t l_uiToRemove = uint32_t( m_comboPrograms->GetSelected() );

		if ( l_uiToRemove > 0 && l_uiToRemove < m_comboPrograms->GetItemCount() - 1 )
		{
			m_gpuStep->RemoveEffect( l_uiToRemove );
			m_comboPrograms->RemoveItem( int( l_uiToRemove ) );
			m_buttonFile->Hide();
			m_comboKernels->Hide();
			m_buttonRemove->Hide();
			m_buttonCompilerLog->Hide();
		}
	}

	void Generator::OnCompilerLog()
	{
		ShowMessageBox( _( "Compile and Link result" ), m_gpuStep->GetCompilerLog() );
	}

	void Generator::OnFilePath()
	{
		ProceduralTextures::String l_path;

		if ( OpenFile( l_path, _( "Choose an OpenCL program file" ), _( "OpenCL files (*.cl)|*.cl" ) ) )
		{
			try
			{
				m_gpuStep->SetFilePath( l_path );
				m_comboKernels->Clear();

				for ( size_t i = 0; i < m_gpuStep->GetSelectedEffect()->GetKernelCount(); ++i )
				{
					m_comboKernels->AppendItem( ProceduralTextures::StringUtils::ToString( m_gpuStep->GetSelectedEffect()->GetKernelName( i ).c_str() ) );
				}

				m_staticKernel->Show();
				m_comboKernels->Show();
				m_buttonCompilerLog->Show();
			}
			catch ( std::exception & l_exc )
			{
				m_staticKernel->Hide();
				m_comboKernels->Hide();
				m_buttonCompilerLog->Hide();
				ShowMessageBox( _( "OpenCL error" ), _( "Couldn't load OpenCL program:" ) + _T( "\n" ) + l_exc.what() );
			}
		}
	}

	void Generator::OnSelectKernel( uint32_t p_value )
	{
		m_gpuStep->SelectKernel( p_value );
	}

	void Generator::OnImage()
	{
		ProceduralTextures::PixelBuffer l_buffer( ProceduralTextures::Size( 0, 0 ) );

		if ( SelectImage( l_buffer ) )
		{
			Resize( l_buffer.GetDimensions() );
			m_gpuStep->SetImage( l_buffer );
			m_sliderOffset->Show();
			m_buttonReset->Show();
			m_staticPrograms->Show();
			m_comboPrograms->Show();
			m_staticSeparator->Show();
			m_comboSeparator->Show();
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
