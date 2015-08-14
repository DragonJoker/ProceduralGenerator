#include "Generator.h"

#include "ControlsManager.h"
#include "CpuStep.h"
#include "Font.h"
#include "GeneratorUtils.h"
#include "GpuStep.h"
#include "OverlayManager.h"
#include "BorderPanelOverlay.h"
#include "PanelOverlay.h"
#include "TextOverlay.h"
#include "XpmLoader.h"

#include "button.xpm"
#include "button_mo.xpm"

#define PARALLEL_RENDERING	1

namespace ProceduralTextures
{
	GeneratorBase::GeneratorBase( bool p_needsConfigPanel, int p_frameTime )
		: m_needsConfigPanel( p_needsConfigPanel )
		, m_frameTime( p_frameTime )
	{
		m_panel = std::make_shared< StaticCtrl >( nullptr, _T( "" ), Position(), Size( CONFIG_PANEL_WIDTH, 512 ) );
		m_options = std::make_shared< StaticCtrl >( m_panel, _T( "" ), Position( 0, 40 ), Size( CONFIG_PANEL_WIDTH, CONFIG_PANEL_HEIGHT ) );
	}

	GeneratorBase::~GeneratorBase()
	{
	}

	void GeneratorBase::Create( Size const & p_size, Size const & p_bordersSize )
	{
		XpmLoader l_loader;
		m_buttonImage = std::make_shared< PixelBuffer >( l_loader.LoadImage( button_xpm, Utils::GetCountOf( button_xpm ) ) );
		m_buttonMouseOverImage = std::make_shared< PixelBuffer >( l_loader.LoadImage( button_mo_xpm, Utils::GetCountOf( button_mo_xpm ) ) );
		DoCreate( p_size, p_bordersSize );
		m_controlsManager = std::make_shared< ControlsManager >( DoGetThis(), DoGetGpuStep()->GetOverlayManager() );
		DoGeneratePanel();
	}

	void GeneratorBase::Destroy()
	{
		DoDestroy();
	}

	void GeneratorBase::Initialise()
	{
		DoGetCpuStep()->Initialise();
		DoGetGpuStep()->Initialise();

		m_controlsManager->Create( m_panel );

		std::shared_ptr< StaticCtrl > l_panel = std::make_shared< StaticCtrl >( m_panel, _T( "" ), Position(), Size( CONFIG_PANEL_WIDTH, 40 ) );
		l_panel->SetBackgroundColour( Colour( 0.0, 0.0, 0.0, 0.5 ) );
		l_panel->SetBackgroundBorders( Point4i( 1, 1, 1, 1 ) );
		m_controlsManager->Create( l_panel );
		std::shared_ptr< StaticCtrl > l_cpu = std::make_shared< StaticCtrl >( l_panel, _T( "" ), Position( 0, 0 ), Size( CONFIG_PANEL_WIDTH, 20 ) );
		l_cpu->SetForegroundColour( Colour( 1.0, 1.0, 1.0, 1.0 ) );
		m_controlsManager->Create( l_cpu );
		std::shared_ptr< StaticCtrl > l_gpu = std::make_shared< StaticCtrl >( l_panel, _T( "" ), Point2d( 0, 20 ), Size( CONFIG_PANEL_WIDTH, 20 ) );
		l_gpu->SetForegroundColour( Colour( 1.0, 1.0, 1.0, 1.0 ) );
		m_controlsManager->Create( l_gpu );

		m_options->SetBackgroundColour( Colour( 0.0, 0.0, 0.0, 0.5 ) );
		m_options->SetBackgroundBorders( Point4i( 1, 1, 1, 1 ) );
		m_controlsManager->Create( m_options );

		for ( auto && l_control : m_arrayControls )
		{
			switch ( l_control->GetType() )
			{
			case eCONTROL_TYPE_STATIC:
				m_controlsManager->Create( std::static_pointer_cast< StaticCtrl >( l_control ) );
				break;

			case eCONTROL_TYPE_EDIT:
				m_controlsManager->Create( std::static_pointer_cast< EditCtrl >( l_control ) );
				break;

			case eCONTROL_TYPE_SLIDER:
				m_controlsManager->Create( std::static_pointer_cast< SliderCtrl >( l_control ) );
				break;

			case eCONTROL_TYPE_COMBO:
				m_controlsManager->Create( std::static_pointer_cast< ComboBoxCtrl >( l_control ) );
				break;

			case eCONTROL_TYPE_LIST:
				m_controlsManager->Create( std::static_pointer_cast< ListBoxCtrl >( l_control ) );
				break;

			case eCONTROL_TYPE_BUTTON:
				m_controlsManager->Create( std::static_pointer_cast< ButtonCtrl >( l_control ) );
				break;
			}
		}

		m_visibility = std::make_shared< ButtonCtrl >( nullptr, _T( "-" ), 0, Position( CONFIG_PANEL_WIDTH, 0 ), Size( 20, 20 ) );
		m_visibility->Connect( eBUTTON_EVENT_CLICKED, std::bind( &GeneratorBase::DoSwitchVisibility, this ) );
		m_controlsManager->Create( m_visibility );

		m_cpuTime = l_cpu;
		m_gpuTime = l_gpu;
	}

