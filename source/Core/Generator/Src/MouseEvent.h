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
#ifndef ___GENERATOR_MOUSE_EVENT_H___
#define ___GENERATOR_MOUSE_EVENT_H___

#include "Event.h"
#include "Position.h"

namespace ProceduralTextures
{
	/*!
	\author 	Sylvain DOREMUS
	\date 		02/03/2015
	\version	2.0.0
	\brief		Structure holding mouse state
	*/
	struct MouseState
	{
		//! The position
		Position m_position;
		//! The wheel position
		Position m_wheel;
		//! The buttons state (true = down)
		bool m_buttons[eMOUSE_BUTTON_COUNT];
		//! The button which had the last change
		eMOUSE_BUTTON m_changed;
	};
	/*!
	\author		Sylvain DOREMUS
	\brief		Description of a mouse event
	*/
	class MouseEvent
		: public Event
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_type		The mouse event type
		 *\param[in]	p_position	The position, of whell or mouse, depending on the evnt type
		 *\param[in]	p_button	The mouse button
		 */
		MouseEvent( eMOUSE_EVENT p_type, Position const & p_position, eMOUSE_BUTTON p_button = eMOUSE_BUTTON_COUNT )
			: Event( eEVENT_TYPE_MOUSE )
			, m_mouseEventType( p_type )
			, m_position( p_position )
			, m_button( p_button )
		{
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~MouseEvent()
		{
		}
		/**
		 *\brief		Retrieves the mouse event type
		 *\return		The type
		 */
		inline eMOUSE_EVENT GetMouseEventType()const
		{
			return m_mouseEventType;
		}
		/**
		 *\brief		Retrieves the mouse button
		 *\return		The button
		 */
		inline eMOUSE_BUTTON GetButton()const
		{
			return m_button;
		}
		/**
		 *\brief		Retrieves the position (wheel or mouse, depending on mouse event type)
		 *\return		The position
		 */
		inline Position const & GetPosition()const
		{
			return m_position;
		}

	private:
		//! The mouse event type
		eMOUSE_EVENT m_mouseEventType;
		//! The mouse button
		eMOUSE_BUTTON m_button;
		//! The position, (whhel or mouse, depending on mouse event type)
		Position m_position;
	};
}

#endif
