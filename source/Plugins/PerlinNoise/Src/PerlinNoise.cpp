#include "PerlinNoise.h"

namespace PerlinNoise
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

		String l_arrayFuncNames[eNOISE_PARAM_FUNCTION_COUNT] =
		{
			_T( "cos" ),	_T( "sin" ),	_T( "tan" ),
			_T( "cosh" ),	_T( "sinh" ),	_T( "tanh" ),
			_T( "acos" ),	_T( "asin" ),	_T( "atan" ),
			_T( "sqrt" ),
			_T( "ln" ),	_T( "exp" ),	_T( "log" ),
		};
		String l_arrayTypeNames[eNOISE_TYPE_COUNT] =
		{
			_( "Turbulence" ),
			_( "Cloud" ),
			_( "Marble" ),
			_( "Parametric" ),
		};

		m_staticOctaves = std::make_shared< StaticCtrl >( _( "Octaves: " ), Position( 10, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		m_staticFrequency = std::make_shared< StaticCtrl >( _( "Frequency: " ), Position( 10, 10 + DEFAULT_HEIGHT * 5 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		m_comboFunction = std::make_shared< ComboBoxCtrl >( l_arrayFuncNames, 0, eID_FUNCTION, Position( 10, 10 + DEFAULT_HEIGHT * 12 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_comboFunction->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::DoSetFunction, this, std::placeholders::_1 ) );

		std::shared_ptr< SliderCtrl > l_red = std::make_shared< SliderCtrl >( Range( 0, 255 ), 255, eID_REDVALUE, Position( 30, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH -  40, DEFAULT_HEIGHT ) );
		l_red->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::DoSetRed, this, std::placeholders::_1 ) );
		l_red->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetRed, this, std::placeholders::_1 ) );
		std::shared_ptr< SliderCtrl > l_green = std::make_shared< SliderCtrl >( Range( 0, 255 ), 255, eID_GREENVALUE, Position( 30, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH -  40, DEFAULT_HEIGHT ) );
		l_green->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::DoSetGreen, this, std::placeholders::_1 ) );
		l_green->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetGreen, this, std::placeholders::_1 ) );
		std::shared_ptr< SliderCtrl > l_blue = std::make_shared< SliderCtrl >( Range( 0, 255 ), 255, eID_BLUEVALUE, Position( 30, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH -  40, DEFAULT_HEIGHT ) );
		l_blue->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::DoSetBlue, this, std::placeholders::_1 ) );
		l_blue->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetBlue, this, std::placeholders::_1 ) );
		std::shared_ptr< SliderCtrl > l_octaves = std::make_shared< SliderCtrl >( Range( 2, 100 ), 2, eID_OCTAVES, Position( 10, 10 + DEFAULT_HEIGHT * 4 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ) );
		l_octaves->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::DoSetOctaves, this, std::placeholders::_1 ) );
		l_octaves->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetOctaves, this, std::placeholders::_1 ) );
		std::shared_ptr< SliderCtrl > l_frequency = std::make_shared< SliderCtrl >( Range( 2, 1000 ), 50, eID_FREQUENCY, Position( 10, 10 + DEFAULT_HEIGHT * 6 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ) );
		l_frequency->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::DoSetFrequency, this, std::placeholders::_1 ) );
		l_frequency->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::DoSetFrequency, this, std::placeholders::_1 ) );
		std::shared_ptr< ListBoxCtrl > l_types = std::make_shared< ListBoxCtrl >( l_arrayTypeNames, 0, eID_TYPE, Position( 10, 10 + DEFAULT_HEIGHT * 8 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT * 3 ) );
		l_types->Connect( eLISTBOX_EVENT_SELECTED, std::bind( &Generator::DoSetType, this, std::placeholders::_1 ) );

		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "R" ), Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( 20, DEFAULT_HEIGHT ) ) );
		m_arrayControls.push_back( l_red );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "G" ), Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( 20, DEFAULT_HEIGHT ) ) );
		m_arrayControls.push_back( l_green );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "B" ), Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( 20, DEFAULT_HEIGHT ) ) );
		m_arrayControls.push_back( l_blue );
		m_arrayControls.push_back( m_staticOctaves );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _T( "2" ), Position( 90, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH - 100, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( l_octaves );
		m_arrayControls.push_back( m_staticFrequency );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _T( "50" ), Position( 90, 10 + DEFAULT_HEIGHT * 5 ), Size( CONFIG_PANEL_WIDTH - 100, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( l_frequency );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( _( "Type : " ), Position( 10, 10 + DEFAULT_HEIGHT * 7 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( l_types );
		m_arrayControls.push_back( m_comboFunction );
	}

	void Generator::DoSetRed( uint8_t p_value )
	{
		m_cpuStep->SetRed( p_value );
	}

	void Generator::DoSetGreen( uint8_t p_value )
	{
		m_cpuStep->SetGreen( p_value );
	}

	void Generator::DoSetBlue( uint8_t p_value )
	{
		m_cpuStep->SetBlue( p_value );
	}

	void Generator::DoSetOctaves( int p_value )
	{
		m_cpuStep->SetOctavesCount( p_value );
		ProceduralTextures::StringStream l_strValue;
		l_strValue << p_value;
		m_staticOctaves->SetCaption( l_strValue.str() );
	}

	void Generator::DoSetFrequency( int p_value )
	{
		m_cpuStep->SetPersistency( p_value / 1000.0f );
		ProceduralTextures::StringStream l_strValue;
		l_strValue << int( m_cpuStep->GetPersistency() * 1000.0 );
		m_staticFrequency->SetCaption( l_strValue.str() );
	}

	void Generator::DoSetType( uint32_t p_value )
	{
		m_cpuStep->SetType( eNOISE_TYPE( p_value ) );

		if ( p_value == eNOISE_TYPE_PARAM )
		{
			m_comboFunction->Show();
		}
		else
		{
			m_comboFunction->Hide();
		}
	}

	void Generator::DoSetFunction( uint32_t p_value )
	{
		m_cpuStep->SetFunction( eNOISE_PARAM_FUNCTION( p_value ) );
	}
}
