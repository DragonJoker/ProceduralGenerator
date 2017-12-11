#include "GPULifeGame.h"

namespace GpuLifeGame
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
			eID_RESETTIME = 53,
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

		m_arrayControls.push_back( m_buttonHelpOpen );
		m_arrayControls.push_back( m_panelHelp );
		m_arrayControls.push_back( m_panelHelpText );
		m_arrayControls.push_back( m_buttonReset );
	}

	void Generator::OnResetTime()
	{
		m_gpuStep->ResetTime();
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
