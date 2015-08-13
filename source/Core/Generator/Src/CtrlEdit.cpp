#include "CtrlEdit.h"

#include "BorderPanelOverlay.h"
#include "OverlayManager.h"
#include "PanelOverlay.h"
#include "TextOverlay.h"

namespace ProceduralTextures
{
	EditCtrl::EditCtrl( std::shared_ptr< Control > p_parent, String const & p_caption, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style, bool p_visible )
		: Control( eCONTROL_TYPE_EDIT, p_parent, p_id, p_position, p_size, p_style, p_visible )
		, m_caption( p_caption )
		, m_caretIt( p_caption.end() )
		, m_active( false )
	{
		m_caretIt = m_caption.end();
		m_cursor = eMOUSE_CURSOR_TEXT;
		m_borders = Point4i( 1, 1, 1, 1 );
		m_backgroundColour = Colour( 1.0, 1.0, 1.0, 1.0 );
		m_foregroundColour = Colour( 0.0, 0.0, 0.0, 1.0 );
		EventHandler::Connect( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, std::bind( &EditCtrl::OnMouseLButtonDown, this, std::placeholders::_1 ) );
		EventHandler::Connect( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, std::bind( &EditCtrl::OnMouseLButtonUp, this, std::placeholders::_1 ) );
		EventHandler::Connect( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &EditCtrl::OnKeyDown, this, std::placeholders::_1 ) );
		EventHandler::Connect( eKEYBOARD_EVENT_KEY_RELEASED, std::bind( &EditCtrl::OnKeyUp, this, std::placeholders::_1 ) );
		EventHandler::Connect( eKEYBOARD_EVENT_CHAR, std::bind( &EditCtrl::OnChar, this, std::placeholders::_1 ) );
		EventHandler::Connect( eCONTROL_EVENT_ACTIVATE, std::bind( &EditCtrl::OnActivate, this, std::placeholders::_1 ) );
		EventHandler::Connect( eCONTROL_EVENT_DEACTIVATE, std::bind( &EditCtrl::OnDeactivate, this, std::placeholders::_1 ) );
	}

	EditCtrl::~EditCtrl()
	{
	}

	void EditCtrl::SetCaption( String const & p_value )
	{
		m_caption = p_value;
		m_caretIt = m_caption.end();
		DoUpdateCaption();
	}

	void EditCtrl::DoCreate( std::shared_ptr< OverlayManager > p_manager )
	{
		std::shared_ptr< TextOverlay > l_text = p_manager->CreateText( _T( "T_CtrlEdit_" ) + StringUtils::ToString( GetId() ), Position(), GetSize(), Material( m_foregroundColour ), p_manager->GetFont( DEFAULT_FONT_NAME, DEFAULT_FONT_HEIGHT ), GetBackground() );
		l_text->SetVAlign( eVALIGN_TOP );
		l_text->SetVisible( m_visible );

		if ( ( m_style & eEDIT_STYLE_MULTILINE ) == eEDIT_STYLE_MULTILINE )
		{
			l_text->SetTextWrappingMode( eTEXT_WRAPPING_MODE_BREAK );
		}

		m_text = l_text;
		DoUpdateCaption();
	}

	void EditCtrl::DoSetPosition( Position const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelPosition( Position() );
			l_text.reset();
		}
	}

	void EditCtrl::DoSetSize( Size const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelSize( p_value );
			l_text.reset();
		}
	}

	void EditCtrl::DoSetBackgroundColour( Colour const & p_value )
	{
	}

	void EditCtrl::DoSetForegroundColour( Colour const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetColour( p_value );
			l_text.reset();
		}
	}

	void EditCtrl::DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void EditCtrl::DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void EditCtrl::DoSetVisible( bool p_visible )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetVisible( p_visible );
			l_text.reset();
		}
	}

	String EditCtrl::DoGetCaptionWithCaret()const
	{
		String l_caption( m_caption.begin(), m_caretIt.internal() );
		l_caption += _T( '|' );

		if ( m_caretIt != m_caption.end() )
		{
			l_caption += String( m_caretIt.internal(), m_caption.end() );
		}

		return l_caption;
	}

	void EditCtrl::OnActivate( ControlEvent const & p_event )
	{
		m_active = true;
		DoUpdateCaption();
	}

	void EditCtrl::OnDeactivate( ControlEvent const & p_event )
	{
		m_active = false;
		DoUpdateCaption();
	}

	void EditCtrl::OnMouseLButtonDown( MouseEvent const & p_event )
	{
	}

	void EditCtrl::OnMouseLButtonUp( MouseEvent const & p_event )
	{
	}

	void EditCtrl::OnChar( KeyboardEvent const & p_event )
	{
		eKEYBOARD_KEY l_code = p_event.GetKey();

		if ( l_code >= 0x20 && l_code <= 255 && l_code != eKEY_DELETE )
		{
			DoAddCharAtCaret( p_event.GetChar() );
			m_signals[eEDIT_EVENT_UPDATED]( m_caption );
		}
	}

	void EditCtrl::OnKeyDown( KeyboardEvent const & p_event )
	{
		if ( !p_event.IsCtrlDown() && !p_event.IsAltDown() )
		{
			eKEYBOARD_KEY l_code = p_event.GetKey();

			if ( l_code == eKEY_BACKSPACE )
			{
				DoDeleteCharBeforeCaret();
				m_signals[eEDIT_EVENT_UPDATED]( m_caption );
			}
			else if ( l_code == eKEY_DELETE )
			{
				DoDeleteCharAtCaret();
				m_signals[eEDIT_EVENT_UPDATED]( m_caption );
			}
			else if ( l_code == eKEY_LEFT && m_caretIt != m_caption.begin() )
			{
				m_caretIt--;
				DoUpdateCaption();
			}
			else if ( l_code == eKEY_RIGHT && m_caretIt != m_caption.end() )
			{
				m_caretIt++;
				DoUpdateCaption();
			}
			else if ( l_code == eKEY_HOME && m_caretIt != m_caption.begin() )
			{
				m_caretIt = m_caption.begin();
				DoUpdateCaption();
			}
			else if ( l_code == eKEY_END && m_caretIt != m_caption.end() )
			{
				m_caretIt = m_caption.end();
				DoUpdateCaption();
			}
		}
	}

	void EditCtrl::OnKeyUp( KeyboardEvent const & p_event )
	{
	}

	void EditCtrl::DoAddCharAtCaret( String const & p_char )
	{
		size_t l_diff = std::distance( StringUtils::Utf8Iterator( m_caption.begin() ), m_caretIt );
		m_caption = String( m_caption.cbegin(), m_caretIt.internal() ) + p_char + String( m_caretIt.internal(), m_caption.cend() );
		m_caretIt = StringUtils::Utf8Iterator( m_caption.begin() ) + l_diff + 1;
		DoUpdateCaption();
	}

	void EditCtrl::DoDeleteCharAtCaret()
	{
		if ( m_caretIt != m_caption.end() )
		{
			size_t l_diff = std::distance( StringUtils::Utf8Iterator( m_caption.begin() ), m_caretIt );
			String l_caption( m_caption.cbegin(), m_caretIt.internal() );
			StringUtils::Utf8Iterator l_it = m_caretIt;

			if ( ++l_it != m_caption.end() )
			{
				l_caption += String( l_it.internal(), m_caption.cend() );
			}

			m_caption = l_caption;
			m_caretIt = StringUtils::Utf8Iterator( m_caption.begin() ) + l_diff;
			DoUpdateCaption();
		}
	}

	void EditCtrl::DoDeleteCharBeforeCaret()
	{
		if ( m_caretIt != m_caption.begin() )
		{
			m_caretIt--;
			size_t l_diff = std::distance( StringUtils::Utf8Iterator( m_caption.begin() ), m_caretIt );
			String l_caption( m_caption.cbegin(), m_caretIt.internal() );
			StringUtils::Utf8Iterator l_it = m_caretIt;

			if ( ++l_it != m_caption.end() )
			{
				l_caption += String( l_it.internal(), m_caption.cend() );
			}

			m_caption = l_caption;
			m_caretIt = StringUtils::Utf8Iterator( m_caption.begin() ) + l_diff;
			DoUpdateCaption();
		}
	}

	void EditCtrl::DoUpdateCaption()
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			if ( m_active )
			{
				l_text->SetCaption( DoGetCaptionWithCaret() );
			}
			else
			{
				l_text->SetCaption( m_caption );
			}
		}
	}
}
