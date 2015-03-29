#include "TextEffects.h"

namespace TextEffects
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

		String l_bgChoices[] =
		{
			_T( "Colour" ),
			_T( "Texture" ),
		};

		m_staticForeground = std::make_shared< StaticCtrl >( _( "Foreground type" ), Position( 10, 10 + DEFAULT_HEIGHT * 0 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ) );
		m_comboForeground = std::make_shared< ComboBoxCtrl >( l_bgChoices, 0, eID_TEXTBGTYPE, Position( 10, 10 + DEFAULT_HEIGHT * 1 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), eCOMBO_STYLE_READONLY );
		m_comboForeground->Connect( eCOMBOBOX_EVENT_SELECTED, std::bind( &Generator::OnForegroundType, this, std::placeholders::_1 ) );
		m_buttonForegroundColour = std::make_shared< ButtonCtrl >( _( "Foreground colour" ), eID_TEXTBG_COLOUR, Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ) );
		m_buttonForegroundColour->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnForegroundColour, this ) );
		m_buttonForegroundImage = std::make_shared< ButtonCtrl >( _( "Foreground image" ), eID_TEXTBG_IMAGE, Position( 10, 10 + DEFAULT_HEIGHT * 2 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ), 0, false );
		m_buttonForegroundImage->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnForegroundImage, this ) );
		m_buttonBackgroundImage = std::make_shared< ButtonCtrl >( _( "Background image" ), eID_BACKGROUND, Position( 10, 10 + DEFAULT_HEIGHT * 3 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ) );
		m_buttonBackgroundImage->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnBackgroundImage, this ) );
		m_buttonFont = std::make_shared< ButtonCtrl >( _( "Text Font" ), eID_FONT, Position( 10, 10 + DEFAULT_HEIGHT * 4 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ) );
		m_buttonFont->Connect( eBUTTON_EVENT_CLICKED, std::bind( &Generator::OnFont, this ) );
		m_staticCaption = std::make_shared< StaticCtrl >( _( "Caption" ), Position( 10, 10 + DEFAULT_HEIGHT * 5 ), Size( CONFIG_PANEL_WIDTH -  20, DEFAULT_HEIGHT ) );
		m_editCaption = std::make_shared< EditCtrl >( _T( "" ), eID_TEXT, Position( 10, 10 + DEFAULT_HEIGHT * 6 ), Size( CONFIG_PANEL_WIDTH -  20, 472 - ( 10 + DEFAULT_HEIGHT * 6 ) - 10 ), eEDIT_STYLE_MULTILINE | eEDIT_STYLE_PROCESS_TAB | eEDIT_STYLE_WORDWRAP );
		m_editCaption->Connect( eEDIT_EVENT_UPDATED, std::bind( &Generator::OnChar, this, std::placeholders::_1 ) );

		m_editCaption->SetCaption( _T( "The quick brown fox jumps over the lazy dog." ) );
		m_gpuStep->SetText( m_editCaption->GetCaption() );

		m_arrayControls.push_back( m_staticForeground );
		m_arrayControls.push_back( m_comboForeground );
		m_arrayControls.push_back( m_buttonForegroundImage );
		m_arrayControls.push_back( m_buttonForegroundColour );
		m_arrayControls.push_back( m_buttonBackgroundImage );
		m_arrayControls.push_back( m_buttonFont );
		m_arrayControls.push_back( m_staticCaption );
		m_arrayControls.push_back( m_editCaption );
	}

	void Generator::OnForegroundType( int p_value )
	{
		m_gpuStep->SetForegroundType( ProceduralTextures::eMATERIAL_TYPE( p_value ) );

		if ( p_value == ProceduralTextures::eMATERIAL_TYPE_COLOUR )
		{
			m_buttonForegroundColour->Show();
			m_buttonForegroundImage->Hide();
		}
		else
		{
			m_buttonForegroundColour->Hide();
			m_buttonForegroundImage->Show();
		}
	}

	void Generator::OnChar( ProceduralTextures::String const & p_text )
	{
		m_gpuStep->SetText( p_text );
	}

	void Generator::OnForegroundColour()
	{
		uint32_t l_colour;

		if ( SelectColour( l_colour ) )
		{
			m_gpuStep->SetForegroundColour( l_colour );
		}
	}

	void Generator::OnForegroundImage()
	{
		ProceduralTextures::PixelBuffer l_image;

		if ( SelectImage( l_image ) )
		{
			m_gpuStep->SetForegroundImage( l_image );
		}
	}

	void Generator::OnBackgroundImage()
	{
		ProceduralTextures::PixelBuffer l_image;

		if ( SelectImage( l_image ) )
		{
			m_cpuStep->SetBackgroundImage( l_image );
			m_gpuStep->SetBackgroundImage( l_image );
			Resize( l_image.GetDimensions() );
		}
	}

	void Generator::OnFont()
	{
		ProceduralTextures::String l_path;

		if ( OpenFile( l_path, _( "Choose a font file" ), _( "All files (*.*)|*.*" ) ) )
		{
			m_gpuStep->SetFont( l_path );
		}
	}
}
