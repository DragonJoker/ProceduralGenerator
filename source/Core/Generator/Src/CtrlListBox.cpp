#include "CtrlListBox.h"

#include "BorderPanelOverlay.h"
#include "ControlsManager.h"
#include "OverlayManager.h"
#include "CtrlStatic.h"
#include "TextOverlay.h"

namespace ProceduralTextures
{
	ListBoxCtrl::ListBoxCtrl( std::shared_ptr< Control > p_parent, StringArray const & p_values, int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style, bool p_visible )
		: Control( eCONTROL_TYPE_LIST, p_parent, p_id, p_position, p_size, p_style, p_visible )
		, m_values( p_values )
		, m_selected( p_selected )
	{
	}

	ListBoxCtrl::~ListBoxCtrl()
	{
	}

	void ListBoxCtrl::SetSelectedBackgroundColour( Colour const & p_value )
	{
		m_selectedBackgroundColour = p_value;
		int i = 0;

		for ( auto && l_item : m_items )
		{
			if ( i++ == m_selected )
			{
				l_item->SetBackgroundColour( p_value );
			}
		}
	}

	void ListBoxCtrl::SetSelectedForegroundColour( Colour const & p_value )
	{
		m_selectedForegroundColour = p_value;
		int i = 0;

		for ( auto && l_item : m_items )
		{
			if ( i++ == m_selected )
			{
				l_item->SetForegroundColour( p_value );
			}
		}
	}

	void ListBoxCtrl::AppendItem( String const & p_value )
	{
		m_values.push_back( p_value );
		DoCreateItem( p_value );
		DoUpdateItems();
	}

	void ListBoxCtrl::RemoveItem( int p_value )
	{
		if ( uint32_t( p_value ) < m_values.size() && p_value >= 0 )
		{
			m_values.erase( m_values.begin() + p_value );

			if ( uint32_t( p_value ) < m_items.size() )
			{
				m_items.erase( m_items.begin() + p_value );
				DoUpdateItems();

				if ( p_value <= m_selected )
				{
					m_selectedItem.reset();
					m_selected = -1;
				}
			}
		}
	}

	void ListBoxCtrl::SetItemText( int p_index, String const & p_text )
	{
		if ( uint32_t( p_index ) < m_values.size() && p_index >= 0 )
		{
			m_values[p_index] = p_text;

			if ( uint32_t( p_index ) < m_items.size() )
			{
				std::shared_ptr< StaticCtrl > l_item = m_items[p_index];

				if ( l_item )
				{
					l_item->SetCaption( p_text );
				}
			}
		}
	}

	String ListBoxCtrl::GetItemText( int p_index )
	{
		String l_return;

		if ( uint32_t( p_index ) < m_values.size() && p_index >= 0 )
		{
			l_return = m_values[p_index];
		}

		return l_return;
	}

	void ListBoxCtrl::Clear()
	{
		m_values.clear();
		m_items.clear();
		m_selectedItem.reset();
		m_selected = -1;
	}

	void ListBoxCtrl::SetSelected( int p_value )
	{
		if ( m_selected >= 0 && uint32_t( m_selected ) < m_items.size() )
		{
			std::shared_ptr< StaticCtrl > l_item = m_items[m_selected];

			if ( l_item )
			{
				l_item->SetBackgroundColour( m_backgroundColourNormal );
				l_item->SetForegroundColour( GetForegroundColour() );
				m_selectedItem.reset();
			}
		}

		m_selected = p_value;

		if ( m_selected >= 0 && uint32_t( m_selected ) < m_items.size() )
		{
			std::shared_ptr< StaticCtrl > l_item = m_items[m_selected];

			if ( l_item )
			{
				l_item->SetBackgroundColour( m_selectedBackgroundColour );
				l_item->SetForegroundColour( m_selectedForegroundColour );
				m_selectedItem = l_item;
			}
		}
	}

	void ListBoxCtrl::DoUpdateItems()
	{
		Position l_position;

		for ( auto && l_item : m_items )
		{
			l_item->SetPosition( l_position );
			l_item->SetSize( Size( GetSize().x(), DEFAULT_HEIGHT ) );
			l_position.y() += DEFAULT_HEIGHT;
		}

		std::shared_ptr< BorderPanelOverlay > l_background = GetBackground();

		if ( l_background )
		{
			l_background->SetPixelSize( Size( GetSize().x(), m_items.size() * DEFAULT_HEIGHT ) );
		}
	}

