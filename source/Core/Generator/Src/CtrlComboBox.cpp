#include "CtrlComboBox.h"

#include "BorderPanelOverlay.h"
#include "ControlsManager.h"
#include "CtrlButton.h"
#include "CtrlListBox.h"
#include "OverlayManager.h"
#include "TextOverlay.h"

namespace ProceduralTextures
{
	ComboBoxCtrl::ComboBoxCtrl( std::shared_ptr< Control > p_parent, StringArray const & p_values, int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style, bool p_visible )
		: Control( eCONTROL_TYPE_COMBO, p_parent, p_id, p_position, p_size, p_style, p_visible )
		, m_values( p_values )
		, m_selected( p_selected )
	{
	}

	ComboBoxCtrl::~ComboBoxCtrl()
	{
	}

	void ComboBoxCtrl::AppendItem( String  const & p_value )
	{
		m_choices->AppendItem( p_value );
	}

	void ComboBoxCtrl::RemoveItem( int p_value )
	{
		m_choices->RemoveItem( p_value );
	}

	void ComboBoxCtrl::SetItemText( int p_index, String const & p_text )
	{
		return m_choices->SetItemText( p_index, p_text );
	}

	void ComboBoxCtrl::Clear()
	{
		return m_choices->Clear();
	}

	void ComboBoxCtrl::SetSelected( int p_value )
	{
		return m_choices->SetSelected( p_value );
	}

	StringArray const & ComboBoxCtrl::GetItems()const
	{
		return m_choices->GetItems();
	}

	uint32_t ComboBoxCtrl::GetItemCount()const
	{
		return m_choices->GetItemCount();
	}

	int ComboBoxCtrl::GetSelected()const
	{
		return m_choices->GetSelected();
	}

	void ComboBoxCtrl::DoCreate( std::shared_ptr< OverlayManager > p_manager )
	{
		m_borders = Point4i( 1, 1, 1, 1 );
		m_backgroundColour = Colour( 1.0, 1.0, 1.0, 1.0 );
		m_foregroundColour = Colour( 0.0, 0.0, 0.0, 1.0 );

		m_expand = std::make_shared< ButtonCtrl >( shared_from_this(), _T( "+" ), GetId() << 12, Position( GetSize().x() - GetSize().y(), 0 ), Size( GetSize().y(), GetSize().y() ) );
		m_expand->SetForegroundColour( m_foregroundColour );
		m_expand->SetVisible( IsVisible() );
		m_expand->Connect( eBUTTON_EVENT_CLICKED, std::bind( &ComboBoxCtrl::DoSwitchExpand, this ) );

		m_choices = std::make_shared< ListBoxCtrl >( shared_from_this(), m_values, m_selected, ( GetId() << 12 ) + 1, Position( 0, GetSize().y() ), Size( GetSize().x() - GetSize().y(), -1 ), 0, false );
		m_choices->SetBackgroundColour( m_backgroundColour );
		m_choices->SetForegroundColour( m_foregroundColour );
		m_choices->Connect( eLISTBOX_EVENT_SELECTED, std::bind( &ComboBoxCtrl::OnSelected, this, std::placeholders::_1 ) );

		EventHandler::Connect( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &ComboBoxCtrl::OnKeyDown, this, std::placeholders::_1 ) );
		EventHandler::ConnectNC( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &ComboBoxCtrl::OnNcKeyDown, this, std::placeholders::_1, std::placeholders::_2 ) );

		std::shared_ptr< TextOverlay > l_text = p_manager->CreateText( _T( "T_CtrlCombo_" ) + StringUtils::ToString( GetId() ), Position(), GetSize() - Size( GetSize().y(), 0 ), Material( m_foregroundColour ), p_manager->GetFont( DEFAULT_FONT_NAME, DEFAULT_FONT_HEIGHT ), GetBackground() );
		l_text->SetVAlign( eVALIGN_CENTER );
		int l_sel = GetSelected();

		if ( l_sel >= 0 && uint32_t( l_sel ) < GetItemCount() )
		{
			l_text->SetCaption( GetItems()[l_sel] );
		}

		m_text = l_text;
		std::shared_ptr< ControlsManager > l_manager = m_ctrlManager.lock();

		if ( l_manager )
		{
			l_manager->Create( m_expand );
			l_manager->Create( m_choices );
		}
	}

	void ComboBoxCtrl::DoSetPosition( Position const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelPosition( p_value );
			l_text.reset();
		}

		m_expand->SetPosition( Position( GetSize().x() - GetSize().y(), 0 ) );
		m_choices->SetPosition( Position( 0, GetSize().y() ) );
	}

	void ComboBoxCtrl::DoSetSize( Size const & p_value )
	{
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetPixelSize( p_value );
			l_text.reset();
		}

		m_expand->SetSize( Size( p_value.y(), p_value.y() ) );
		m_choices->SetSize( Size( p_value.x() - p_value.y(), -1 ) );
		m_expand->SetPosition( Position( p_value.x() - p_value.y(), 0 ) );
		m_choices->SetPosition( Position( 0, p_value.y() ) );
	}

	void ComboBoxCtrl::DoSetBackgroundColour( Colour const & p_value )
	{
		m_choices->SetBackgroundColour( m_backgroundColour );
	}

	void ComboBoxCtrl::DoSetForegroundColour( Colour const & p_value )
	{
		m_expand->SetForegroundColour( m_foregroundColour );
		m_choices->SetForegroundColour( m_foregroundColour );
		std::shared_ptr< TextOverlay > l_text = m_text.lock();

		if ( l_text )
		{
			l_text->SetColour( p_value );
			l_text.reset();
		}
	}

	void ComboBoxCtrl::DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void ComboBoxCtrl::DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	bool ComboBoxCtrl::DoCatchesMouseEvents()const
	{
		return false;
	}

	void ComboBoxCtrl::OnKeyDown( KeyboardEvent const & p_event )
	{
		if ( GetSelected() != -1 )
		{
			bool l_changed = false;
			int l_index = GetSelected();

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
				l_index = std::max( 0, std::min( l_index, int( GetItemCount() - 1 ) ) );
				SetSelected( l_index );
				OnSelected( l_index );
			}
		}
	}

	void ComboBoxCtrl::OnNcKeyDown( std::shared_ptr< Control > p_control, KeyboardEvent const & p_event )
	{
		OnKeyDown( p_event );
	}

	void ComboBoxCtrl::DoSetVisible( bool p_visible )
	{
		m_expand->SetVisible( p_visible );
		m_choices->Hide();
	}

	void ComboBoxCtrl::DoSwitchExpand()
	{
		m_choices->SetVisible( !m_choices->IsVisible() );
	}

	void ComboBoxCtrl::OnSelected( int p_selected )
	{
		if ( p_selected >= 0 )
		{
			DoSwitchExpand();
			std::shared_ptr< TextOverlay > l_text = m_text.lock();

			if ( l_text )
			{
				l_text->SetCaption( m_choices->GetItemText( p_selected ) );
			}
		}

		m_signals[eCOMBOBOX_EVENT_SELECTED]( p_selected );
	}
}
