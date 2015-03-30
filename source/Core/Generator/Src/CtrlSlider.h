/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_SLIDER_H___
#define ___GENERATOR_SLIDER_H___

#include "CtrlControl.h"

namespace ProceduralTextures
{
	typedef std::pair< int, int > Range;
	/*!
	\author		Sylvain DOREMUS
	\date		16/02/2015
	\version	2.0.0
	\brief		Slider control
	*/
	class GeneratorAPI SliderCtrl
		: public Control
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_range		The slider min and max values
		 *\param[in]	p_value		The slider initial value
		 *\param[in]	p_id		The ID
		 *\param[in]	p_position	The position
		 *\param[in]	p_size		The size
		 *\param[in]	p_style		The style
		 *\param[in]	p_visible	Initial visibility status
		 */
		SliderCtrl( Range const & p_range, int p_value, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true );
		/**
		 *\brief		Destructor
		 */
		virtual ~SliderCtrl();
		/**
		 *\brief		Sets the range
		 *\param[in]	p_value	The new value
		 */
		void SetRange( Range const & p_value );
		/**
		 *\brief		Sets the caption
		 *\param[in]	p_value	The new value
		 */
		void SetValue( int32_t p_value );
		/**
		 *\brief		Retrieves the range
		 *\return		The value
		 */
		inline Range const & GetRange()const
		{
			return m_range;
		}
		/**
		 *\brief		Retrieves the caption
		 *\return		The value
		 */
		inline int32_t GetValue()const
		{
			return m_value;
		}
		/**
		 *\brief		Connects a function to a slider event
		 *\param[in]	p_event		The event type
		 *\param[in]	p_function	The function
		 *\return		The internal function index, to be able to disconnect it
		 */
		inline uint32_t Connect( eSLIDER_EVENT p_event, std::function< void( int ) > p_function )
		{
			return m_signals[p_event].Connect( p_function );
		}
		/**
		 *\brief		Disconnects a function from a slider event
		 *\param[in]	p_event	The event type
		 *\param[in]	p_index	The function index
		 */
		inline void Disconnect( eSLIDER_EVENT p_event, uint32_t p_index )
		{
			m_signals[p_event].Disconnect( p_index );
		}

	private:
		/**
		 *\brief		Updates the line and tick position and size
		 */
		void DoUpdateLineAndTick();
		/**
		 *\copydoc		ProceduralTextures::Control::DoCreate
		 */
		virtual void DoCreate( std::shared_ptr< OverlayManager > p_manager );
		/**
		 *\copydoc		ProceduralTextures::Control::DoSetPosition
		 */
		virtual void DoSetPosition( Position const & p_value );
		/**
		 *\copydoc		ProceduralTextures::Control::DoSetSize
		 */
		virtual void DoSetSize( Size const & p_value );
		/**
		 *\copydoc		ProceduralTextures::Control::DoSetBackgroundColour
		 */
		virtual void DoSetBackgroundColour( Colour const & p_colour );
		/**
		 *\copydoc		ProceduralTextures::Control::DoSetForegroundColour
		 */
		virtual void DoSetForegroundColour( Colour const & p_colour );
		/**
		 *\copydoc		ProceduralTextures::Control::DoSetBackgroundTexture
		 */
		virtual void DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture );
		/**
		 *\copydoc		ProceduralTextures::Control::DoSetForegroundTexture
		 */
		virtual void DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture );
		/**
		 *\brief		Sets the visibility
		 *\remarks		Used for derived control specific behaviour
		 *\param[in]	p_value	The new value
		 */
		virtual void DoSetVisible( bool p_visible );
		/**
		 *\brief		Event when mouse moves over the control
		 *\param[in]	p_event		The mouse event
		 */
		void OnMouseMove( MouseEvent const & p_event );
		/**
		 *\brief		Event when mouse leaves the control
		 *\param[in]	p_event		The mouse event
		 */
		void OnMouseLeave( MouseEvent const & p_event );
		/**
		 *\brief		Event when mouse left button is released over the control
		 *\param[in]	p_event		The mouse event
		 */
		void OnMouseLButtonUp( MouseEvent const & p_event );
		/**
		 *\brief		Event when mouse moves over the tick control
		 *\param[in]	p_control	The tick control
		 *\param[in]	p_event		The mouse event
		 */
		void OnTickMouseMove( std::shared_ptr< Control > p_control, MouseEvent const & p_event );
		/**
		 *\brief		Event when mouse left button is released over the tick control
		 *\param[in]	p_control	The tick control
		 *\param[in]	p_event		The mouse event
		 */
		void OnTickMouseLButtonDown( std::shared_ptr< Control > p_control, MouseEvent const & p_event );
		/**
		 *\brief		Event when mouse left button is released over the tick control
		 *\param[in]	p_control	The tick control
		 *\param[in]	p_event		The mouse event
		 */
		void OnTickMouseLButtonUp( std::shared_ptr< Control > p_control, MouseEvent const & p_event );
		/**
		 *\brief		Event when a keyboard key is pressed
		 *\param[in]	p_event	The keyboard event
		 */
		void OnKeyDown( KeyboardEvent const & p_event );
		/**
		 *\brief		Event when a keyboard key is pressed on the active tick or line control
		 *\param[in]	p_event	The keyboard event
		 */
		void OnNcKeyDown( std::shared_ptr< Control > p_control, KeyboardEvent const & p_event );
		/**
		 *\brief		Updates the tick position
		 *\param[in]	p_deltat	The position delta
		 */
		void DoUpdateTick( Position const & p_delta );

	private:
		//! The slider range
		Range m_range;
		//! The slider current value
		int32_t m_value;
		//! Tells the tick is moving
		bool m_scrolling;
		//! The previous mouse position
		Position m_mouse;
		//! The panel overlay used to display the line
		std::weak_ptr< StaticCtrl > m_line;
		//! The static used to display the line
		std::weak_ptr< StaticCtrl > m_tick;
		//! The slider events signals
		Signal< std::function< void( int ) > > m_signals[eSLIDER_EVENT_COUNT];
	};
}

#endif
