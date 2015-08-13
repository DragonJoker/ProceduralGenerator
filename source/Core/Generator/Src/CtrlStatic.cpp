#include "CtrlStatic.h"

#include "BorderPanelOverlay.h"
#include "OverlayManager.h"
#include "TextOverlay.h"

namespace ProceduralTextures
{
	int StaticCtrl::m_count = 0xFF000000;

	StaticCtrl::StaticCtrl( std::shared_ptr< Control > p_parent, String const & p_caption, Position const & p_position, Size const & p_size, uint32_t p_style, bool p_visible )
		: Control( eCONTROL_TYPE_STATIC, p_parent, m_count++, p_position, p_size, p_style, p_visible )
		, m_caption( p_caption )
	{
		m_borders = Point4i();
		m_backgroundColour = Colour();
		m_foregroundColour = Colour( 1.0, 1.0, 1.0, 1.0 );
	}

	StaticCtrl::~StaticCtrl()
	{
	}

	void StaticCtrl::SetCaption( String const & p_value )
	{
		m_caption = p_value;
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetCaption( p_value );
			l_text.reset();
		}
	}

	void StaticCtrl::DoCreate( std::shared_ptr< OverlayManager > p_manager )
	{
		std::shared_ptr< TextOverlay > l_text = p_manager->CreateText( _T( "T_CtrlStatic_" ) + StringUtils::ToString( GetId() ), Position(), GetSize(), Material( m_foregroundColour ), p_manager->GetFont( DEFAULT_FONT_NAME, DEFAULT_FONT_HEIGHT ), GetBackground() );

		if ( m_style & eSTATIC_STYLE_HALIGN_CENTER )
		{
			l_text->SetHAlign( eHALIGN_CENTER );
		}
		else if ( m_style & eSTATIC_STYLE_HALIGN_RIGHT )
		{
			l_text->SetHAlign( eHALIGN_RIGHT );
		}
		else
		{
			l_text->SetHAlign( eHALIGN_LEFT );
		}

		if ( m_style & eSTATIC_STYLE_VALIGN_CENTER )
		{
			l_text->SetVAlign( eVALIGN_CENTER );
		}
		else if ( m_style & eSTATIC_STYLE_VALIGN_BOTTOM )
		{
			l_text->SetVAlign( eVALIGN_BOTTOM );
		}
		else
		{
			l_text->SetVAlign( eVALIGN_TOP );
		}

		l_text->SetCaption( m_caption );
		l_text->SetVisible( m_visible );
		m_text = l_text;
	}

	void StaticCtrl::DoSetPosition( Position const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelPosition( Position() );
		}
	}

	void StaticCtrl::DoSetSize( Size const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelSize( p_value );
		}
	}

	void StaticCtrl::DoSetBackgroundColour( Colour const & p_value )
	{
	}

	void StaticCtrl::DoSetForegroundColour( Colour const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetColour( p_value );
		}
	}

	void StaticCtrl::DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void StaticCtrl::DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void StaticCtrl::DoSetVisible( bool p_visible )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetVisible( p_visible );
		}
	}
}
