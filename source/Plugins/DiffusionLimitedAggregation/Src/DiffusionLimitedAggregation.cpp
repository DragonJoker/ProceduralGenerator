#include "DiffusionLimitedAggregation.h"

namespace DiffusionLimitedAggregation
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

		m_staticMass = std::make_shared< StaticCtrl >( _T( "10000" ), Position( 90, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH - 100,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		std::shared_ptr< SliderCtrl > l_sliderMass = std::make_shared< SliderCtrl >( Range( 1000, 100000 ), 10000, eID_MASS, Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSLIDER_STYLE_HORIZONTAL );
		l_sliderMass->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetMass, this, std::placeholders::_1 ) );
		m_staticMinValue = std::make_shared< StaticCtrl >( _T( "1" ), Position( 110, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH - 110,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		std::shared_ptr< SliderCtrl > l_sliderMinValue = std::make_shared< SliderCtrl >( Range( 1, 10 ), 1, eID_MINVALUE, Position( 10, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSLIDER_STYLE_HORIZONTAL );
		l_sliderMinValue->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetMinValue, this, std::placeholders::_1 ) );

		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "Max Mass :" ), Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( m_staticMass );
		m_arrayControls.push_back( l_sliderMass );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "MinValue :" ), Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( m_staticMinValue );
		m_arrayControls.push_back( l_sliderMinValue );
	}

	void Generator::DoSetMass( int p_value )
	{
		m_cpuStep->SetMass( p_value );
		ProceduralTextures::StringStream l_strValue;
		l_strValue << p_value;
		m_staticMass->SetCaption( l_strValue.str() );
	}

	void Generator::DoSetMinValue( int p_value )
	{
		m_cpuStep->SetMinValue( p_value );
		ProceduralTextures::StringStream l_strValue;
		l_strValue << p_value;
		m_staticMinValue->SetCaption( l_strValue.str() );
	}
}
