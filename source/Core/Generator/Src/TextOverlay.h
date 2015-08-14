/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

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
#ifndef ___GENERATOR_TEXT_OVERLAY_H___
#define ___GENERATOR_TEXT_OVERLAY_H___

#include "Overlay.h"
#include "Vertex.h"
#include "GlTexture.h"

#pragma warning( push )
#pragma warning( disable:4251 )
#pragma warning( disable:4275 )

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
		Horizontal alignments for text overlays
	*/
	typedef enum eHALIGN
	{
		//! Aligned on the left
		eHALIGN_LEFT,
		//! Centered horizontally
		eHALIGN_CENTER,
		//! Aligned on the right
		eHALIGN_RIGHT,
	}	eHALIGN;

	/*!
	@author
		Sylvain DOREMUS
	@date
		02/03/2015
	@version
		2.0.0
	@brief
		Vertical alignments
	*/
	typedef enum eVALIGN
	{
		//! Aligned on the top
		eVALIGN_TOP,
		//! Centered vertically
		eVALIGN_CENTER,
		//! Aligned on the bottom
		eVALIGN_BOTTOM,
	}	eVALIGN;

	/*!
	@author
		Sylvain DOREMUS
	@date
		23/02/2015
	@version
		2.0.0
	@brief
		A character in a text overlay : position, size and value
	*/
	class GeneratorAPI Character
	{
	public:
		/** Constructor
		@param[in] p_value
			The character value
		@param[in] p_position
			The character position
		@param[in] p_size
			The character size
		*/
		Character( char32_t p_value, Position const & p_position, Size const & p_size )
			: m_value( p_value )
			, m_position( p_position )
			, m_size( p_size )
		{
		}

		/** Destructor
		*/
		virtual ~Character()
		{
		}

		/** Retrieves the character value
		@return
			The value
		*/
		inline char32_t GetValue()const
		{
			return m_value;
		}

		/** Retrieves the character position
		@return
			The value
		*/
		inline Position const & GetPosition()const
		{
			return m_position;
		}

		/** Retrieves the character size
		@return
			The value
		*/
		inline Size const & GetSize()const
		{
			return m_size;
		}

	private:
		//! The character value
		char32_t m_value;
		//! The character position
		Position m_position;
		//! The character size
		Size m_size;
	};
	/*!
	@author
		Sylvain DOREMUS
	@date
		23/02/2015
	@version
		2.0.0
	@brief
		An overlay with a text
	*/
	class GeneratorAPI TextOverlay
		:	public Overlay
	{
	public:
		typedef std::map< char32_t, Position > GlyphPositionMap;
		typedef std::vector< Character > CharacterArray;

	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGl instance
		@param[in] p_material
			The material
		@param[in] p_parent
			The parent overlay (if any)
		*/
		TextOverlay( gl::OpenGl & p_openGl, Material const & p_material, std::shared_ptr< Overlay > p_parent );

		/** Destructor
		*/
		virtual ~TextOverlay();

		/** Sets the text font
		@param[in] p_font
			The new value
		*/
		void SetFont( std::shared_ptr< Font > p_font );

		/** Sets the shader program
		@param[in] p_type
			The new value
		@param[in] p_program
			The new value
		*/
		virtual void SetProgram( eMATERIAL_TYPE p_type, std::shared_ptr< gl::ShaderProgram > p_program );

		/** Retrieves the wanted glyph position
		@param[in] p_char
			The glyph index
		@return
			The position
		*/
		Position const & GetGlyphPosition( char32_t p_char )const;

		/** Retrieves the font
		@return
			The value
		*/
		std::shared_ptr< Font > GetFont()const
		{
			return m_wpFont.lock();
		}

		/** Retrieves the overlay text
		@return
			The value
		*/
		inline String const & GetCaption()const
		{
			return m_caption;
		}

		/** Retrieves the horizontal alignment
		@return
			The value
		*/
		inline eHALIGN GetHAlign()const
		{
			return m_hAlign;
		}

		/** Retrieves the vertical alignment
		@return
			The value
		*/
		inline eVALIGN GetVAlign()const
		{
			return m_vAlign;
		}

		/** Sets the overlay text
		@param[in] p_caption
			The new value
		*/
		inline void SetCaption( String const & p_caption )
		{
			m_changed = m_caption != p_caption;
			m_caption = p_caption;
		}

		/** Sets text wrapping mode
		@param[in] p_mode
			The new value
		*/
		inline void SetTextWrappingMode( eTEXT_WRAPPING_MODE p_mode )
		{
			m_changed = m_wrappingMode != p_mode;
			m_wrappingMode = p_mode;
		}

		/** Sets the horizontal alignment
		@param[in] p_align
			The new value
		*/
		inline void SetHAlign( eHALIGN p_align )
		{
			m_changed = m_hAlign != p_align;
			m_hAlign = p_align;
		}

		/** Sets the vertical alignment
		@param[in] p_align
			The new value
		*/
		inline void SetVAlign( eVALIGN p_align )
		{
			m_changed = m_vAlign != p_align;
			m_vAlign = p_align;
		}

		/** Retrieves an iterator to the beginning of the characters array
		@return
			The iterator
		*/
		inline CharacterArray::const_iterator Begin()const
		{
			return m_characters.begin();
		}

		/** Retrieves an iterator to the end of the characters array
		@return
			The iterator
		*/
		inline CharacterArray::const_iterator End()const
		{
			return m_characters.end();
		}

	protected:
		/** Initialises the overlay and its buffers
		*/
		virtual void DoInitialise();

		/** Cleans the overlay and its buffers up
		*/
		virtual void DoCleanup();

		/** Draws the overlay
		*/
		virtual void DoRender();

		/** Updates the vertex buffer
		*/
		virtual void DoUpdate();

		/** Updates the overlay position and size, taking care of wanted pixel size and position
		*/
		virtual void DoUpdatePositionAndSize( Size const & p_size );

		/** Adds a word to the vertex buffer
		@param[in] p_word
			The word to add
		@param[in] p_wordWidth
			The word width
		@param[in] p_position
			The word position
		@param[in] p_size
			The overlay size
		@param[out] p_lineWidth
			The line width
		@param[out] p_lineVtx
			The line
		@param[out] p_linesVtx
			the lines
		*/
		void DoWriteWord( String const & p_word, int32_t p_wordWidth, Size const & p_size, Position & p_position, int32_t & p_lineWidth, std::vector< VertexI > & p_lineVtx, std::vector< std::vector< VertexI > > & p_linesVtx );

		/** Horizontally align a line
		@param[in] p_width
			The overlay width
		@param[out] p_lineWidth
			The line width
		@param[out] p_lineVtx
			The line
		@param[out] p_linesVtx
			the lines
		*/
		void DoAlignHorizontally( int32_t p_width, int32_t & p_lineWidth, std::vector< VertexI > & p_lineVtx, std::vector< std::vector< VertexI > > & p_linesVtx );

		/** Vertically align a line
		@param[in] p_height
			The overlay width
		@param[out] p_linesHeight
			The lines height
		@param[out] p_linesVtx
				the lines
		*/
		void DoAlignVertically( int32_t p_height, int32_t p_linesHeight, std::vector< std::vector< VertexI > > & p_linesVtx );

	protected:
		//! The current overlay caption
		String m_caption;
		//! The previous overlay caption
		String m_previousCaption;
		//! The font
		std::weak_ptr< Font > m_wpFont;
		//! The texture that will receive the glyphs
		gl::Texture m_glyphsTexture;
		//! The uniform containing the text sampler
		std::weak_ptr< gl::FrameVariable< int > > m_uniformTextTexture;
		//! Glyphs positions in the texture
		GlyphPositionMap m_glyphsPositions;
		//! The wrapping mode
		eTEXT_WRAPPING_MODE m_wrappingMode;
		//! The horizontal alignment
		eHALIGN m_hAlign;
		//! The vertical alignment
		eVALIGN m_vAlign;
		//! The previous display size
		Size m_previousDisplaySize;
		//! The characters array
		CharacterArray m_characters;
	};
}

#pragma warning( pop )

#endif
