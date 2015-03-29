#include "ColoredLifeGame.h"

namespace ColoredLifeGame
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

		std::shared_ptr< SliderCtrl > l_red = std::make_shared< SliderCtrl >( Range( 0, 255 ), 255, eRED_VALUE, Position( 30, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH -  40, DEFAULT_HEIGHT ) );
		l_red->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::DoSetRed, this, std::placeholders::_1 ) );
		l_red->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetRed, this, std::placeholders::_1 ) );
		std::shared_ptr< SliderCtrl > l_green = std::make_shared< SliderCtrl >( Range( 0, 255 ), 255, eGREEN_VALUE, Position( 30, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH -  40, DEFAULT_HEIGHT ) );
		l_green->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::DoSetGreen, this, std::placeholders::_1 ) );
		l_green->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetGreen, this, std::placeholders::_1 ) );
		std::shared_ptr< SliderCtrl > l_blue = std::make_shared< SliderCtrl >( Range( 0, 255 ), 255, eBLUE_VALUE, Position( 30, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH -  40, DEFAULT_HEIGHT ) );
		l_blue->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::DoSetBlue, this, std::placeholders::_1 ) );
		l_blue->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetBlue, this, std::placeholders::_1 ) );

		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "R" ), Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( 20, DEFAULT_HEIGHT ) ) );
		m_arrayControls.push_back( l_red );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "G" ), Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( 20, DEFAULT_HEIGHT ) ) );
		m_arrayControls.push_back( l_green );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "B" ), Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( 20, DEFAULT_HEIGHT ) ) );
		m_arrayControls.push_back( l_blue );
	}

	void Generator::DoSetRed( int val )
	{
		m_cpuStep->SetRed( val );
	}

	void Generator::DoSetGreen( int val )
	{
		m_cpuStep->SetGreen( val );
	}

	void Generator::DoSetBlue( int val )
	{
		m_cpuStep->SetBlue( val );
	}
}
