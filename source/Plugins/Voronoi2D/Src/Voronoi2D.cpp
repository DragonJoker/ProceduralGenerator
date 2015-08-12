#include "Voronoi2D.h"

#include "V2dCpuStep.h"

namespace Voronoi2D
{
	Generator::Generator()
		: ProceduralTextures::Generator< CpuStep, ProceduralTextures::DefaultGpuStep >( true, ProceduralTextures::DEFAULT_FRAME_TIME )
	{
	}

	Generator::~Generator()
	{
	}

	void Generator::DoCreate( ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize )
	{
		m_cpuStep = std::make_shared< CpuStep >( shared_from_this(), p_size );
		m_gpuStep = std::make_shared< ProceduralTextures::DefaultGpuStep >( shared_from_this(), p_size, p_bordersSize );
	}

	void Generator::DoDestroy()
	{
		m_cpuStep.reset();
		m_gpuStep.reset();
	}

	void Generator::DoGeneratePanel()
	{
		using namespace ProceduralTextures;

		String l_arrayChoices[] =
		{
			_( "Euclidian" ),
			_( "Squared Euclidian" ),
			_( "Manhattan" ),
			_( "Minkowski" ),
			_( "Chebychev" ),
		};

		m_buttonReset = std::make_shared< ButtonCtrl >( m_options, _( "Reset" ), eID_RESET, Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ) );
		m_buttonReset->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnReset, this ) );
		m_statDepthTitle = std::make_shared< StaticCtrl >( m_options, _( "Max Depth:" ), Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		m_statDepth = std::make_shared< StaticCtrl >( m_options, _T( "5" ), Position( 90, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH - 100,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		m_slideDepth = std::make_shared< SliderCtrl >( m_options, Range( 1, 100 ), 5, eID_DEPTH, Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSLIDER_STYLE_HORIZONTAL );
		m_slideDepth->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::OnMaxDepths, this, std::placeholders::_1 ) );
		m_slideDepth->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::OnMaxDepthsTrack, this, std::placeholders::_1 ) );
		m_statNeighboursTitle = std::make_shared< StaticCtrl >( m_options, _( "Neighbours:" ), Position( 10, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		m_statNeighbours = std::make_shared< StaticCtrl >( m_options, _T( "5" ), Position( 90, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH - 100,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		m_slideNeighbours = std::make_shared< SliderCtrl >( m_options, Range( 1, 256 ), 32, eID_NEIGHBOURS, Position( 10, 10 + DEFAULT_HEIGHT * 4 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSLIDER_STYLE_HORIZONTAL );
		m_slideNeighbours->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::OnMaxNeighbours, this, std::placeholders::_1 ) );
		m_slideNeighbours->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::OnMaxNeighboursTrack, this, std::placeholders::_1 ) );
		m_statDistance = std::make_shared< StaticCtrl >( m_options, _( "Distance Function:" ), Position( 10, 10 + DEFAULT_HEIGHT * 5 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		m_comboDistance = std::make_shared< ComboBoxCtrl >( m_options, l_arrayChoices, 0, eID_DISTANCE_FUNC, Position( 10, 10 + DEFAULT_HEIGHT * 6 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY );
		m_comboDistance->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnDistanceType, this, std::placeholders::_1 ) );
		m_statMinkowski = std::make_shared< StaticCtrl >( m_options, _( "Minkowski Order : " ) + String( _T( "4.0" ) ), Position( 10, 10 + DEFAULT_HEIGHT * 7 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER, false );
		m_slideMinkowski = std::make_shared< SliderCtrl >( m_options, Range( 1, 256 ), 40, eID_MINKOWSKI_ORDER, Position( 10, 10 + DEFAULT_HEIGHT * 8 ), Size( CONFIG_PANEL_WIDTH -  20,  DEFAULT_HEIGHT ), eSLIDER_STYLE_HORIZONTAL, false );
		m_slideMinkowski->Connect( eSLIDER_EVENT_THUMBRELEASE, std::bind( &Generator::OnMinkowskiOrder, this, std::placeholders::_1 ) );
		m_slideMinkowski->Connect( eSLIDER_EVENT_THUMBTRACK, std::bind( &Generator::OnMinkowskiOrderTrack, this, std::placeholders::_1 ) );

		m_arrayControls.push_back( m_buttonReset );
		m_arrayControls.push_back( m_statDepthTitle );
		m_arrayControls.push_back( m_statDepth );
		m_arrayControls.push_back( m_slideDepth );
		m_arrayControls.push_back( m_statNeighboursTitle );
		m_arrayControls.push_back( m_statNeighbours );
		m_arrayControls.push_back( m_slideNeighbours );
		m_arrayControls.push_back( m_statDistance );
		m_arrayControls.push_back( m_comboDistance );
		m_arrayControls.push_back( m_statMinkowski );
		m_arrayControls.push_back( m_slideMinkowski );
	}

	void Generator::OnMaxDepths( int p_value )
	{
		if ( IsInitialised() )
		{
			m_cpuStep->SetMaxDepth( p_value );
			ProceduralTextures::StringStream l_strValue;
			l_strValue << p_value;
			m_statDepth->SetCaption( l_strValue.str() );
		}
	}

	void Generator::OnMaxDepthsTrack( int p_value )
	{
		ProceduralTextures::StringStream l_strValue;
		l_strValue << p_value;
		m_statDepth->SetCaption( l_strValue.str() );
	}

	void Generator::OnMaxNeighbours( int p_value )
	{
		if ( IsInitialised() )
		{
			m_cpuStep->SetMaxNeighbours( p_value );
			ProceduralTextures::StringStream l_strValue;
			l_strValue << p_value;
			m_statNeighbours->SetCaption( l_strValue.str() );
		}
	}

	void Generator::OnMaxNeighboursTrack( int p_value )
	{
		ProceduralTextures::StringStream l_strValue;
		l_strValue << p_value;
		m_statNeighbours->SetCaption( l_strValue.str() );
	}

	void Generator::OnMinkowskiOrder( int p_value )
	{
		double l_dValue = double( p_value ) / 10.0;

		if ( IsInitialised() )
		{
			m_cpuStep->SetMinkowskiOrder( l_dValue );
			ProceduralTextures::StringStream l_strValue;
			l_strValue << _( "Minkowski Order : " ) << l_dValue;
			m_statMinkowski->SetCaption( l_strValue.str() );
		}
	}

	void Generator::OnMinkowskiOrderTrack( int p_value )
	{
		double l_dValue = double( p_value ) / 10.0;
		ProceduralTextures::StringStream l_strValue;
		l_strValue << _( "Minkowski Order : " ) << l_dValue;
		m_statMinkowski->SetCaption( l_strValue.str() );
	}

	void Generator::OnDistanceType( int p_value )
	{
		eDISTANCE_FUNC l_eValue = eDISTANCE_FUNC( p_value );

		if ( IsInitialised() )
		{
			m_cpuStep->SetTypeDistance( l_eValue );

			if ( l_eValue == eDISTANCE_FUNC_MINKOWSKI )
			{
				m_statMinkowski->Show();
				m_slideMinkowski->Show();
			}
			else
			{
				m_statMinkowski->Hide();
				m_slideMinkowski->Hide();
			}
		}
	}

	void Generator::OnReset()
	{
		Cleanup();
		Initialise();
		m_cpuStep->Generate();
	}
}
