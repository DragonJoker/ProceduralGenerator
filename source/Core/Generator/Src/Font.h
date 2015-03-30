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
#ifndef ___GENERATOR_FONT_H___
#define ___GENERATOR_FONT_H___

#include "Point.h"
#include "Glyph.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\version	2.0.0
	\date		23/02/2015
	\brief		Font resource
	\remark		Representation of a font : face, precision, and others
				<br />The generated font will be put in an image
	*/
	class GeneratorAPI Font
	{
	public:
		/*!
		\author		Sylvain DOREMUS
		\version	2.0.0
		\date		23/02/2015
		\brief		Structure used to load glyphs on demand
		\remark		PImpl to hide FreeType inclusions
		*/
		struct SFontImpl
		{
			/**
			 *\brief		Initialises the loader
			 */
			virtual void Initialise() = 0;
			/**
			 *\brief		Cleans the loader up
			 */
			virtual void Cleanup() = 0;
			/**
			 *\brief		Loads wanted glyph
			 *\param[in]	p_glyph	The glyph
			 *\return		The glyph
			 */
			virtual int LoadGlyph( Glyph & p_glyph ) = 0;
		};

		typedef std::map< char32_t, Glyph > GlyphMap;

	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_path		The font file path
		 *\param[in]	p_height	The font char height
		 */
		Font( String const & p_path, uint32_t p_height );
		/**
		 *\brief		Destructor
		 */
		~Font();
		/**
		 *\brief		Sets the glyph for given character
		 *\param[in]	p_char		The character
		 *\param[in]	p_size		The glyph dimensions
		 *\param[in]	p_position	The glyph position
		 *\param[in]	p_advance	Pixels to advance in order to go next character
		 *\param[in]	p_bitmap	The glyph image
		 */
		void SetGlyphAt( char32_t p_char, Size const & p_size, Position p_position, Size const & p_advance, std::vector< UbPixel > const & p_bitmap );
		/**
		 *\brief		Loads the glyph for given character
		 *\param[in]	p_char	The character
		 */
		void LoadGlyph( char32_t p_char );
		/**
		 *\brief		Retrieves the glyph of wanted chaaracter
		 *\param[in]	p_char The wanted character
		 *\return		A constant reference on the glyph
		 */
		inline Glyph const & GetGlyphAt( char32_t p_char )const
		{
			auto && l_it = m_loadedGlyphs.find( p_char );

			if ( l_it != m_loadedGlyphs.end() )
			{
				return l_it->second;
			}

			throw std::out_of_range( "character not found" );
		}
		/**
		 *\brief		Retrieves the glyph of wanted chaaracter
		 *\param[in]	p_char The wanted character
		 *\return		A reference on the glyph
		 */
		inline Glyph & GetGlyphAt( char32_t p_char )
		{
			auto && l_it = m_loadedGlyphs.find( p_char );

			if ( l_it != m_loadedGlyphs.end() )
			{
				return l_it->second;
			}

			throw std::out_of_range( "character not found" );
		}
		/**
		 *\brief		Retrieves the glyph of wanted chaaracter
		 *\param[in]	p_char The wanted character
		 *\return		A constant reference on the glyph
		 */
		inline Glyph const & operator[]( char32_t p_char )const
		{
			auto && l_it = m_loadedGlyphs.find( p_char );

			if ( l_it != m_loadedGlyphs.end() )
			{
				return l_it->second;
			}

			throw std::out_of_range( "character not found" );
		}
		/**
		 *\brief		Retrieves the glyph of wanted chaaracter
		 *\param[in]	p_char The wanted character
		 *\return		A reference on the glyph
		 */
		inline Glyph & operator[]( char32_t p_char )
		{
			auto && l_it = m_loadedGlyphs.find( p_char );

			if ( l_it != m_loadedGlyphs.end() )
			{
				return l_it->second;
			}

			throw std::out_of_range( "character not found" );
		}
		/**
		 *\brief		Retrieves the height of the font
		 *\return		The font height
		 */
		inline uint32_t GetHeight()const
		{
			return m_uiHeight;
		}
		/**
		 *\brief		Retrieves the max height of the glyphs
		 *\return		The glyphs max height
		 */
		inline int GetMaxHeight()const
		{
			return m_iMaxHeight;
		}
		/**
		 *\brief		Retrieves the max width of the glyphs
		 *\return		The glyphs max width
		 */
		inline int GetMaxWidth()const
		{
			return m_iMaxWidth;
		}
		/**
		 *\brief		Sets the max height of the glyphs
		 *\param[in]	p_iHeight	The new height
		 */
		inline void SetMaxHeight( int p_iHeight )
		{
			m_iMaxHeight = p_iHeight;
		}
		/**
		 *\brief		Sets the max width of the glyphs
		 *\param[in]	p_iWidth	The new width
		 */
		inline void SetMaxWidth( int p_iWidth )
		{
			m_iMaxWidth = p_iWidth;
		}
		/**
		 *\brief		Retrieves an iterator on the first glyph
		 *\return		The iterator
		 */
		inline GlyphMap::iterator Begin()
		{
			return m_loadedGlyphs.begin();
		}
		/**
		 *\brief		Retrieves an iterator on the first glyph
		 *\return		The iterator
		 */
		inline GlyphMap::const_iterator Begin()const
		{
			return m_loadedGlyphs.begin();
		}
		/**
		 *\brief		Retrieves an iterator on the glyph array end
		 *\return		The iterator
		 */
		inline GlyphMap::iterator End()
		{
			return m_loadedGlyphs.end();
		}
		/**
		 *\brief		Retrieves an iterator on the glyph array end
		 *\return		The iterator
		 */
		inline GlyphMap::const_iterator End()const
		{
			return m_loadedGlyphs.end();
		}

	protected:
		//! The height of the font
		uint32_t m_uiHeight;
		//! The font file, in memory
		std::vector< uint8_t > m_fileBuffer;
		//! The array of loaded glyphs
		GlyphMap m_loadedGlyphs;
		//! The max height of the glyphs
		int m_iMaxHeight;
		//! The max top of the glyphs
		int m_iMaxTop;
		//! The max width of the glyphs
		int m_iMaxWidth;
	};
}

#endif