	void GeneratorBase::Cleanup()
	{
		DoGetCpuStep()->Cleanup();
		DoGetGpuStep()->Cleanup();
		m_loadedFonts.clear();
	}

	void GeneratorBase::Run()
	{
		DoGetCpuStep()->Run();
	}

	void GeneratorBase::Stop()
	{
		DoGetCpuStep()->Stop();
	}

	void GeneratorBase::Render( std::function< void() > p_swapFunction )
	{
		std::shared_ptr< CpuStepBase > l_cpuStep = DoGetCpuStep();
		std::shared_ptr< GpuStep > l_gpuStep = DoGetGpuStep();
		l_cpuStep->SwapBuffers();
		DoWriteTime( _T( "CPU" ), l_cpuStep->GetTime(), m_cpuTime.lock() );
		DoWriteTime( _T( "GPU" ), l_gpuStep->GetTime(), m_gpuTime.lock() );
#if PARALLEL_RENDERING
		l_cpuStep->Wake();
		l_gpuStep->Render( p_swapFunction );
		l_cpuStep->Wait( std::chrono::milliseconds( -1 ) );
#else
		l_cpuStep->Render();
		DoGetGpuStep()->Render( p_swapFunction );
#endif
		l_gpuStep->UpdateBuffer( *l_cpuStep->GetBuffer() );
		m_controlsManager->ProcessEvents();
	}

	void GeneratorBase::Resize( int p_iWidth, int p_iHeight )
	{
		Resize( Size( p_iWidth, p_iHeight ) );
	}

	void GeneratorBase::Resize( Size const & p_size )
	{
		DoGetCpuStep()->Resize( p_size );
		DoGetGpuStep()->Resize( p_size );

		if ( m_callbackResize )
		{
			m_callbackResize( GetDisplaySize() );
		}
	}

	void GeneratorBase::SaveFrame()
	{
		DoGetGpuStep()->SaveFrame();
	}

	void GeneratorBase::ShowMessageBox( String const & p_title, String const & p_message )
	{
		if ( m_callbackShowMessageBox )
		{
			m_callbackShowMessageBox( p_title, p_message );
		}
	}

	void GeneratorBase::UpdateDisplaySize( Size const & p_size )
	{
		DoGetGpuStep()->UpdateDisplaySize( p_size );
	}

	bool GeneratorBase::OpenFile( String & p_path, String const & p_prompt, String const & p_wildcard )
	{
		bool l_return = false;

		if ( m_callbackOpenFile )
		{
			l_return = m_callbackOpenFile( p_path, p_prompt, p_wildcard );
		}

		return l_return;
	}

	bool GeneratorBase::SaveFile( String & p_path, String const & p_prompt, String const & p_wildcard )
	{
		bool l_return = false;

		if ( m_callbackSaveFile )
		{
			l_return = m_callbackSaveFile( p_path, p_prompt, p_wildcard );
		}

		return l_return;
	}

	bool GeneratorBase::SelectImage( PixelBuffer & p_buffer, String const & p_prompt )
	{
		bool l_return = false;

		if ( m_callbackSelectImage )
		{
			l_return = m_callbackSelectImage( p_buffer, p_prompt );
		}

		return l_return;
	}

	bool GeneratorBase::SelectColour( uint32_t & p_colour, String const & p_prompt )
	{
		bool l_return = false;

		if ( m_callbackSelectColour )
		{
			l_return = m_callbackSelectColour( p_colour, p_prompt );
		}

		return l_return;
	}

	void GeneratorBase::SetCursor( eMOUSE_CURSOR p_cursor )
	{
		if ( m_callbackSetCursor )
		{
			m_callbackSetCursor( p_cursor );
		}
	}

