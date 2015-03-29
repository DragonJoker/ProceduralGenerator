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
#ifndef ___GENERATOR_STATIC_H___
#define ___GENERATOR_STATIC_H___

#include "CtrlControl.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\brief		Description of a control, in order to be able to create it completely
	*/
	class GeneratorAPI StaticCtrl
		: public Control
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_caption	The static caption
		 *\param[in]	p_position	The control position
		 *\param[in]	p_size		The control size
		 *\param[in]	p_style		The control style
		 *\param[in]	p_visible	Initial visibility status
		 */
		StaticCtrl( String const & p_caption, Position const & p_position, Size const & p_size, uint32_t p_style = 0, bool p_visible = true );
		/**
		 *\brief		Destructor
		 */
		virtual ~StaticCtrl();
		/**
		 *\brief		Sets the static caption
		 *\param[in]	p_value	The new value
		 */
		void SetCaption( String const & p_value );
		/**
		 *\brief		Retrieve the static caption
		 *\return		The value
		 */
		inline String const & GetCaption()const
		{
			return m_caption;
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
		 *\copydoc		ProceduralTextures::Control::DoSetVisible
		 */
		virtual void DoSetVisible( bool p_visible );

	private:
		//! The static caption
		String m_caption;

	protected:
		//! The text overlay used to display the caption
		std::weak_ptr< TextOverlay > m_text;
		// The statics count
		static int m_count;
	};
}

#endif
