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
#ifndef ___GENERATOR_GLYPH_H___
#define ___GENERATOR_GLYPH_H___

#include "Position.h"
#include "Size.h"
#include "Pixel.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@version
		2.0.0
	@date
		23/02/2015
	@brief
		Font character (glyph) representation
	@remarks
		Holds position, size and data of a character
	*/
	class GeneratorAPI Glyph
	{
	public:
		/** Constructor
		@param[in] p_char
			The glyph character
		@param[in] p_size
			The glyph dimensions
		@param[in] p_position
			The glyph position
		@param[in] p_advance
			Pixels to advance in order to go next character
		@param[in] p_bitmap
			The glyph image
		 */
		Glyph( char32_t p_char = 0, Size const & p_size = Size(), Position const & p_position = Position(), Size const & p_advance = Size(), std::vector< UbPixel > const & p_bitmap = std::vector< UbPixel >() );

		/** Destructor
		 */
		~Glyph();

		/** Retrieves the glyph character
		@return
			The value
		 */
		inline char32_t GetCharacter()const
		{
			return m_character;
		}

		/** Sets the glyph dimensions
		@param[in] p_size
			The glyph dimensions
		 */
		inline void SetSize( Size const & p_size )
		{
			m_size = p_size;
		}

		/** Retrieves the glyph dimensions
		@return
			The glyph dimensions
		 */
		inline Size const & GetSize()const
		{
			return m_size;
		}

		/** Sets the glyph position
		@param[in] p_position
			The glyph position
		 */
		inline void SetPosition( Position const & p_position )
		{
			m_position = p_position;
		}

		/** Retrieves the glyph position
		@return
			The glyph position
		 */
		inline Position const & GetPosition()const
		{
			return m_position;
		}

		/** Sets the number of pixels to go before drawing next glyph
		@param[in] p_advance
			The value
		 */
		inline void SetAdvance( Size const & p_advance )
		{
			m_advance = p_advance;
		}

		/** Retrieves the number of pixels to go before drawing next glyph
		@return
			The value
		 */
		inline Size const & GetAdvance()const
		{
			return m_advance;
		}

		/** Sets the glyph image
		@param[in] p_bitmap
			The image of the glyph
		 */
		inline void SetBitmap( std::vector< UbPixel > const & p_bitmap )
		{
			m_bitmap = p_bitmap;
		}

		/** Retrieves the glyph image
		@return
			A constant reference to the image of the glyph
		 */
		inline std::vector< UbPixel > const & GetBitmap()const
		{
			return m_bitmap;
		}

		/** Adjusts the glyph position
		@param[in] x, y
			The glyph position decal
		 */
		inline void AdjustPosition( uint32_t x, uint32_t y )
		{
			m_position += Position( x, y );
		}

	private:
		//! Glyph position, in texture
		Position m_position;
		//! Glyph dimensions
		Size m_size;
		//! Pixels to advance in order to go next character
		Size m_advance;
		//! Glyph image
		std::vector< UbPixel > m_bitmap;
		//! Glyph character
		char32_t m_character;
	};
}

#endif
