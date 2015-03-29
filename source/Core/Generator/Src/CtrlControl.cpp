#include "CtrlControl.h"

#include "BorderPanelOverlay.h"
#include "OverlayManager.h"
#include "Translator.h"

namespace ProceduralTextures
{
	Control::Control( eCONTROL_TYPE p_type, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style, bool p_visible )
		: EventHandler< Control >( p_type != eCONTROL_TYPE_STATIC )
		, m_type( p_type )
		, m_id( p_id )
		, m_position( p_position )
		, m_size( p_size )
		, m_style( p_style )
		, m_visible( p_visible )
		, m_borders( 0, 0, 0, 0 )
		, m_backgroundColour( 0.0, 0.0, 0.0, 0.0 )
		, m_foregroundColour( 1.0, 1.0, 1.0, 1.0 )
		, m_cursor( eMOUSE_CURSOR_HAND )
	{
	}

	Control::~Control()
	{
	}

	void Control::Create( std::shared_ptr< ControlsManager > p_ctrlManager, std::shared_ptr< OverlayManager > p_ovManager, std::shared_ptr< Control > p_parent )
	{
		m_parent = p_parent;
		m_ovManager = p_ovManager;
		m_ctrlManager = p_ctrlManager;
		std::shared_ptr< Overlay > l_parentOv;

		if ( p_parent )
		{
			l_parentOv = p_parent->GetBackground();
		}

		std::shared_ptr< BorderPanelOverlay > l_panel;
		Material l_background;
		Material l_foreground;

		if ( !m_backgroundTexture.expired() )
		{
			l_background = Material( m_backgroundTexture.lock() );
		}
		else
		{
			l_background = Material( m_backgroundColour );
		}

		if ( !m_foregroundTexture.expired() )
		{
			l_foreground = Material( m_foregroundTexture.lock() );
		}
		else
		{
			l_foreground = Material( m_foregroundColour );
		}

		l_panel = p_ovManager->CreateBorderPanel( _T( "BP_CtrlControl_" ) + StringUtils::ToString( GetId() ), GetPosition(), GetSize(), l_background, m_borders, l_foreground, l_parentOv );
		l_panel->SetVisible( IsVisible() );
		l_panel->SetBordersPosition( eBORDER_POSITION_INTERNAL );
		m_background = l_panel;
		DoCreate( p_ovManager );
	}

	void Control::SetBackgroundBorders( Point4i const & p_value )
	{
		m_borders = p_value;
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			l_panel->SetBordersPixelSize( p_value );
			l_panel.reset();
		}
	}

	void Control::SetPosition( Position const & p_value )
	{
		m_position = p_value;
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			l_panel->SetPixelPosition( p_value );
			l_panel.reset();
		}

		DoSetPosition( p_value );
	}

	Position Control::GetAbsolutePosition()const
	{
		std::shared_ptr< Control > l_parent = GetParent();
		Position l_return = m_position;

		if ( l_parent )
		{
			l_return += l_parent->GetAbsolutePosition();
		}

		return l_return;
	}

	void Control::SetSize( Size const & p_value )
	{
		m_size = p_value;
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			l_panel->SetPixelSize( p_value );
			l_panel.reset();
		}

		DoSetSize( p_value );
	}

	void Control::SetBackgroundColour( Colour const & p_value )
	{
		m_backgroundColour = p_value;
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			l_panel->SetColour( p_value );
			l_panel.reset();
		}

		DoSetBackgroundColour( p_value );
	}

	void Control::SetForegroundColour( Colour const & p_value )
	{
		m_foregroundColour = p_value;
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			l_panel->SetBordersColour( p_value );
			l_panel.reset();
		}

		DoSetForegroundColour( p_value );
	}

	void Control::SetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
		m_backgroundTexture = p_texture;
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			l_panel->SetTexture( p_texture );
			l_panel.reset();
		}

		DoSetBackgroundTexture( p_texture );
	}

	void Control::SetForegroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
		m_foregroundTexture = p_texture;
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			l_panel->SetBordersTexture( p_texture );
			l_panel.reset();
		}

		DoSetForegroundTexture( p_texture );
	}

	void Control::SetVisible( bool p_value )
	{
		m_visible = p_value;
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			l_panel->SetVisible( p_value );
			l_panel.reset();
		}

		for ( auto && l_child : m_childs )
		{
			l_child.lock()->SetVisible( p_value );
		}

		DoSetVisible( p_value );
	}

	bool Control::IsVisible()const
	{
		bool l_visible = m_visible;
		std::shared_ptr< Control > l_parent = GetParent();

		if ( l_visible && l_parent )
		{
			l_visible = l_parent->IsVisible();
			l_parent.reset();
		}

		return l_visible;
	}

	std::shared_ptr< Control > Control::GetChildControl( uint32_t p_id )
	{
		auto l_it = std::find_if( std::begin( m_childs ), std::end( m_childs ), [&p_id]( std::weak_ptr< Control > p_ctrl )
		{
			return p_ctrl.expired() ? false : p_ctrl.lock()->GetId() == p_id;
		} );

		if ( l_it == m_childs.end() )
		{
			throw std::runtime_error( _( "This control does not exist in my childs" ) );
		}

		return l_it->lock();
	}
}