	void ListBoxCtrl::DoCreateItem( String const & p_value )
	{
		std::shared_ptr< StaticCtrl > l_static = std::make_shared< StaticCtrl >( shared_from_this(), p_value, Position(), Size( GetSize().x(), DEFAULT_HEIGHT ), eSTATIC_STYLE_VALIGN_CENTER );
		l_static->SetBackgroundColour( m_backgroundColourNormal );
		l_static->SetForegroundColour( GetForegroundColour() );
		l_static->SetVisible( DoIsVisible() );
		l_static->SetCatchesMouseEvents( true );
		l_static->ConnectNC( eMOUSE_EVENT_MOUSE_ENTER, std::bind( &ListBoxCtrl::OnItemMouseEnter, this, std::placeholders::_1, std::placeholders::_2 ) );
		l_static->ConnectNC( eMOUSE_EVENT_MOUSE_LEAVE, std::bind( &ListBoxCtrl::OnItemMouseLeave, this, std::placeholders::_1, std::placeholders::_2 ) );
		l_static->ConnectNC( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, std::bind( &ListBoxCtrl::OnItemMouseLButtonUp, this, std::placeholders::_1, std::placeholders::_2 ) );
		l_static->ConnectNC( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &ListBoxCtrl::OnItemKeyDown, this, std::placeholders::_1, std::placeholders::_2 ) );
		DoGetCtrlManager()->Create( l_static );
		m_items.push_back( l_static );
	}

	void ListBoxCtrl::DoCreate( std::shared_ptr< OverlayManager > p_manager )
	{
		SetBackgroundBorders( Point4i( 1, 1, 1, 1 ) );
		SetBackgroundColour( Colour( 1.0, 1.0, 1.0, 1.0 ) );
		SetForegroundColour( Colour( 0.0, 0.0, 0.0, 1.0 ) );
		m_selectedBackgroundColour = Colour( 0.0, 0.0, 0.5, 1.0 );
		m_selectedForegroundColour = Colour( 1.0, 1.0, 1.0, 1.0 );
		SetSize( Size( GetSize().x(), m_values.size() * DEFAULT_HEIGHT ) );
		EventHandler::Connect( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &ListBoxCtrl::OnKeyDown, this, std::placeholders::_1 ) );

		for ( auto && l_value : m_values )
		{
			DoCreateItem( l_value );
		}

		DoUpdateItems();
		SetSelected( m_selected );
	}

	void ListBoxCtrl::DoSetPosition( Position const & p_value )
	{
		DoUpdateItems();
	}

	void ListBoxCtrl::DoSetSize( Size const & p_value )
	{
		DoUpdateItems();
	}

	void ListBoxCtrl::DoSetBackgroundColour( Colour const & p_value )
	{
		int i = 0;
		Colour l_colour = GetBackgroundColour();
		m_backgroundColourNormal = l_colour;
		DoGetBackgroundColour().a = 0.0;

		for ( auto && l_item : m_items )
		{
			if ( i++ != m_selected )
			{
				l_item->SetBackgroundColour( p_value );
			}
		}

		m_backgroundColourHighlight.r = std::max( 0.0f, p_value.r / 2.0f );
		m_backgroundColourHighlight.g = std::max( 0.0f, p_value.g / 2.0f );
		m_backgroundColourHighlight.b = std::max( 0.0f, p_value.b / 2.0f );
		m_backgroundColourHighlight.a = 1.0;
	}

	void ListBoxCtrl::DoSetForegroundColour( Colour const & p_value )
	{
		int i = 0;

		for ( auto && l_item : m_items )
		{
			if ( i++ != m_selected )
			{
				l_item->SetForegroundColour( p_value );
			}
		}
	}

	void ListBoxCtrl::DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void ListBoxCtrl::DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void ListBoxCtrl::DoSetVisible( bool p_visible )
	{
		for ( auto && l_item : m_items )
		{
			l_item->SetVisible( p_visible );
		}
	}

	void ListBoxCtrl::OnItemMouseEnter( std::shared_ptr< Control > p_control, MouseEvent const & p_event )
	{
		p_control->SetBackgroundColour( m_backgroundColourHighlight );
	}

	void ListBoxCtrl::OnItemMouseLeave( std::shared_ptr< Control > p_control, MouseEvent const & p_event )
	{
		if ( m_selectedItem.lock() == p_control )
		{
			p_control->SetBackgroundColour( m_selectedBackgroundColour );
		}
		else
		{
			p_control->SetBackgroundColour( m_backgroundColourNormal );
		}
	}

	void ListBoxCtrl::OnItemMouseLButtonUp( std::shared_ptr< Control > p_control, MouseEvent const & p_event )
	{
		if ( p_event.GetButton() == eMOUSE_BUTTON_LEFT )
		{
			int l_index = -1;

			if ( m_selectedItem.lock() != p_control )
			{
				auto && l_it = m_items.begin();
				int i = 0;

				while ( l_index == -1 && l_it != m_items.end() )
				{
					if ( *l_it == p_control )
					{
						l_index = i;
					}

					++l_it;
					++i;
				}

				SetSelected( l_index );
				m_signals[eLISTBOX_EVENT_SELECTED]( m_selected );
			}
			else
			{
				l_index = m_selected;
			}
		}
	}

	void ListBoxCtrl::OnKeyDown( KeyboardEvent const & p_event )
	{
		if ( m_selected != -1 )
		{
			bool l_changed = false;
			int l_index = m_selected;

			if ( p_event.GetKey() == eKEY_UP )
			{
				l_index--;
				l_changed = true;
			}
			else if ( p_event.GetKey() == eKEY_DOWN )
			{
				l_index++;
				l_changed = true;
			}

			if ( l_changed )
			{
				l_index = std::max( 0, std::min( l_index, int( m_items.size() - 1 ) ) );
				SetSelected( l_index );
				m_signals[eLISTBOX_EVENT_SELECTED]( l_index );
			}
		}
	}

	void ListBoxCtrl::OnItemKeyDown( std::shared_ptr< Control > p_control, KeyboardEvent const & p_event )
	{
		OnKeyDown( p_event );
	}
}
