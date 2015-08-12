#include "IteratedFunctionSystem.h"

namespace IteratedFunctionSystem
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

		std::shared_ptr< EditCtrl > l_editAngle = std::make_shared< EditCtrl >( m_options, _T( "0" ), eID_ANGLE, Position( 50, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH - 60, DEFAULT_HEIGHT ), eEDIT_STYLE_PROCESS_ENTER );
		l_editAngle->Connect( eEDIT_EVENT_UPDATED, std::bind( &Generator::DoSetAngle, this, std::placeholders::_1 ) );
		std::shared_ptr< EditCtrl > l_editShift = std::make_shared< EditCtrl >( m_options, _T( "0" ), eID_SHIFT, Position( 50, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH - 60, DEFAULT_HEIGHT ), eEDIT_STYLE_PROCESS_ENTER );
		l_editShift->Connect( eEDIT_EVENT_UPDATED, std::bind( &Generator::DoSetShift, this, std::placeholders::_1 ) );
		std::shared_ptr< EditCtrl > l_editSize = std::make_shared< EditCtrl >( m_options, _T( "0" ), eID_SIZE, Position( 50, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH - 60, DEFAULT_HEIGHT ), eEDIT_STYLE_PROCESS_ENTER );
		l_editSize->Connect( eEDIT_EVENT_UPDATED, std::bind( &Generator::DoSetSize, this, std::placeholders::_1 ) );
		std::shared_ptr< ComboBoxCtrl > l_comboGenerator = std::make_shared< ComboBoxCtrl >( m_options, CpuStep::ArrayNames, eIFS_GENERATOR_SIERPINSKI_CARPET, eID_GENERATOR, Position( 10, 10 + DEFAULT_HEIGHT * 4 ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY );
		l_comboGenerator->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::DoSetGenerator, this, std::placeholders::_1 ) );

		m_arrayControls.push_back( std::make_shared< StaticCtrl >( m_options, _( "Angle :" ), Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( 40, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( l_editAngle );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( m_options, _( "Shift :" ), Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( 40, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( l_editShift );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( m_options, _( "Size :" ), Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( 40, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( l_editSize );
		m_arrayControls.push_back( std::make_shared< StaticCtrl >( m_options, _( "Generator :" ), Position( 10, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER ) );
		m_arrayControls.push_back( l_comboGenerator );
	}

	void Generator::DoSetAngle( ProceduralTextures::String const & p_value )
	{
		m_cpuStep->SetGeneratorAngle( std::stol( p_value ) );
	}

	void Generator::DoSetShift( ProceduralTextures::String const & p_value )
	{
		m_cpuStep->SetGeneratorShift( std::stol( p_value ) );
	}

	void Generator::DoSetSize( ProceduralTextures::String const & p_value )
	{
		m_cpuStep->SetGeneratorSize( std::stol( p_value ) );
	}

	void Generator::DoSetGenerator( int p_value )
	{
		m_cpuStep->SetGenerator( eIFS_GENERATOR( p_value ) );
	}
}
