/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_CONTROLS_MANAGER_H___
#define ___GENERATOR_CONTROLS_MANAGER_H___

#include "GeneratorPrerequisites.h"
#include "Position.h"
#include "EventHandler.h"

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
		Class used to to manage the controls: events and all GUI related stuff.
	*/
	class GeneratorAPI ControlsManager
		: public std::enable_shared_from_this< ControlsManager >
	{
	public:
		/**@name General */
		//@{

		/** Constructor
		@param[in] p_generator
			The generator
		@param[in] p_overlayManager
			The overlay manager
		*/
		ControlsManager( std::shared_ptr< GeneratorBase > p_generator, std::shared_ptr< OverlayManager > p_overlayManager );
		/** Destructor
		*/
		virtual ~ControlsManager();

		/** Handles all queued events
		*/
		void ProcessEvents();

		/** Retrieves the current mouse position
		@return
			The value
		*/
		Position const & GetMousePosition()
		{
			return m_mouse.m_position;
		}

		//@}
		/**@name Mouse events */
		//@{

		/** Fires a mouse move event
		@param[in] p_position
			The mouse position
		*/
		void FireMouseMove( Position const & p_position );

		/** Fires a mouse pushed event
		@param[in] p_button
			The mouse button
		*/
		void FireMouseButtonPushed( eMOUSE_BUTTON p_button );

		/** Fires a mouse released event
		@param[in] p_button
			The mouse button
		*/
		void FireMouseButtonReleased( eMOUSE_BUTTON p_button );

		/** Fires a mouse wheel event
		@param[in] p_offsets
			The mouse wheel offsets (x and y)
		*/
		void FireMouseWheel( Position const & p_offsets );

		//@}
		/**@name Keyboard events */
		//@{

		/** Fires a keyboard key down event
		@param[in] p_key
			The key
		@param[in] p_ctrl
			Tells if the Ctrl key is down
		@param[in] p_alt
			Tells if the Alt key is down
		@param[in] p_shift
			Tells if the Shift key is down
		*/
		void FireKeyDown( eKEYBOARD_KEY p_key, bool p_ctrl, bool p_alt, bool p_shift );

		/** Fires a keyboard key up event
		@param[in] p_key
			The key
		@param[in] p_ctrl
			Tells if the Ctrl key is down
		@param[in] p_alt
			Tells if the Alt key is down
		@param[in] p_shift
			Tells if the Shift key is down
		*/
		void FireKeyUp( eKEYBOARD_KEY p_key, bool p_ctrl, bool p_alt, bool p_shift );

		/** Fires a printable key event
		@param[in] p_key
			The key
		@param[in] p_char
			The character coorresponding to the key, displayable as is
		*/
		void FireChar( eKEYBOARD_KEY p_key, String const & p_char );

		//@}
		/**@name Controls management */
		//@{

		/** Creates the given control's overlays and binds its callbacks to appropriate events
		@param[in] p_control
			The control
		*/
		void Create( std::shared_ptr< ButtonCtrl > p_control );

		/** Creates the given control's overlays and binds its callbacks to appropriate events
		@param[in] p_control
			The control
		*/
		void Create( std::shared_ptr< ComboBoxCtrl > p_control );

		/** Creates the given control's overlays and binds its callbacks to appropriate events
		@param[in] p_control
			The control
		*/
		void Create( std::shared_ptr< EditCtrl > p_control );

		/** Creates the given control's overlays and binds its callbacks to appropriate events
		@param[in] p_control
			The control
		*/
		void Create( std::shared_ptr< ListBoxCtrl > p_control );

		/** Creates the given control's overlays and binds its callbacks to appropriate events
		@param[in] p_control
			The control
		*/
		void Create( std::shared_ptr< SliderCtrl > p_control );

		/** Creates the given control's overlays and binds its callbacks to appropriate events
		@param[in] p_control
			The control
		*/
		void Create( std::shared_ptr< StaticCtrl > p_control );

		/** Adds a control that can an event target
		@param[in] p_control
			The control
		*/
		void AddControl( std::shared_ptr< Control > p_control );

		/** Removes a control
		@param[in] p_id
			The control ID
		*/
		void RemoveControl( uint32_t p_id );

		/** Retrieves a control
		@param[in] p_id
			The control ID
		@return
			The control
		*/
		std::shared_ptr< Control > GetControl( uint32_t p_id );

		/** Retrieves the active control
		@return
			The control
		*/
		inline std::shared_ptr< Control > GetActiveControl()
		{
			return m_activeControl.lock();
		}

		/** Retrieves the focused control
		@return
			The control
		*/
		inline std::shared_ptr< Control > GetFocusedControl()
		{
			return m_lastMouseTarget.lock();
		}

		//@}

	private:
		/** Retrieves the first control which can be targeted by mouse, at its position
		@param[in] p_position
			The mouse position
		*/
		std::shared_ptr< Control > DoGetMouseTargetableControl( Position const & p_position )const;

		/** Updates the z-index ordered controls array
		*/
		void DoUpdate()const;

		/** Retrieve the controls in a thread-safe way
		@return
				The controls array
		*/
		std::vector< std::shared_ptr< Control > > DoGetControls()const
		{
			std::unique_lock< std::mutex > l_lock( m_mutexControls );
			return m_controls;
		}

	private:
		//! The current mouse state
		MouseState m_mouse;
		//! The current keyboard state
		KeyboardState m_keyboard;
		//! The currently active control
		std::weak_ptr< Control > m_activeControl;
		//! The currently active control
		std::weak_ptr< Control > m_lastMouseTarget;
		//! The mutex used to protect the controls array.
		mutable std::mutex m_mutexControls;
		//! The controls array
		std::vector< std::shared_ptr< Control > > m_controls;
		//! The controls array
		mutable std::vector< std::shared_ptr< Control > > m_controlsByZIndex;
		//! The controls map, sorted by ID
		std::map< uint32_t, std::weak_ptr< Control > > m_controlsById;
		//! The generator
		std::weak_ptr< GeneratorBase > m_generator;
		//! The overlay manager
		std::weak_ptr< OverlayManager > m_overlayManager;
		//! Tells the controls array has changed
		bool m_changed;
	};
}

#endif
