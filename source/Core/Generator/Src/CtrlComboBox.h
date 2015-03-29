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
#ifndef ___GENERATOR_COMBO_BOX_H___
#define ___GENERATOR_COMBO_BOX_H___

#include "CtrlControl.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\brief		ComboBox control
	*/
	class GeneratorAPI ComboBoxCtrl
		: public Control
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_values	The list values
		 *\param[in]	p_selected	The selected value
		 *\param[in]	p_id		The ID
		 *\param[in]	p_position	The position
		 *\param[in]	p_size		The size
		 *\param[in]	p_style		The style
		 *\param[in]	p_visible	Initial visibility status
		 */
		ComboBoxCtrl( StringArray const & p_values, int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true );
		/**
		 *\brief		Constructor
		 *\param[in]	p_values	The list values
		 *\param[in]	p_selected	The selected value
		 *\param[in]	p_id		The ID
		 *\param[in]	p_position	The position
		 *\param[in]	p_size		The size
		 *\param[in]	p_style		The style
		 *\param[in]	p_visible	Initial visibility status
		 */
		template< size_t N >
		ComboBoxCtrl( String const( & p_values )[N], int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true )
			: Control( eCONTROL_TYPE_COMBO, p_id, p_position, p_size, p_style, p_visible )
		{
			std::vector< String > l_values( &p_values[0], &p_values[N] );
			DoCreate( l_values, p_selected, p_id, p_position, p_size );
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~ComboBoxCtrl();
		/**
		 *\brief		Appends a new item
		 *\param[in]	p_value	The item
		 */
		void AppendItem( String  const & p_value );
		/**
		 *\brief		Removes an item
		 *\param[in]	p_value	The item index
		 */
		void RemoveItem( int p_value );
		/**
		 *\brief		Sets an item text
		 *\param[in]	p_value	The item index
		 *\param[in]	p_text	The item text
		 */
		void SetItemText( int p_index, String const & p_text );
		/**
		 *\brief		Clears the items
		 */
		void Clear();
		/**
		 *\brief		Sets the selected item
		 *\param[in]	p_value	The new value
		 */
		void SetSelected( int p_value );
		/**
		 *\brief		Retrieves the items
		 *\return		The value
		 */
		StringArray const & GetItems()const;
		/**
		 *\brief		Retrieves the items count
		 *\return		The value
		 */
		uint32_t GetItemCount()const;
		/**
		 *\brief		Retrieves the selected item index
		 *\return		The value
		 */
		int GetSelected()const;
		/**
		 *\brief		Connects a function to a combobox event
		 *\param[in]	p_event		The event type
		 *\param[in]	p_function	The function
		 *\return		The internal function index, to be able to disconnect it
		 */
		inline uint32_t Connect( eCOMBOBOX_EVENT p_event, std::function< void( int ) > p_function )
		{
			return m_signals[p_event].Connect( p_function );
		}
		/**
		 *\brief		Disconnects a function from a combobox event
		 *\param[in]	p_event	The event type
		 *\param[in]	p_index	The function index
		 */
		inline void Disconnect( eCOMBOBOX_EVENT p_event, uint32_t p_index )
		{
			m_signals[p_event].Disconnect( p_index );
		}

	private:
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
		 *\copydoc		ProceduralTextures::Control::DoCatchesMouseEvents
		 */
		virtual bool DoCatchesMouseEvents()const;
		/**
		 *\copydoc		ProceduralTextures::Control::DoSetVisible
		 */
		virtual void DoSetVisible( bool p_visible );
		/**
		 *\brief		Event raised when an item is selected in the choices listbox
		 *\param[in]	p_selected	The item index
		 */
		void OnSelected( int p_selected );
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
		 *\brief		Creates sub-controls
		 *\param[in]	p_values	The list values
		 *\param[in]	p_selected	The selected value
		 *\param[in]	p_id		The ID
		 *\param[in]	p_position	The position
		 *\param[in]	p_size		The size
		 */
		void DoCreate( StringArray const & p_values, int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size );
		/**
		 *\brief		Switch the combobox list
		 */
		void DoSwitchExpand();

	private:
		//! The text overlay used to display the caption
		std::weak_ptr< TextOverlay > m_text;
		//! The button used to expand the combo box
		std::shared_ptr< ButtonCtrl > m_expand;
		//! The list used to hold the combo box choices
		std::shared_ptr< ListBoxCtrl > m_choices;
		//! The combobox events signals
		Signal< std::function< void( int ) > > m_signals[eCOMBOBOX_EVENT_COUNT];
	};
}

#endif
