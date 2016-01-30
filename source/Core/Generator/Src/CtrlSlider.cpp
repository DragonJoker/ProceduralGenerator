#include "CtrlSlider.h"

#include "BorderPanelOverlay.h"
#include "ControlsManager.h"
#include "CtrlStatic.h"
#include "OverlayManager.h"

namespace ProceduralTextures
{
	SliderCtrl::SliderCtrl( std::shared_ptr< Control > p_parent, Range const & p_range, int p_value, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style, bool p_visible )
		: Control( eCONTROL_TYPE_SLIDER, p_parent, p_id, p_position, p_size, p_style, p_visible )
		, m_range( p_range )
		, m_value( p_value )
		, m_scrolling( false )
	{
		SetBackgroundBorders( Point4i() );
		SetBackgroundColour( Colour() );
		SetForegroundColour( Colour( 0.0, 0.0, 0.0, 1.0 ) );
		EventHandler::Connect( eMOUSE_EVENT_MOUSE_MOVE, std::bind( &SliderCtrl::OnMouseMove, this, std::placeholders::_1 ) );
		EventHandler::Connect( eMOUSE_EVENT_MOUSE_LEAVE, std::bind( &SliderCtrl::OnMouseLeave, this, std::placeholders::_1 ) );
		EventHandler::Connect( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, std::bind( &SliderCtrl::OnMouseLButtonUp, this, std::placeholders::_1 ) );
		EventHandler::Connect( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &SliderCtrl::OnKeyDown, this, std::placeholders::_1 ) );
	}

	SliderCtrl::~SliderCtrl()
	{
	}

	void SliderCtrl::SetRange( Range const & p_value )
	{
		m_range =  p_value;
		DoUpdateLineAndTick();
	}

	void SliderCtrl::SetValue( int32_t p_value )
	{
		m_value =  p_value;
		DoUpdateLineAndTick();
	}

	void SliderCtrl::DoUpdateLineAndTick()
	{
		Size l_lineSize( GetSize() );
		Position l_linePosition;
		Size l_tickSize( GetSize() );
		Position l_tickPosition;

		if ( GetStyle() & eSLIDER_STYLE_VERTICAL )
		{
			l_lineSize.x() = 3;
			l_lineSize.y() -= 4;
			l_linePosition.x() = ( GetSize().x() - 3 ) / 2;
			l_linePosition.y() += 2;
			l_tickSize.x() = ( GetSize().x() / 2 ) + ( GetSize().x() % 2 );
			l_tickSize.y() = 5;
			l_tickPosition.x() = l_tickSize.x() / 2;
			l_tickPosition.y() = int32_t( l_lineSize.y() * double( GetValue() ) / std::abs( GetRange().second - GetRange().first ) );
		}
		else
		{
			l_lineSize.x() -= 4;
			l_lineSize.y() = 3;
			l_linePosition.x() += 2;
			l_linePosition.y() = ( GetSize().y() - 3 ) / 2;
			l_tickSize.x() = 5;
			l_tickSize.y() = ( GetSize().y() / 2 ) + ( GetSize().y() % 2 );
			l_tickPosition.x() = int32_t( l_lineSize.x() * double( GetValue() ) / std::abs( GetRange().second - GetRange().first ) );
			l_tickPosition.y() = l_tickSize.y() / 2;
		}

		std::shared_ptr< StaticCtrl > l_line = m_line.lock();

		if ( l_line )
		{
			l_line->SetPosition( l_linePosition );
			l_line->SetSize( l_lineSize );
			l_line->SetVisible( DoIsVisible() );
		}

		std::shared_ptr< StaticCtrl > l_tick = m_tick.lock();

		if ( l_tick )
		{
			l_tick->SetPosition( l_tickPosition );
			l_tick->SetSize( l_tickSize );
			l_tick->SetVisible( DoIsVisible() );
		}
	}

	void SliderCtrl::DoCreate( std::shared_ptr< OverlayManager > p_manager )
	{
		std::shared_ptr< StaticCtrl > l_line = std::make_shared< StaticCtrl >( shared_from_this(), _T( "" ), Position(), Size() );
		l_line->SetBackgroundColour( Colour( 0.5, 0.5, 0.5, 1.0 ) );
		l_line->SetForegroundColour( GetForegroundColour() );
		l_line->SetBackgroundBorders( Point4i( 1, 1, 1, 1 ) );
		l_line->SetVisible( DoIsVisible() );
		l_line->ConnectNC( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &SliderCtrl::OnNcKeyDown, this, std::placeholders::_1, std::placeholders::_2 ) );
		DoGetCtrlManager()->Create( l_line );
		m_line = l_line;

		std::shared_ptr< StaticCtrl > l_tick = std::make_shared< StaticCtrl >( shared_from_this(), _T( "" ), Position(), Size() );
		l_tick->SetBackgroundColour( Colour( 1.0, 1.0, 1.0, 1.0 ) );
		l_tick->SetForegroundColour( GetForegroundColour() );
		l_tick->SetBackgroundBorders( Point4i( 1, 1, 1, 1 ) );
		l_tick->SetVisible( DoIsVisible() );
		l_tick->SetCatchesMouseEvents( true );
		l_tick->ConnectNC( eMOUSE_EVENT_MOUSE_MOVE, std::bind( &SliderCtrl::OnTickMouseMove, this, std::placeholders::_1, std::placeholders::_2 ) );
		l_tick->ConnectNC( eMOUSE_EVENT_MOUSE_BUTTON_PUSHED, std::bind( &SliderCtrl::OnTickMouseLButtonDown, this, std::placeholders::_1, std::placeholders::_2 ) );
		l_tick->ConnectNC( eMOUSE_EVENT_MOUSE_BUTTON_RELEASED, std::bind( &SliderCtrl::OnTickMouseLButtonUp, this, std::placeholders::_1, std::placeholders::_2 ) );
		l_tick->ConnectNC( eKEYBOARD_EVENT_KEY_PUSHED, std::bind( &SliderCtrl::OnNcKeyDown, this, std::placeholders::_1, std::placeholders::_2 ) );
		DoGetCtrlManager()->Create( l_tick );
		m_tick = l_tick;

		DoUpdateLineAndTick();
	}

	void SliderCtrl::DoSetPosition( Position const & p_value )
	{
		DoUpdateLineAndTick();
	}

	void SliderCtrl::DoSetSize( Size const & p_value )
	{
		DoUpdateLineAndTick();
	}

	void SliderCtrl::DoSetBackgroundColour( Colour const & p_value )
	{
	}

	void SliderCtrl::DoSetForegroundColour( Colour const & p_value )
	{
		std::shared_ptr< StaticCtrl > l_line = m_line.lock();

		if ( l_line )
		{
			l_line->SetForegroundColour( p_value );
			l_line.reset();
		}

		std::shared_ptr< StaticCtrl > l_tick = m_tick.lock();

		if ( l_tick )
		{
			l_tick->SetForegroundColour( p_value );
			l_tick.reset();
		}
	}

	void SliderCtrl::DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void SliderCtrl::DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture )
	{
	}

	void SliderCtrl::DoSetVisible( bool p_value )
	{
		std::shared_ptr< StaticCtrl > l_line = m_line.lock();

		if ( l_line )
		{
			l_line->SetVisible( p_value );
			l_line.reset();
		}

		std::shared_ptr< StaticCtrl > l_tick = m_tick.lock();

		if ( l_tick )
		{
			l_tick->SetVisible( p_value );
			l_tick.reset();
		}
	}

	void SliderCtrl::OnMouseMove( MouseEvent const & p_event )
	{
		if ( m_scrolling )
		{
			Position l_delta = ( p_event.GetPosition() - GetAbsolutePosition() ) - m_mouse;
			m_mouse = p_event.GetPosition() - GetAbsolutePosition();
			DoUpdateTick( l_delta );
			m_signals[eSLIDER_EVENT_THUMBTRACK]( m_value );
		}
	}

	void SliderCtrl::OnMouseLeave( MouseEvent const & p_event )
	{
		if ( m_scrolling
				&& DoGetCtrlManager()->GetFocusedControl() != shared_from_this()
				&& DoGetCtrlManager()->GetFocusedControl() != m_tick.lock()
				&& DoGetCtrlManager()->GetFocusedControl() != m_line.lock()
		   )
		{
			Position l_delta = ( p_event.GetPosition() - GetAbsolutePosition() ) - m_mouse;
			m_mouse = p_event.GetPosition() - GetAbsolutePosition();
			DoUpdateTick( l_delta );
			m_signals[eSLIDER_EVENT_THUMBRELEASE]( m_value );
			m_scrolling = false;
		}
	}

	void SliderCtrl::OnMouseLButtonUp( MouseEvent const & p_event )
	{
		if ( p_event.GetButton() == eMOUSE_BUTTON_LEFT )
		{
			if ( !m_scrolling )
			{
				m_mouse = m_tick.lock()->GetPosition();
			}

			Position l_delta = ( p_event.GetPosition() - GetAbsolutePosition() ) - m_mouse;
			m_mouse = p_event.GetPosition() - GetAbsolutePosition();
			DoUpdateTick( l_delta );
			m_signals[eSLIDER_EVENT_THUMBRELEASE]( m_value );
			m_scrolling = false;
		}
	}

	void SliderCtrl::OnTickMouseMove( std::shared_ptr< Control > p_control, MouseEvent const & p_event )
	{
		OnMouseMove( p_event );
	}

	void SliderCtrl::OnTickMouseLButtonDown( std::shared_ptr< Control > p_control, MouseEvent const & p_event )
	{
		if ( p_event.GetButton() == eMOUSE_BUTTON_LEFT )
		{
			m_scrolling = true;
			m_mouse = p_event.GetPosition() - GetAbsolutePosition();
		}
	}

	void SliderCtrl::OnTickMouseLButtonUp( std::shared_ptr< Control > p_control, MouseEvent const & p_event )
	{
		OnMouseLButtonUp( p_event );
	}

	void SliderCtrl::OnKeyDown( KeyboardEvent const & p_event )
	{
		if ( !m_scrolling )
		{
			if ( GetStyle() & eSLIDER_STYLE_VERTICAL )
			{
				if ( p_event.GetKey() == eKEY_UP )
				{
					DoUpdateTick( Position( 0, -1 ) );
					m_signals[eSLIDER_EVENT_THUMBRELEASE]( m_value );
				}
				else if ( p_event.GetKey() == eKEY_DOWN )
				{
					DoUpdateTick( Position( 0, 1 ) );
					m_signals[eSLIDER_EVENT_THUMBRELEASE]( m_value );
				}
			}
			else
			{
				if ( p_event.GetKey() == eKEY_LEFT )
				{
					DoUpdateTick( Position( -1, 0 ) );
					m_signals[eSLIDER_EVENT_THUMBRELEASE]( m_value );
				}
				else if ( p_event.GetKey() == eKEY_RIGHT )
				{
					DoUpdateTick( Position( 1, 0 ) );
					m_signals[eSLIDER_EVENT_THUMBRELEASE]( m_value );
				}
			}
		}
	}

	void SliderCtrl::OnNcKeyDown( std::shared_ptr< Control > p_control, KeyboardEvent const & p_event )
	{
		OnKeyDown( p_event );
	}

	void SliderCtrl::DoUpdateTick( Position const & p_delta )
	{
		Position l_delta = p_delta;

		if ( GetStyle() & eSLIDER_STYLE_VERTICAL )
		{
			l_delta.x() = 0;
		}
		else
		{
			l_delta.y() = 0;
		}

		std::shared_ptr< StaticCtrl > l_tick = m_tick.lock();

		if ( l_tick )
		{
			Position l_position = l_tick->GetPosition() + l_delta;
			double l_tickValue = 0;
			std::shared_ptr< StaticCtrl > l_line = m_line.lock();

			if ( l_line )
			{
				if ( GetStyle() & eSLIDER_STYLE_VERTICAL )
				{
					l_position.y() = std::min( int32_t( l_line->GetSize().y() ), std::max( 0, l_position.y() ) );
					l_tickValue = ( l_position.y() - l_line->GetPosition().y() ) / double( l_line->GetSize().y() );
				}
				else
				{
					l_position.x() = std::min( int32_t( l_line->GetSize().x() ), std::max( 0, l_position.x() ) );
					l_tickValue = ( l_position.x() - l_line->GetPosition().x() ) / double( l_line->GetSize().x() );
				}
			}
			else
			{
				if ( GetStyle() & eSLIDER_STYLE_VERTICAL )
				{
					l_position.y() = std::min( int32_t( GetSize().y() ), std::max( 0, l_position.y() ) );
					l_tickValue = ( l_position.y() - GetPosition().y() ) / double( GetSize().y() );
				}
				else
				{
					l_position.x() = std::min( int32_t( GetSize().x() ), std::max( 0, l_position.x() ) );
					l_tickValue = ( l_position.x() - GetPosition().x() ) / double( GetSize().x() );
				}
			}

			l_tickValue = std::max( 0.0, std::min( 1.0, l_tickValue ) );
			l_tick->SetPosition( l_position );
			m_value = m_range.first + int32_t( ( m_range.second - m_range.first ) * l_tickValue );
		}
	}
}
