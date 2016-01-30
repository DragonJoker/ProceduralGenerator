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
	@author
		Sylvain DOREMUS
	@version
		2.0.0
	@brief
		ComboBox control
	*/
	class GeneratorAPI ComboBoxCtrl
		: public Control
	{
	public:
		/** Constructor
		@param[in] p_parent
			The parent control, if any
		@param[in] p_values
			The list values
		@param[in] p_selected
			The selected value
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
		ComboBoxCtrl( std::shared_ptr< Control > p_parent, StringArray const & p_values, int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true );

		/** Constructor
		@param[in] p_parent
			The parent control, if any
		@param[in] p_values
			The list values
		@param[in] p_selected
			The selected value
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
		ComboBoxCtrl( std::shared_ptr< Control > p_parent, String const( & p_values )[N], int p_selected, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true )
			: Control( eCONTROL_TYPE_COMBO, p_parent, p_id, p_position, p_size, p_style, p_visible )
			, m_values( StringArray( &p_values[0], &p_values[N] ) )
			, m_selected( p_selected )
		{
		}

		/** Destructor
		*/
		virtual ~ComboBoxCtrl();

		/** Appends a new item
		@param[in] p_value
			The item
		*/
		void AppendItem( String  const & p_value );

		/** Removes an item
		@param[in] p_value
			The item index
		*/
		void RemoveItem( int p_value );

		/** Sets an item text
		@param[in] p_index
			The item index
		@param[in] p_text
			The item text
		*/
		void SetItemText( int p_index, String const & p_text );

		/** Clears the items
		*/
		void Clear();

		/** Sets the selected item
		@param[in] p_value
			The new value
		*/
		void SetSelected( int p_value );

		/** Retrieves the items
		@return
			The value
		*/
		StringArray const & GetItems()const;

		/** Retrieves the items count
		@return
			The value
		*/
		uint32_t GetItemCount()const;

		/** Retrieves the selected item index
		@return
			The value
		*/
		int GetSelected()const;

		/** Connects a function to a combobox event
		@param[in] p_event
			The event type
		@param[in] p_function
			The function
		@return
			The internal function index, to be able to disconnect it
		*/
		inline uint32_t Connect( eCOMBOBOX_EVENT p_event, std::function< void( int ) > p_function )
		{
			return m_signals[p_event].Connect( p_function );
		}

		/** Disconnects a function from a combobox event
		@param[in] p_event
			The event type
		@param[in] p_index
			The function index
		*/
		inline void Disconnect( eCOMBOBOX_EVENT p_event, uint32_t p_index )
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

		/** @copydoc ProceduralTextures::Control::DoCatchesMouseEvents
		*/
		virtual bool DoCatchesMouseEvents()const;

		/** @copydoc ProceduralTextures::Control::DoSetVisible
		*/
		virtual void DoSetVisible( bool p_visible );

		/** Event raised when an item is selected in the choices listbox
		@param[in] p_selected
			The item index
		*/
		void OnSelected( int p_selected );

		/** Event when a keyboard key is pressed
		@param[in] p_event
			The keyboard event
		*/
		void OnKeyDown( KeyboardEvent const & p_event );

		/** Event when a keyboard key is pressed on the active tick or line control
		@param[in] p_event
			The keyboard event
		*/
		void OnNcKeyDown( std::shared_ptr< Control > p_control, KeyboardEvent const & p_event );

		/** Switch the combobox list
		*/
		void DoSwitchExpand();

	private:
		//! The text overlay used to display the caption
		std::weak_ptr< TextOverlay > m_text;
		//! The button used to expand the combo box
		std::shared_ptr< ButtonCtrl > m_expand;
		//! The list used to hold the combo box choices
		std::shared_ptr< ListBoxCtrl > m_choices;
		//! The combo box string choices
		std::vector< String > m_values;
		//! Teh selected value index
		int m_selected;
		//! The combobox events signals
		Signal< std::function< void( int ) > > m_signals[eCOMBOBOX_EVENT_COUNT];
	};
}

#endif
