#include "ControlsManager.h"

#include "BorderPanelOverlay.h"
#include "CtrlControl.h"
#include "CtrlButton.h"
#include "CtrlComboBox.h"
#include "CtrlEdit.h"
#include "CtrlListBox.h"
#include "CtrlSlider.h"
#include "CtrlStatic.h"
#include "Generator.h"
#include "GpuStep.h"
#include "Translator.h"

namespace ProceduralTextures
{
	ControlsManager::ControlsManager( std::shared_ptr< GeneratorBase > p_generator, std::shared_ptr< OverlayManager > p_overlayManager )
		: m_generator( p_generator )
		, m_overlayManager( p_overlayManager )
	{
		m_mouse.m_buttons[eMOUSE_BUTTON_LEFT] = false;
		m_mouse.m_buttons[eMOUSE_BUTTON_MIDDLE] = false;
		m_mouse.m_buttons[eMOUSE_BUTTON_RIGHT] = false;
		m_mouse.m_changed = eMOUSE_BUTTON_COUNT;
		m_keyboard.m_ctrl = false;
		m_keyboard.m_alt = false;
		m_keyboard.m_shift = false;
	}

	ControlsManager::~ControlsManager()
	{
	}

	void ControlsManager::ProcessEvents()
	{
		std::vector< std::shared_ptr< Control > > l_controls = DoGetControls();

		for ( auto && l_control : l_controls )
		{
			l_control->ProcessEvents();
		}
	}

	void ControlsManager::FireMouseMove( Position const & p_position )
	{
		m_mouse.m_position = p_position;
		bool l_unset = false;
		std::shared_ptr< Control > l_control = DoGetMouseTargetableControl( p_position );
		std::shared_ptr< Control > l_last = m_lastMouseTarget.lock();

		if ( l_control != l_last )
		{
			if ( l_last )
			{
				l_last->PushEvent( MouseEvent( eMOUSE_EVENT_MOUSE_LEAVE, p_position ) );
				l_last.reset();
				m_lastMouseTarget.reset();
				l_unset = true;
			}
		}

		if ( l_control )
		{
			if ( l_control != l_last )
			{
				l_control->PushEvent( MouseEvent( eMOUSE_EVENT_MOUSE_ENTER, p_position ) );
				m_generator.lock()->SetCursor( l_control->GetCursor() );
			}

			l_control->PushEvent( MouseEvent( eMOUSE_EVENT_MOUSE_MOVE, p_position ) );
			m_lastMouseTarget = l_control;
		}
		else if ( l_unset )
		{
			m_generator.lock()->SetCursor( eMOUSE_CURSOR_ARROW );
		}
	}