	void GeneratorBase::PostMouseEvent( eMOUSE_EVENT p_type, Position const & p_position, eMOUSE_BUTTON p_button )
	{
		switch ( p_type )
		{
		case eMOUSE_EVENT_MOUSE_MOVE:
			m_controlsManager->FireMouseMove( p_position - DoGetGpuStep()->GetDisplayOffset() );
			break;

		case eMOUSE_EVENT_MOUSE_BUTTON_PUSHED:
			m_controlsManager->FireMouseButtonPushed( p_button );
			break;

		case eMOUSE_EVENT_MOUSE_BUTTON_RELEASED:
			m_controlsManager->FireMouseButtonReleased( p_button );
			break;

		case eMOUSE_EVENT_MOUSE_WHEEL:
			m_controlsManager->FireMouseWheel( p_position );
			break;
		}
	}

	void GeneratorBase::PostKeyboardEvent( eKEYBOARD_EVENT p_type, eKEYBOARD_KEY p_key, bool p_ctrl, bool p_alt, bool p_shift )
	{
		switch ( p_type )
		{
		case eKEYBOARD_EVENT_KEY_PUSHED:
			m_controlsManager->FireKeyDown( p_key, p_ctrl, p_alt, p_shift );
			break;

		case eKEYBOARD_EVENT_KEY_RELEASED:
			m_controlsManager->FireKeyUp( p_key, p_ctrl, p_alt, p_shift );
			break;
		}
	}

	void GeneratorBase::PostCharEvent( eKEYBOARD_KEY p_key, String const & p_char )
	{
		m_controlsManager->FireChar( p_key, p_char );
	}

	std::shared_ptr< Font > GeneratorBase::LoadFont( String const & p_fontName, uint32_t p_height, String const & p_fontPath )
	{
		std::shared_ptr< Font > l_return;
		String l_name = p_fontName + _T( "_" ) + StringUtils::ToString( std::to_string( p_height ) );
		auto it = m_loadedFonts.find( l_name );

		if ( it == m_loadedFonts.end() )
		{
			l_return = std::make_shared< Font >( p_fontPath, p_height );
			m_loadedFonts[l_name] = l_return;
		}
		else
		{
			l_return = it->second;
		}

		return l_return;
	}

	std::shared_ptr< Font > GeneratorBase::GetFont( String const & p_fontName, uint32_t p_height )
	{
		std::shared_ptr< Font > l_return;
		auto it = m_loadedFonts.find( p_fontName + _T( "_" ) + StringUtils::ToString( std::to_string( p_height ) ) );

		if ( it != m_loadedFonts.end() )
		{
			l_return = it->second;
		}

		return l_return;
	}

	bool GeneratorBase::IsInitialised()const
	{
		return DoGetCpuStep()->IsInitialised() && DoGetGpuStep()->IsInitialised();
	}

	Size const & GeneratorBase::GetDisplaySize()const
	{
		return DoGetGpuStep()->GetDisplaySize();
	}

	Size const & GeneratorBase::GetImageSize()const
	{
		return DoGetCpuStep()->GetImageSize();
	}

	PixelBuffer const & GeneratorBase::GetSavedFrame()const
	{
		return DoGetGpuStep()->GetSavedFrame();
	}

	std::chrono::milliseconds const & GeneratorBase::GetCpuTime()const
	{
		return DoGetCpuStep()->GetTime();
	}

	std::chrono::milliseconds const & GeneratorBase::GetGpuTime()const
	{
		return DoGetGpuStep()->GetTime();
	}

	void GeneratorBase::DoSwitchVisibility()
	{
		if ( m_panel->IsVisible() )
		{
			m_panel->Hide();
			m_visibility->SetPosition( Position( 0, 0 ) );
		}
		else
		{
			m_panel->Show();
			m_visibility->SetPosition( Position( CONFIG_PANEL_WIDTH, 0 ) );
		}
	}

	void GeneratorBase::DoWriteTime( String const & p_name, std::chrono::milliseconds const & p_time, std::shared_ptr< StaticCtrl > p_text )
	{
		if ( p_text )
		{
			StringStream l_stream;
			l_stream << p_name << _T( ": " );
			l_stream.width( 3 );
			l_stream.fill( _T( ' ' ) );
			l_stream << std::right << p_time.count();
			l_stream << _T( "ms" );
			p_text->SetCaption( l_stream.str() );
		}
	}
}
