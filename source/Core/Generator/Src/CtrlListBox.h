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
#ifndef ___GENERATOR_LIST_BOX_H___
#define ___GENERATOR_LIST_BOX_H___

#include "CtrlControl.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@version
		2.0.0
	@brief
		ListBox control
	*/
	class GeneratorAPI ListBoxCtrl
		: public Control
	{
	public:
		/** Constructor
		@param[in] p_parent
			The parent control, if any
		@param[in] p_values
			The values list
		@param[in] p_selected
			The selected value index (-1 for no selection)
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
		ListBoxCtrl( std::shared_ptr< Control > p_parent, StringArray const & p_values, int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true );

		/** Constructor
		@param[in] p_parent
			The parent control, if any
		@param[in] p_values
			The values list
		@param[in] p_selected
			The selected value index (-1 for no selection)
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
		template< size_t N >
		ListBoxCtrl( std::shared_ptr< Control > p_parent, String const( & p_values )[N], int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true )
			: Control( eCONTROL_TYPE_LIST, p_parent, p_id, p_position, p_size, p_style, p_visible )
			, m_values( std::vector< String >( &p_values[0], &p_values[N] ) )
			, m_selected( p_selected )
		{
		}

		/** Destructor
		 */
		virtual ~ListBoxCtrl();

		/** Sets the background colour for selected item
		@param[in] p_colour
			The new value
		 */
		void SetSelectedBackgroundColour( Colour const & p_colour );

		/** Sets the foreground colour for selected item
		@param[in] p_colour
			The new value
		 */
		void SetSelectedForegroundColour( Colour const & p_colour );

		/** Appends a new item
		@param[in] p_value
			The item
		 */
		void AppendItem( String  const & p_value );

		/** Removes an item
		@param[in] p_index
			The item index
		 */
		void RemoveItem( int p_index );

		/** Sets an item text
		@param[in] p_index
			The item index
		@param[in] p_text
			The item text
		 */
		void SetItemText( int p_index, String const & p_text );

		/** Retrieves an item text
		@param[in] p_index
			The item index
		@return
			The item text
		 */
		String GetItemText( int p_index );

		/** Clears the items
		 */
		void Clear();

		/** Sets the selected item
		@param[in] p_index
			The new value
		 */
		void SetSelected( int p_index );

		/** Retrieves the items
		@return
			The value
		 */
		inline StringArray const & GetItems()const
		{
			return m_values;
		}

		/** Retrieves the items count
		@return
			The value
		 */
		inline uint32_t GetItemCount()const
		{
			return uint32_t( m_values.size() );
		}

		/** Retrieves the selected item index
		@return
			The value
		 */
		inline int GetSelected()const
		{
			return m_selected;
		}

		/** Retrieves the background colour for selected item
		@return
			The value
		 */
		inline Colour const & GetSelectedBackgroundColour()const
		{
			return m_selectedBackgroundColour;
		}

		/** Retrieves the foreground colour for selected item
		@return
			The value
		 */
		inline Colour const & GetSelectedForegroundColour()const
		{
			return m_selectedForegroundColour;
		}

		/** Connects a function to a listbox event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		@return
			The internal function index, to be able to disconnect it
		 */
		inline uint32_t Connect( eLISTBOX_EVENT p_event, std::function< void( int ) > p_function )
		{
			return m_signals[p_event].Connect( p_function );
		}

		/** Disconnects a function from a listbox event
		@param[in] p_event
			The event type
		@param[in] p_index
			The function index
		 */
		inline void Disconnect( eLISTBOX_EVENT p_event, uint32_t p_index )
		{
			m_signals[p_event].Disconnect( p_index );
		}

	private:
		/** Creates a sub-control
		@param[in] p_value
			The control label
		*/
		void DoCreateItem( String const & p_value );

		/** Recomputes the items positions, according to their position in the items array
		 */
		void DoUpdateItems();

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

		/** Event when mouse enters an item
		@param[in] p_control
			The item
		@param[in] p_event
			The mouse event
		 */
		void OnItemMouseEnter( std::shared_ptr< Control > p_control, MouseEvent const & p_event );

		/** Event when mouse leaves an item
		@param[in] p_control
			The item
		@param[in] p_event
			The mouse event
		 */
		void OnItemMouseLeave( std::shared_ptr< Control > p_control, MouseEvent const & p_event );

		/** Event when mouse left button is released on an item
		@param[in] p_control
			The item
		@param[in] p_event
			The mouse event
		 */
		void OnItemMouseLButtonUp( std::shared_ptr< Control > p_control, MouseEvent const & p_event );

		/** Event when a keyboard key is pressed on the active tick or line control
		@param[in] p_event
			The keyboard event
		 */
		void OnItemKeyDown( std::shared_ptr< Control > p_control, KeyboardEvent const & p_event );

		/** Event when a keyboard key is pressed
		@param[in] p_event
			The keyboard event
		 */
		void OnKeyDown( KeyboardEvent const & p_event );

	private:
		//! The combo box values
		StringArray m_values;
		//! The selected value
		int m_selected;
		//! The selected item
		std::weak_ptr< StaticCtrl > m_selectedItem;
		//! The text overlay used to display the caption
		std::vector< std::shared_ptr< StaticCtrl > > m_items;
		//! The background colour, for normal item
		Colour m_backgroundColourNormal;
		//! The background colour, for highlighted item
		Colour m_backgroundColourHighlight;
		//! The background colour
		Colour m_selectedBackgroundColour;
		//! The foreground colour
		Colour m_selectedForegroundColour;
		//! The listbox events signals
		Signal< std::function< void( int ) > > m_signals[eLISTBOX_EVENT_COUNT];
	};
}

#endif
