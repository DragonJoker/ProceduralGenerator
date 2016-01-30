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
#ifndef ___GENERATOR_CONTROL_H___
#define ___GENERATOR_CONTROL_H___

#include "EventHandler.h"
#include "Pixel.h"
#include "Position.h"
#include "Size.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@brief
		Description of a control
	*/
	class GeneratorAPI Control
		: public EventHandler< Control >
	{
	public:
		/** Constructor
		@param[in] p_type
			The type
		@param[in] p_parent
			The parent control, if any
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
		Control( eCONTROL_TYPE p_type, std::shared_ptr< Control > p_parent, uint32_t p_id, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true );

		/** Destructor
		*/
		virtual ~Control();

		/** Creates the control's overlays
		@param[in] p_ctrlManager
			The controls manager
		@param[in] p_ovManager
			The overlay manager
		*/
		void Create( std::shared_ptr< ControlsManager > p_ctrlManager, std::shared_ptr< OverlayManager > p_ovManager );

		/** Sets the position
		@param[in] p_value
			The new value
		*/
		void SetPosition( Position const & p_value );

		/** Retrieves the absolute control position
		@return
			The value
		*/
		Position GetAbsolutePosition()const;

		/** Sets the size
		@param[in] p_value
			The new value
		*/
		void SetSize( Size const & p_value );

		/** Sets the background borders size
		@param[in] p_size
			The new value
		*/
		void SetBackgroundBorders( Point4i const & p_size );

		/** Sets the background colour
		@param[in] p_colour
			The new value
		*/
		void SetBackgroundColour( Colour const & p_colour );

		/** Sets the foreground colour
		@param[in] p_colour
			The new value
		*/
		void SetForegroundColour( Colour const & p_colour );

		/** Sets the background texture
		@param[in] p_texture
			The new value
		*/
		void SetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture );

		/** Sets the foreground texture
		@param[in] p_texture
			The new value
		*/
		void SetForegroundTexture( std::shared_ptr< gl::Texture > p_texture );

		/** Sets the visibility
		@param[in] p_value
			The new value
		*/
		void SetVisible( bool p_value );

		/** Retrieves the visibility status
		@return
			The value
		*/
		bool IsVisible()const;

		/** Retrieves a control
		@param[in] p_id
			The control ID
		*/
		std::shared_ptr< Control > GetChildControl( uint32_t p_id );

		/** Retrieves the control ID
		@return
			The value
		*/
		inline uint32_t GetId()const
		{
			return m_id;
		}

		/** Retrieves the type
		@return
			The value
		*/
		inline eCONTROL_TYPE GetType()const
		{
			return m_type;
		}

		/** Retrieves the style
		@return
			The value
		*/
		inline uint32_t GetStyle()const
		{
			return m_style;
		}

		/** Retrieves the position
		@return
			The value
		*/
		inline Position const & GetPosition()const
		{
			return m_position;
		}

		/** Retrieves the size
		@return
			The value
		*/
		inline Size const & GetSize()const
		{
			return m_size;
		}

		/** Retrieves the background colour
		@return
			The value
		*/
		inline Colour const & GetBackgroundColour()const
		{
			return m_backgroundColour;
		}

		/** Retrieves the foreground colour
		@return
			The value
		*/
		inline Colour const & GetForegroundColour()const
		{
			return m_foregroundColour;
		}

		/** Retrieves the background texture
		@return
			The value
		*/
		inline std::shared_ptr< gl::Texture > GetBackgroundTexture()const
		{
			return m_backgroundTexture.lock();
		}

		/** Retrieves the foreground texture
		@return
			The value
		*/
		inline std::shared_ptr< gl::Texture > GetForegroundTexture()const
		{
			return m_foregroundTexture.lock();
		}

		/** Shows the control
		*/
		inline void Show()
		{
			SetVisible( true );
		}

		/** Hides the control
		*/
		inline void Hide()
		{
			SetVisible( false );
		}

		/** Retrieves the control's parent
		@return
			The parent
		*/
		inline std::shared_ptr< Control > GetParent()const
		{
			return m_parent.lock();
		}

		/** Retrieves the control's main overlay (to be the parent of child controls' overlays)
		@return
			The main overlay
		*/
		inline std::shared_ptr< BorderPanelOverlay > GetBackground()const
		{
			return m_background.lock();
		}

		/** Retrieves the cursor when mouse is over this control
		@return
			The main overlay
		*/
		inline eMOUSE_CURSOR GetCursor()const
		{
			return m_cursor;
		}

	protected:
		/** Creates the control's overlays and sub-controls
		@param[in] p_manager
			The overlay manager
		*/
		virtual void DoCreate( std::shared_ptr< OverlayManager > p_manager ) = 0;

		/** Sets the position
		@param[in] p_value
			The new value
		*/
		virtual void DoSetPosition( Position const & p_value ) = 0;

		/** Sets the size
		@param[in] p_value
			The new value
		*/
		virtual void DoSetSize( Size const & p_value ) = 0;

		/** Sets the background colour
		@param[in] p_colour
			The new value
		*/
		virtual void DoSetBackgroundColour( Colour const & p_colour ) = 0;

		/** Sets the foreground colour
		@param[in] p_colour
			The new value
		*/
		virtual void DoSetForegroundColour( Colour const & p_colour ) = 0;

		/** Sets the background texture
		@param[in] p_texture
			The new value
		*/
		virtual void DoSetBackgroundTexture( std::shared_ptr< gl::Texture > p_texture ) = 0;

		/** Sets the foreground texture
		@param[in] p_texture
			The new value
		*/
		virtual void DoSetForegroundTexture( std::shared_ptr< gl::Texture > p_texture ) = 0;

		/** Tells if the control catches mouse events
		@remarks
			A control catches mouse events when it is visible, enabled, and when it explicitly catches it (enabled by default, except for static controls)
		@return
			false if the mouse events don't affect the control
		*/
		virtual bool DoCatchesMouseEvents()const
		{
			return IsVisible();
		}

		/** Tells if the control catches 'tab' key
		@remarks
			A control catches 'tab' key when it is visible, enabled, and when it explicitly catches it (disabled by default)
		@return
			false if the 'tab' key doesn't affect the control
		*/
		virtual bool DoCatchesTabKey()const
		{
			return IsVisible();
		}

		/** Tells if the control catches 'return' key
		@remarks
			A control catches 'return' key when it is visible, enabled, and when it explicitly catches it (disabled by default)
		@return
			false if the 'return' key doesn't affect the control
		*/
		virtual bool DoCatchesReturnKey()const
		{
			return IsVisible();
		}

		/** Retrieves the visibility
		@return
			The value
		*/
		virtual bool DoIsVisible()
		{
			return m_visible;
		}

		/** Sets the visibility
		@remarks
			Used for derived control specific behaviour
		@param[in] p_value
			The new value
		*/
		virtual void DoSetVisible( bool p_visible )
		{
		}

		/** Retrieves the controls manager
		@return
			The manager
		*/
		virtual std::shared_ptr< ControlsManager > DoGetCtrlManager()
		{
			return m_ctrlManager.lock();
		}

		/** Retrieves the background colour
		@return
			The value
		*/
		inline Colour & DoGetBackgroundColour()
		{
			return m_backgroundColour;
		}

		/** Retrieves the foreground colour
		@return
			The value
		*/
		inline Colour & DoGetForegroundColour()
		{
			return m_foregroundColour;
		}

	protected:
		//! The parent control, if any
		std::weak_ptr< Control > m_parent;
		//! The cursor when mouse is over this control
		eMOUSE_CURSOR m_cursor;

	private:
		//! The ID
		const uint32_t m_id;
		//! The type
		const eCONTROL_TYPE m_type;
		//! The style
		const uint32_t m_style;
		//! The position
		Position m_position;
		//! The dimensions
		Size m_size;
		//! The borders size
		Point4i m_borders;
		//! The background colour
		Colour m_backgroundColour;
		//! The foreground colour
		Colour m_foregroundColour;
		//! The background texture
		std::weak_ptr< gl::Texture > m_backgroundTexture;
		//! The foreground texture
		std::weak_ptr< gl::Texture > m_foregroundTexture;
		//! Visibility status
		bool m_visible;
		//! The border panel overlay used as a background
		std::weak_ptr< BorderPanelOverlay > m_background;
		//! The child controls
		std::vector< std::weak_ptr< Control > > m_childs;
		//! The overlay manager
		std::weak_ptr< OverlayManager > m_ovManager;
		//! The controls manager
		std::weak_ptr< ControlsManager > m_ctrlManager;
	};
}

#endif
