#include "CtrlButton.h"

#include "BorderPanelOverlay.h"
#include "OverlayManager.h"
#include "TextOverlay.h"

namespace ProceduralTextures
{
	ButtonCtrl::ButtonCtrl( String const & p_caption, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style, bool p_visible )
		: Control( eCONTROL_TYPE_BUTTON, p_id, p_position, p_size, p_style, p_visible )
		, m_caption( p_caption )
	{
		m_borders = Point4i( 1, 1, 1, 1 );
		m_backgroundColour = Colour( 0.5, 0.5, 0.5, 1.0 );
		m_foregroundColour = Colour( 1.0, 1.0, 1.0, 1.0 );
		DoSetBackgroundColour( m_backgroundColour );
		DoSetForegroundColour( m_foregroundColour );
		EventHandler::Connect( eMOUSE_EVENT_MOUSE_ENTER, std::bind( &ButtonCtrl::OnMouseEnter, this, std::placeholders::_1 ) );
		EventHandler::Connect( eMOUSE_EVENT_MOUSE_LEAVE, std::bind( &ButtonCtrl::OnMouseLeave, this, std::placeholders::_1 ) );
		EventHandler::Connect( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, std::bind( &ButtonCtrl::OnMouseLButtonUp, this, std::placeholders::_1 ) );
	}

	ButtonCtrl::~ButtonCtrl()
	{
	}

	void ButtonCtrl::SetCaption( String const & p_value )
	{
		m_caption =  p_value;
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetCaption( p_value );
			l_text.reset();
		}
	}

	void ButtonCtrl::SetMouseOverBackgroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
		m_mouseOverBackgroundTexture = p_texture;
	}

	void ButtonCtrl::SetMouseOverForegroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
		m_mouseOverForegroundTexture = p_texture;
	}

	void ButtonCtrl::DoCreate( std::shared_ptr< OverlayManager > p_manager )
	{
		m_background.lock()->SetBordersPosition( eBORDER_POSITION_INTERNAL );
		std::shared_ptr< TextOverlay > l_text = p_manager->CreateText( _T( "T_CtrlButton_" ) + StringUtils::ToString( GetId() ), Position(), GetSize(), Material( m_foregroundColour ), p_manager->GetFont( DEFAULT_FONT_NAME, DEFAULT_FONT_HEIGHT ), GetBackground() );
		l_text->SetHAlign( eHALIGN_CENTER );
		l_text->SetVAlign( eVALIGN_CENTER );
		l_text->SetCaption( m_caption );
		l_text->SetVisible( IsVisible() );
		m_text = l_text;
	}

	void ButtonCtrl::DoSetPosition( Position const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelPosition( Position() );
			l_text.reset();
		}
	}

	void ButtonCtrl::DoSetSize( Size const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelSize( p_value );
		}
	}

	void ButtonCtrl::DoSetBackgroundColour( Colour const & p_value )
	{
		m_mouseOverBackgroundColour.r = std::min( 1.0f, p_value.r + 0.1f );
		m_mouseOverBackgroundColour.g = std::min( 1.0f, p_value.g + 0.1f );
		m_mouseOverBackgroundColour.b = std::min( 1.0f, p_value.b + 0.1f );
		m_mouseOverBackgroundColour.a = std::min( 1.0f, p_value.a + 0.1f );
	}

	void ButtonCtrl::DoSetForegroundColour( Colour const & p_value )
	{
		m_mouseOverForegroundColour.r = std::max( 0.0f, p_value.r - 0.1f );
		m_mouseOverForegroundColour.g = std::max( 0.0f, p_value.g - 0.1f );
		m_mouseOverForegroundColour.b = std::max( 0.0f, p_value.b - 0.1f );
		m_mouseOverForegroundColour.a = std::max( 0.0f, p_value.a - 0.1f );
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetColour( p_value );
			l_text.reset();
		}
	}

	void ButtonCtrl::DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void ButtonCtrl::DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void ButtonCtrl::DoSetVisible( bool p_visible )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetVisible( p_visible );
		}
	}

	void ButtonCtrl::OnMouseEnter( MouseEvent const & p_event )
	{
		m_text.lock()->SetColour( m_mouseOverForegroundColour );
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			if ( m_backgroundTexture.expired() )
			{
				l_panel->SetColour( m_mouseOverBackgroundColour );
				l_panel->SetBordersColour( m_mouseOverForegroundColour );
			}
			else if ( !m_mouseOverBackgroundTexture.expired() )
			{
				l_panel->SetTexture( m_mouseOverBackgroundTexture.lock() );

				if ( !m_mouseOverForegroundTexture.expired() )
				{
					l_panel->SetBordersTexture( m_mouseOverForegroundTexture.lock() );
				}
				else
				{
					l_panel->SetBordersTexture( m_mouseOverBackgroundTexture.lock() );
				}
			}

			l_panel.reset();
		}
	}

	void ButtonCtrl::OnMouseLeave( MouseEvent const & p_event )
	{
		m_text.lock()->SetColour( m_foregroundColour );
		std::shared_ptr< BorderPanelOverlay > l_panel = m_background.lock();

		if ( l_panel )
		{
			if ( m_backgroundTexture.expired() )
			{
				l_panel->SetColour( m_backgroundColour );
				l_panel->SetBordersColour( m_foregroundColour );
			}
			else
			{
				l_panel->SetTexture( m_backgroundTexture.lock() );

				if ( !m_foregroundTexture.expired() )
				{
					l_panel->SetBordersTexture( m_foregroundTexture.lock() );
				}
				else
				{
					l_panel->SetBordersTexture( m_backgroundTexture.lock() );
				}
			}

			l_panel.reset();
		}
	}

	void ButtonCtrl::OnMouseLButtonUp( MouseEvent const & p_event )
	{
		if ( p_event.GetButton() == eMOUSE_BUTTON_LEFT )
		{
			m_signals[eBUTTON_EVENT_CLICKED]();
		}
	}
}
