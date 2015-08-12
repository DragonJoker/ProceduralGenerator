#include "ImageEffects.h"

#include "IfxCpuStep.h"

namespace ImageEffects
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

		StringArray l_values1;
		StringArray l_values2;

		for ( int i = eEFFECT_TYPE_NONE; i < eEFFECT_TYPE_COUNT; i++ )
		{
			auto l_pFunctor = m_cpuStep->GetFunctor( eEFFECT_TYPE( i ) );
			l_values1.push_back( l_pFunctor->GetName() );

			if ( l_pFunctor->CanBeSecondEffect() )
			{
				l_values2.push_back( l_pFunctor->GetName() );
			}
		}

		m_staticFirstFunction = std::make_shared< StaticCtrl >( m_options, _( "First Function" ), Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER );
		m_comboFirstFunction = std::make_shared< ComboBoxCtrl >( m_options, l_values1, -1, eID_FIRSTFUNCTION, Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY );
		m_comboFirstFunction->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnFirstFunction, this, std::placeholders::_1 ) );
		m_staticSecondFunction = std::make_shared< StaticCtrl >( m_options, _( "Second Function" ), Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER | eSTATIC_STYLE_HALIGN_CENTER );
		m_comboSecondFunction = std::make_shared< ComboBoxCtrl >( m_options, l_values2, -1, eID_SECONDFUNCTION, Position( 10, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY );
		m_comboSecondFunction->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnSecondFunction, this, std::placeholders::_1 ) );
		m_buttonImage = std::make_shared< ButtonCtrl >( m_options, _( "Image" ), eID_IMAGEPATH, Position( 10, 10 + DEFAULT_HEIGHT * 4 ), Size( CONFIG_PANEL_WIDTH - 20, DEFAULT_HEIGHT ) );
		m_buttonImage->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnImage, this ) );

		m_arrayControls.push_back( m_staticFirstFunction );
		m_arrayControls.push_back( m_comboFirstFunction );
		m_arrayControls.push_back( m_staticSecondFunction );
		m_arrayControls.push_back( m_comboSecondFunction );
		m_arrayControls.push_back( m_buttonImage );
	}

	void Generator::OnFirstFunction( uint32_t p_choice )
	{
		m_cpuStep->SetFirstFunction( p_choice );

		if ( p_choice >= eEFFECT_TYPE_REC_GAUSSIAN_BLUR )
		{
			m_comboSecondFunction->SetSelected( 0 );
			m_comboSecondFunction->Hide();
			m_staticSecondFunction->Hide();
		}
		else
		{
			m_comboSecondFunction->Show();
			m_staticSecondFunction->Show();
		}
	}

	void Generator::OnSecondFunction( uint32_t p_choice )
	{
		m_cpuStep->SetSecondFunction( p_choice );
	}

	void Generator::OnImage()
	{
		ProceduralTextures::PixelBuffer l_buffer;

		if ( SelectImage( l_buffer ) )
		{
			m_cpuStep->SetImage( l_buffer );
			Resize( l_buffer.GetDimensions() );
		}
	}
}