	void ControlsManager::FireMouseButtonPushed( eMOUSE_BUTTON p_button )
	{
		m_mouse.m_buttons[p_button] = true;
		m_mouse.m_changed = p_button;
		std::shared_ptr< Control > l_control = DoGetMouseTargetableControl( m_mouse.m_position );

		if ( l_control )
		{
			std::shared_ptr< Control > l_active = m_activeControl.lock();

			if ( l_control != l_active )
			{
				if ( l_active )
				{
					l_active->PushEvent( ControlEvent( eCONTROL_EVENT_DEACTIVATE, l_control ) );
				}

				l_control->PushEvent( ControlEvent( eCONTROL_EVENT_ACTIVATE, l_active ) );
			}

			l_control->PushEvent( MouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, m_mouse.m_position, p_button ) );
			m_activeControl = l_control;
		}
	}

	void ControlsManager::FireMouseButtonReleased( eMOUSE_BUTTON p_button )
	{
		m_mouse.m_buttons[p_button] = false;
		m_mouse.m_changed = p_button;
		std::shared_ptr< Control > l_control = DoGetMouseTargetableControl( m_mouse.m_position );

		if ( l_control )
		{
			l_control->PushEvent( MouseEvent( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, m_mouse.m_position, p_button ) );
			m_activeControl = l_control;
		}
	}

	void ControlsManager::FireMouseWheel( Position const & p_offsets )
	{
		m_mouse.m_wheel += p_offsets;
		std::shared_ptr< Control > l_control = DoGetMouseTargetableControl( m_mouse.m_position );

		if ( l_control )
		{
			l_control->PushEvent( MouseEvent( eMOUSE_EVENT_MOUSE_WHEEL, p_offsets ) );
		}
	}

	void ControlsManager::FireKeyDown( eKEYBOARD_KEY p_key, bool p_ctrl, bool p_alt, bool p_shift )
	{
		std::shared_ptr< Control > l_control = m_activeControl.lock();

		if ( l_control )
		{
			if ( p_key == eKEY_CONTROL )
			{
				m_keyboard.m_ctrl = true;
			}

			if ( p_key == eKEY_ALT )
			{
				m_keyboard.m_alt = true;
			}

			if ( p_key == eKEY_SHIFT )
			{
				m_keyboard.m_shift = true;
			}

			l_control->PushEvent( KeyboardEvent( eKEYBOARD_EVENT_KEY_PUSHED, p_key, m_keyboard.m_ctrl, m_keyboard.m_alt, m_keyboard.m_shift ) );
		}
	}

	void ControlsManager::FireKeyUp( eKEYBOARD_KEY p_key, bool p_ctrl, bool p_alt, bool p_shift )
	{
		std::shared_ptr< Control > l_control = m_activeControl.lock();

		if ( l_control )
		{
			if ( p_key == eKEY_CONTROL )
			{
				m_keyboard.m_ctrl = false;
			}

			if ( p_key == eKEY_ALT )
			{
				m_keyboard.m_alt = false;
			}

			if ( p_key == eKEY_SHIFT )
			{
				m_keyboard.m_shift = false;
			}

			l_control->PushEvent( KeyboardEvent( eKEYBOARD_EVENT_KEY_RELEASED, p_key, m_keyboard.m_ctrl, m_keyboard.m_alt, m_keyboard.m_shift ) );
		}
	}

	void ControlsManager::FireChar( eKEYBOARD_KEY p_key, String const & p_char )
	{
		std::shared_ptr< Control > l_control = m_activeControl.lock();

		if ( l_control )
		{
			l_control->PushEvent( KeyboardEvent( eKEYBOARD_EVENT_CHAR, p_key, p_char, m_keyboard.m_ctrl, m_keyboard.m_alt, m_keyboard.m_shift ) );
		}
	}

	void ControlsManager::Create( std::shared_ptr< ButtonCtrl > p_control )
	{
		AddControl( p_control );
		std::shared_ptr< GeneratorBase > l_generator = m_generator.lock();

		if ( l_generator->GetButtonImage() )
		{
			std::shared_ptr< gl::Texture > l_texture = l_generator->GetGpuStep()->GetButtonTexture();
			p_control->SetBackgroundTexture( l_texture );
			p_control->SetForegroundTexture( l_texture );
			uint32_t l_border = std::min( p_control->GetSize().x() / 3, p_control->GetSize().y() / 3 );
			p_control->SetBackgroundBorders( Point4i( l_border, l_border, l_border, l_border ) );
		}

		if ( l_generator->GetButtonMouseOverImage() )
		{
			std::shared_ptr< gl::Texture > l_texture = l_generator->GetGpuStep()->GetButtonMouseOverTexture();
			p_control->SetMouseOverBackgroundTexture( l_texture );
			p_control->SetMouseOverForegroundTexture( l_texture );
			uint32_t l_border = std::min( p_control->GetSize().x() / 3, p_control->GetSize().y() / 3 );
			p_control->SetBackgroundBorders( Point4i( l_border, l_border, l_border, l_border ) );
		}

		p_control->Create( shared_from_this(), m_overlayManager.lock() );
	}

	void ControlsManager::Create( std::shared_ptr< ComboBoxCtrl > p_control )
	{
		AddControl( p_control );
		p_control->Create( shared_from_this(), m_overlayManager.lock() );
	}

	void ControlsManager::Create( std::shared_ptr< EditCtrl > p_control )
	{
		AddControl( p_control );
		p_control->Create( shared_from_this(), m_overlayManager.lock() );
	}

	void ControlsManager::Create( std::shared_ptr< ListBoxCtrl > p_control )
	{
		AddControl( p_control );
		p_control->Create( shared_from_this(), m_overlayManager.lock() );
	}

	void ControlsManager::Create( std::shared_ptr< SliderCtrl > p_control )
	{
		AddControl( p_control );
		p_control->Create( shared_from_this(), m_overlayManager.lock() );
	}

	void ControlsManager::Create( std::shared_ptr< StaticCtrl > p_control )
	{
		p_control->Create( shared_from_this(), m_overlayManager.lock() );
		AddControl( p_control );
	}

	void ControlsManager::AddControl( std::shared_ptr< Control > p_control )
	{
		std::unique_lock< std::mutex > l_lock( m_mutexControls );

		if ( std::find( std::begin( m_controls ), std::end( m_controls ), p_control ) != std::end( m_controls ) )
		{
			throw std::runtime_error( _( "This control already exists in the manager" ) );
		}

		if ( m_controlsById.find( p_control->GetId() ) != m_controlsById.end() )
		{
			throw std::runtime_error( _( "A control with this ID already exists in the manager" ) );
		}

		m_controls.push_back( p_control );
		m_controlsById.insert( std::make_pair( p_control->GetId(), p_control ) );
		m_changed = true;
	}

	void ControlsManager::RemoveControl( uint32_t p_id )
	{
		std::unique_lock< std::mutex > l_lock( m_mutexControls );
		auto l_it = m_controlsById.find( p_id );

		if ( l_it == m_controlsById.end() )
		{
			throw std::runtime_error( _( "This control does not exist in the manager" ) );
		}

		m_controls.erase( std::find( std::begin( m_controls ), std::end( m_controls ), l_it->second.lock() ) );
		m_controlsById.erase( l_it );
		m_changed = true;
	}

	std::shared_ptr< Control > ControlsManager::GetControl( uint32_t p_id )
	{
		std::unique_lock< std::mutex > l_lock( m_mutexControls );
		auto l_it = m_controlsById.find( p_id );

		if ( l_it == m_controlsById.end() )
		{
			throw std::runtime_error( _( "This control does not exist in the manager" ) );
		}

		return l_it->second.lock();
	}

	std::shared_ptr< Control > ControlsManager::DoGetMouseTargetableControl( Position const & p_position )const
	{
		if ( m_changed )
		{
			DoUpdate();
		}

		std::unique_lock< std::mutex > l_lock( m_mutexControls );
		std::shared_ptr< Control > l_return;
		auto && l_it = m_controlsByZIndex.rbegin();

		while ( !l_return && l_it != m_controlsByZIndex.rend() )
		{
			std::shared_ptr< Control > l_control = *l_it;

			if ( l_control->CatchesMouseEvents()
					&& l_control->GetAbsolutePosition().x() <= p_position.x()
					&& l_control->GetAbsolutePosition().x() + int32_t( l_control->GetSize().x() ) > p_position.x()
					&& l_control->GetAbsolutePosition().y() <= p_position.y()
					&& l_control->GetAbsolutePosition().y() + int32_t( l_control->GetSize().y() ) > p_position.y()
			   )
			{
				l_return = l_control;
			}

			++l_it;
		}

		return l_return;
	}

	void ControlsManager::DoUpdate()const
	{
		m_controlsByZIndex = DoGetControls();

		std::unique_lock< std::mutex > l_lock( m_mutexControls );
		std::sort( m_controlsByZIndex.begin(), m_controlsByZIndex.end(), []( std::shared_ptr< Control > p_a, std::shared_ptr< Control > p_b )
		{
			uint64_t l_a = p_a->GetBackground()->GetZIndex() + p_a->GetBackground()->GetLevel() * 1000;
			uint64_t l_b = p_b->GetBackground()->GetZIndex() + p_b->GetBackground()->GetLevel() * 1000;
			return l_a < l_b;
		} );
	}
}
