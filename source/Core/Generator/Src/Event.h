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
#ifndef ___GENERATOR_EVENT_H___
#define ___GENERATOR_EVENT_H___

#include "GeneratorPrerequisites.h"

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
		Supported events enumeration
	*/
	typedef enum eEVENT_TYPE
	{
		//! Control event type
		eEVENT_TYPE_CONTROL,
		//! Mouse event type
		eEVENT_TYPE_MOUSE,
		//! Keyboard event type
		eEVENT_TYPE_KEYBOARD,
	}	eEVENT_TYPE;
	/*!
	@author
		Sylvain DOREMUS
	@version
		2.0.0
	@brief
		Description of an event.
		You may use this one, but prefer using ControlEvent, MouseEvent or KeyboardEvent.
	*/
	class Event
	{
	public:
		/** Constructor
		@param[in] p_type
			The event type
		*/
		Event( eEVENT_TYPE p_type )
			: m_eventType( p_type )
		{
		}

		/** Destructor
		*/
		virtual ~Event()
		{
		}

		/** Retrieves the event type
		@return
			The type
		*/
		inline eEVENT_TYPE GetEventType()const
		{
			return m_eventType;
		}

	private:
		//! Event type
		eEVENT_TYPE m_eventType;
	};
}

#endif
