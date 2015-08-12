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
#ifndef ___GENERATOR_EDIT_H___
#define ___GENERATOR_EDIT_H___

#include "CtrlControl.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		16/02/2015
	@version
		2.0.0
	@brief
		Edit control
	*/
	class GeneratorAPI EditCtrl
		: public Control
	{
	public:
		/** Constructor
		@param[in] p_parent
			The parent control, if any
		@param[in] p_caption
			The caption
		@param[in] p_id
			The ID
		@param[in] p_position
			The position
		@param[in] p_size
			The size
		@param[in] p_style
			The style
		@param[in] p_visible
			Initial visibility status
		 */
		EditCtrl( std::shared_ptr< Control > p_parent, String const & p_caption, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true );

		/** Destructor
		 */
		virtual ~EditCtrl();

		/** Sets the caption
		@param[in] p_value
			The new value
		 */
		void SetCaption( String const & p_value );

		/** Retrieves the caption
		@return
			The value
		 */
		inline String const & GetCaption()const
		{
			return m_caption;
		}

		/** Updates the caption
		@param[in] p_value
			The new value
		 */
		inline void UpdateCaption( String const & p_value )
		{
			m_caption = p_value;
		}

		/** Connects a function to an edit event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		@return
			The internal function index, to be able to disconnect it
		 */
		inline uint32_t Connect( eEDIT_EVENT p_event, std::function< void( String const & ) > p_function )
		{
			return m_signals[p_event].Connect( p_function );
		}

		/** Disconnects a function from an edit event
		@param[in] p_event
			The event type
		@param[in] p_index
			The function index
		 */
		inline void Disconnect( eEDIT_EVENT p_event, uint32_t p_index )
		{
			m_signals[p_event].Disconnect( p_index );
		}

	private:
		/** @copydoc ProceduralTextures::Control::DoCreate
		 */
		virtual void DoCreate( std::shared_ptr< OverlayManager > p_manager );

		/** @copydoc ProceduralTextures::Control::DoSetPosition
		 */
		virtual void DoSetPosition( Position const & p_value );

		/** @copydoc ProceduralTextures::Control::DoSetSize
		 */
		virtual void DoSetSize( Size const & p_value );

		/** @copydoc ProceduralTextures::Control::DoSetBackgroundColour
		 */
		virtual void DoSetBackgroundColour( Colour const & p_colour );

		/** @copydoc ProceduralTextures::Control::DoSetForegroundColour
		 */
		virtual void DoSetForegroundColour( Colour const & p_colour );

		/** @copydoc ProceduralTextures::Control::DoSetBackgroundTexture
		 */
		virtual void DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture );

		/** @copydoc ProceduralTextures::Control::DoSetForegroundTexture
		 */
		virtual void DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture );

		/** @copydoc ProceduralTextures::Control::DoSetVisible
		 */
		virtual void DoSetVisible( bool p_visible );

		/** Event when the control is activated
		@param[in] p_event
			The control event
		 */
		void OnActivate( ControlEvent const & p_event );

		/** Event when the control is deactivated
		@param[in] p_event
			The control event
		 */
		void OnDeactivate( ControlEvent const & p_event );

		/** Event when mouse left button is pushed
		@param[in] p_event
			The mouse event
		 */
		void OnMouseLButtonDown( MouseEvent const & p_event );

		/** Event when mouse left button is released
		@param[in] p_event
			The mouse event
		 */
		void OnMouseLButtonUp( MouseEvent const & p_event );

		/** Event when a printable key is pressed
		@param[in] p_event
			The mouse event
		 */
		void OnChar( KeyboardEvent const & p_event );

		/** Event when a keyboard key is pressed
		@param[in] p_event
			The mouse event
		 */
		void OnKeyDown( KeyboardEvent const & p_event );

		/** Event when a keyboard key is pressed
		@param[in] p_event
			The mouse event
		 */
		void OnKeyUp( KeyboardEvent const & p_event );

		/** Adds a character at caret index
		 */
		void DoAddCharAtCaret( String const & p_char );

		/** Removes a character at caret index
		 */
		void DoDeleteCharAtCaret();

		/** Removes a character before caret index (backspace)
		 */
		void DoDeleteCharBeforeCaret();

		/** Updates the caption and text overlay
		 */
		void DoUpdateCaption();

		/** Retrieves the caption with caret
		@return
			The caption and the caret at good position
		 */
		String DoGetCaptionWithCaret()const;

	private:
		//! The caption
		String m_caption;
		//! The caret index in the caption
		StringUtils::Utf8Iterator m_caretIt;
		//! The activation status
		bool m_active;
		//! The text overlay used to display the caption
		std::weak_ptr< TextOverlay > m_text;
		//! The edit events signals
		Signal< std::function< void( String const & ) > > m_signals[eEDIT_EVENT_COUNT];
	};
}

#endif
