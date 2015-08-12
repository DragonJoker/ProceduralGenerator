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
#ifndef ___GENERATOR_KEYBOARD_EVENT_H___
#define ___GENERATOR_KEYBOARD_EVENT_H___

#include "Event.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		02/03/2015
	@version
		2.0.0
	@brief
		Structure holding mouse state
	*/
	struct KeyboardState
	{
		//! The control key state
		bool m_ctrl;
		//! The alt key state
		bool m_alt;
		//! The shift key state
		bool m_shift;
	};
	/*!
	@author
		Sylvain DOREMUS
	@version
		2.0.0
	@brief
		Description of a keyboard event
	*/
	class KeyboardEvent
		: public Event
	{
	public:
		/** Constructor
		@param[in] p_type
			The keyboard event type
		@param[in] p_key
			The key
		@param[in] p_ctrl
			Tells if the Ctrl key is down
		@param[in] p_alt
			Tells if the Alt key is down
		@param[in] p_shift
			Tells if the Shift key is down
		 */
		KeyboardEvent( eKEYBOARD_EVENT p_type, eKEYBOARD_KEY p_key, bool p_ctrl, bool p_alt, bool p_shift )
			: Event( eEVENT_TYPE_KEYBOARD )
			, m_keyboardEventType( p_type )
			, m_key( p_key )
			, m_ctrl( p_ctrl )
			, m_alt( p_alt )
			, m_shift( p_shift )
		{
		}

		/** Constructor
		@param[in] p_type
			The keyboard event type
		@param[in] p_key
			The key
		@param[in] p_char
			The character corresponding to the key
		@param[in] p_ctrl
			Tells if the Ctrl key is down
		@param[in] p_alt
			Tells if the Alt key is down
		@param[in] p_shift
			Tells if the Shift key is down
		 */
		KeyboardEvent( eKEYBOARD_EVENT p_type, eKEYBOARD_KEY p_key, String const & p_char, bool p_ctrl, bool p_alt, bool p_shift )
			: Event( eEVENT_TYPE_KEYBOARD )
			, m_keyboardEventType( p_type )
			, m_key( p_key )
			, m_char( p_char )
			, m_ctrl( p_ctrl )
			, m_alt( p_alt )
			, m_shift( p_shift )
		{
		}

		/** Destructor
		 */
		virtual ~KeyboardEvent()
		{
		}

		/** Retrieves the event type
		@return
				The type
		 */
		inline eKEYBOARD_EVENT GetKeyboardEventType()const
		{
			return m_keyboardEventType;
		}

		/** Retrieves the key code
		@return
				The key
		 */
		inline eKEYBOARD_KEY GetKey()const
		{
			return m_key;
		}

		/** Retrieves the character
		@return
				The key
		 */
		inline String const & GetChar()const
		{
			return m_char;
		}

		/** Tells if the Ctrl key is down
		@return
				The status
		 */
		inline bool IsCtrlDown()const
		{
			return m_ctrl;
		}

		/** Tells if the Alt key is down
		@return
				The status
		 */
		inline bool IsAltDown()const
		{
			return m_alt;
		}

		/** Tells if the Shift key is down
		@return
				The status
		 */
		inline bool IsShiftDown()const
		{
			return m_shift;
		}

	private:
		//! The event type
		eKEYBOARD_EVENT m_keyboardEventType;
		//! The key code
		eKEYBOARD_KEY m_key;
		//! The character
		String m_char;
		//! Tells if the Ctrl key is down
		bool m_ctrl;
		//! Tells if the Alt key is down
		bool m_alt;
		//! Tells if the Shift key is down
		bool m_shift;
	};
}

#endif
