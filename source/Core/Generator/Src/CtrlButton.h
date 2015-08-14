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
#ifndef ___GENERATOR_BUTTON_H___
#define ___GENERATOR_BUTTON_H___

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
		Button control
	*/
	class GeneratorAPI ButtonCtrl
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
		ButtonCtrl( std::shared_ptr< Control > p_parent, String const & p_caption, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true );

		/** Destructor
		*/
		virtual ~ButtonCtrl();

		/** Sets the caption
		@param[in] p_value
			The new value
		*/
		void SetCaption( String const & p_value );

		/** Sets the mouse over background texture
		@param[in] p_texture
			The new value
		*/
		void SetMouseOverBackgroundTexture( std::shared_ptr< gl::Texture > p_texture );

		/** Sets the mouse over foreground texture
		@param[in] p_texture
			The new value
		*/
		void SetMouseOverForegroundTexture( std::shared_ptr< gl::Texture > p_texture );

		/** Retrieves the caption
		@return
			The value
		*/
		inline String const & GetCaption()const
		{
			return m_caption;
		}

		/** Connects a function to a button event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		@return
			The internal function index, to be able to disconnect it
		*/
		inline uint32_t Connect( eBUTTON_EVENT p_event, std::function< void() > p_function )
		{
			return m_signals[p_event].Connect( p_function );
		}

		/** Disconnects a function from a button event
		@param[in] p_event
			The event type
		@param[in] p_index
			The function index
		*/
		inline void Disconnect( eBUTTON_EVENT p_event, uint32_t p_index )
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

		/** Event when mouse enters the control
		@param[in] p_event
			The mouse event
		*/
		void OnMouseEnter( MouseEvent const & p_event );

		/** Event when mouse leaves the control
		@param[in] p_event
			The mouse event
		*/
		void OnMouseLeave( MouseEvent const & p_event );

		/** Event when mouse left button is released
		@param[in] p_event
			The mouse event
		*/
		void OnMouseLButtonUp( MouseEvent const & p_event );

	private:
		//! The caption
		String m_caption;
		//! The text overlay used to display the caption
		std::weak_ptr< TextOverlay > m_text;
		//! The mouse over background colour
		Colour m_mouseOverBackgroundColour;
		//! The mouse over foreground colour
		Colour m_mouseOverForegroundColour;
		//! The mouse over background texture
		std::weak_ptr< gl::Texture > m_mouseOverBackgroundTexture;
		//! The mouse over foreground texture
		std::weak_ptr< gl::Texture > m_mouseOverForegroundTexture;
		//! The button events signals
		Signal< std::function< void() > > m_signals[eBUTTON_EVENT_COUNT];
	};
}

#endif
