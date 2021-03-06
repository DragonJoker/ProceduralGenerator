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
#ifndef ___GENERATOR_EVENT_HANDLER_H___
#define ___GENERATOR_EVENT_HANDLER_H___

#include "ControlEvent.h"
#include "GeneratorSignal.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
#include <deque>
#include <mutex>

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@version
		2.0.0
	@brief
		Description of an event handler, class that can receive events
	*/
	template< class Derived >
	class GeneratorAPI EventHandler
		: public std::enable_shared_from_this< Derived >
	{
	public:
		//! Catcher definition for Control events
		typedef std::function< void( ControlEvent const & ) > ClientControlFunction;
		//! Catcher definition for non client Control events
		typedef std::function< void( std::shared_ptr< Derived >, ControlEvent const & ) > NonClientControlFunction;
		//! Catcher definition for Mouse events
		typedef std::function< void( MouseEvent const & ) > ClientMouseFunction;
		//! Catcher definition for non client Mouse events
		typedef std::function< void( std::shared_ptr< Derived >, MouseEvent const & ) > NonClientMouseFunction;
		//! Catcher definition for Keyboard events
		typedef std::function< void( KeyboardEvent const & ) > ClientKeyboardFunction;
		//! Catcher definition for non client Keyboard events
		typedef std::function< void( std::shared_ptr< Derived >, KeyboardEvent const & ) > NonClientKeyboardFunction;

		/**@name General */
		//@{

		/** Constructor
		@param[in] p_catchMouseEvents
			Defines if the event handler catches mouse events
		*/
		EventHandler( bool p_catchMouseEvents )
			: m_enabled( true )
			, m_catchMouseEvents( p_catchMouseEvents )
			, m_catchTabKey( false )
			, m_catchReturnKey( false )
		{
		}

		/** Destructor
		*/
		virtual ~EventHandler()
		{
			EventQueue l_queue;
			m_mutex.lock();
			std::swap( l_queue, m_queue );
			m_mutex.unlock();
		}

		/** Handles all queued events
		*/
		inline void ProcessEvents()
		{
			EventQueue l_queue;
			m_mutex.lock();
			std::swap( l_queue, m_queue );
			m_mutex.unlock();

			for ( auto && l_eventPair : l_queue )
			{
				l_eventPair.second();
			}
		}

		//@}
		/**@name Control events */
		//@{

		/** Adds a control event to the events queue
		@param[in] p_event
			The event
		*/
		inline void PushEvent( ControlEvent const & p_event )
		{
			std::shared_ptr< ControlEvent > l_event = std::make_shared< ControlEvent >( p_event );
			std::unique_lock< std::mutex > l_lock( m_mutex );
			m_queue.push_back( std::make_pair( l_event, std::bind( &EventHandler::ProcessControlEvent, this, l_event ) ) );
		}

		/** Connects a function to a control event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		*/
		inline void Connect( eCONTROL_EVENT p_event, ClientControlFunction p_function )
		{
			m_controlSlots[p_event].Connect( p_function );
		}

		/** Connects a function to a non client control event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		*/
		inline void ConnectNC( eCONTROL_EVENT p_event, NonClientControlFunction p_function )
		{
			m_ncControlSlots[p_event].Connect( p_function );
		}

		//@}
		/**@name Mouse events */
		//@{

		/** Adds a mouse event to the events queue
		@param[in] p_event
			The mouse event
		*/
		inline void PushEvent( MouseEvent const & p_event )
		{
			std::shared_ptr< MouseEvent > l_event = std::make_shared< MouseEvent >( p_event );
			std::unique_lock< std::mutex > l_lock( m_mutex );
			m_queue.push_back( std::make_pair( l_event, std::bind( &EventHandler::ProcessMouseEvent, this, l_event ) ) );
		}

		/** Tells if the control catches mouse events
		@remarks
			A control catches mouse events when it is enabled, and when it explicitly catches it (enabled by default, except for static controls)
		@return
			false if the mouse events don't affect the control
		*/
		inline bool CatchesMouseEvents()const
		{
			return m_enabled && m_catchMouseEvents && DoCatchesMouseEvents();
		}

		/** Sets if the control can catch mouse events
		@param[in] p_value
			The new value
		*/
		inline void SetCatchesMouseEvents( bool p_value )
		{
			m_catchMouseEvents = p_value;
		}

		/** Connects a function to a mouse event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		*/
		inline void Connect( eMOUSE_EVENT p_event, ClientMouseFunction p_function )
		{
			m_mouseSlots[p_event].Connect( p_function );
		}

		/** Connects a function to a non client mouse event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		*/
		inline void ConnectNC( eMOUSE_EVENT p_event, NonClientMouseFunction p_function )
		{
			m_ncMouseSlots[p_event].Connect( p_function );
		}

		//@}
		/**@name Keyboard events */
		//@{

		/** Adds a keyboard event to the events queue
		@param[in] p_event
			The event
		*/
		inline void PushEvent( KeyboardEvent const & p_event )
		{
			std::shared_ptr< KeyboardEvent > l_event = std::make_shared< KeyboardEvent >( p_event );
			std::unique_lock< std::mutex > l_lock( m_mutex );
			m_queue.push_back( std::make_pair( l_event, std::bind( &EventHandler::ProcessKeyboardEvent, this, l_event ) ) );
		}

		/** Tells if the control catches 'tab' key
		@remarks
			A control catches 'tab' key when it is enabled, and when it explicitly catches it (disabled by default)
		@return
			false if the 'tab' key doesn't affect the control
		*/
		inline bool CatchesTabKey()const
		{
			return m_enabled && m_catchTabKey && DoCatchesTabKey();
		}

		/** Tells if the control catches 'return' key
		@remarks
			A control catches 'return' key when it is enabled, and when it explicitly catches it (disabled by default)
		@return
			false if the 'return' key doesn't affect the control
		*/
		inline bool CatchesReturnKey()const
		{
			return m_enabled && m_catchReturnKey && DoCatchesReturnKey();
		}

		/** Connects a function to a keyboard event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		*/
		inline void Connect( eKEYBOARD_EVENT p_event, ClientKeyboardFunction p_function )
		{
			m_keyboardSlots[p_event].Connect( p_function );
		}

		/** Connects a function to a non client keyboard event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		*/
		inline void ConnectNC( eKEYBOARD_EVENT p_event, NonClientKeyboardFunction p_function )
		{
			m_ncKeyboardSlots[p_event].Connect( p_function );
		}

		//@}

	protected:
		/** Tells if the control catches mouse events
		@remarks
			A control catches mouse events when it is  enabled, and when it explicitly catches it (enables by default, except for static controls)
		@return
			false if the mouse events don't affect the control
		*/
		virtual bool DoCatchesMouseEvents()const = 0;

		/** Tells if the control catches 'tab' key
		@remarks
			A control catches 'tab' key when it is enabled, and when it explicitly catches it (disabled by default)
		@return
			false if the 'tab' key doesn't affect the control
		*/
		virtual bool DoCatchesTabKey()const = 0;

		/** Tells if the control catches 'return' key
		@remarks
			A control catches 'return' key when it is enabled, and when it explicitly catches it (disabled by default)
		@return
			false if the 'return' key doesn't affect the control
		*/
		virtual bool DoCatchesReturnKey()const = 0;

	private:
		/** Control event handler
		@param[in] p_event
			The event
		*/
		inline void ProcessControlEvent( std::shared_ptr< ControlEvent > p_event )
		{
			m_controlSlots[p_event->GetControlEventType()]( *p_event );
			m_ncControlSlots[p_event->GetControlEventType()]( this->shared_from_this(), *p_event );
		}

		/** Mouse event handler
		@param[in] p_event
			The event
		*/
		inline void ProcessMouseEvent( std::shared_ptr< MouseEvent > p_event )
		{
			m_mouseSlots[p_event->GetMouseEventType()]( *p_event );
			m_ncMouseSlots[p_event->GetMouseEventType()]( this->shared_from_this(), *p_event );
		}

		/** Keyboard event handler
		@param[in] p_event
			The event
		*/
		inline void ProcessKeyboardEvent( std::shared_ptr< KeyboardEvent > p_event )
		{
			m_keyboardSlots[p_event->GetKeyboardEventType()]( *p_event );
			m_ncKeyboardSlots[p_event->GetKeyboardEventType()]( this->shared_from_this(), *p_event );
		}

	private:
		typedef std::function< void() > EventHandlerFunction;
		typedef std::deque< std::pair< std::shared_ptr< Event >, EventHandlerFunction > > EventQueue;
		//! Activation status
		bool m_enabled;
		//! Tells if the control catches mouse events
		bool m_catchMouseEvents;
		//! Tells if the control catches 'tab' key
		bool m_catchTabKey;
		//! Tells if the control catches 'return' key
		bool m_catchReturnKey;
		//! The mutex used to protect the events queue
		std::mutex m_mutex;
		//! The events queue
		EventQueue m_queue;

	protected:
		//! The control events slots
		Signal< ClientControlFunction > m_controlSlots[eCONTROL_EVENT_COUNT];
		//! The mouse events slots
		Signal< ClientMouseFunction > m_mouseSlots[eMOUSE_EVENT_COUNT];
		//! The keyboard events slots
		Signal< ClientKeyboardFunction > m_keyboardSlots[eKEYBOARD_EVENT_COUNT];
		//! The non client control events slots
		Signal< NonClientControlFunction > m_ncControlSlots[eCONTROL_EVENT_COUNT];
		//! The non client mouse events slots
		Signal< NonClientMouseFunction > m_ncMouseSlots[eMOUSE_EVENT_COUNT];
		//! The non client keyboard events slots
		Signal< NonClientKeyboardFunction > m_ncKeyboardSlots[eKEYBOARD_EVENT_COUNT];
	};
}

#endif
