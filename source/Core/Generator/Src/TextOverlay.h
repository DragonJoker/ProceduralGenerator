/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option ) any later
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

#include "Pixel.h"
#include "GlVertexBuffer.h"
#include "GlIndexBuffer.h"
#include "GlTexture.h"
#include "GlFrameBuffer.h"

namespace ProceduralTextures
{
	typedef std::vector< uint8_t > ByteArray;

	/*!
	\author		Sylvain DOREMUS
	\date		15/05/2013
	\brief		Class used to display 2D text
	*/
	class GeneratorAPI TextOverlay
	{
	private:
		/*!
		\author		Sylvain DOREMUS
		\date		15/05/2013
		\brief		Class holding a glyph (id est a character bitmap)
		*/
		class Glyph
		{
		public:
			/**
			 *\brief		Default constructor
			 */
			Glyph();
			/**
			 *\brief		Copy assignment constructor
			 *\param[in]	p_glyph	The source object
			 */
			Glyph( const Glyph & p_glyph );
			/**
			 *\brief		Copy assignment operator
			 *\param[in]	p_glyph	The source object
			 *\return		A reference to this object
			 */
			Glyph & operator =( const Glyph & p_glyph );
			/**
			 *\brief		Destructor
			 */
			~Glyph();
			/**
			 *\brief		Retrieves the glyph dimensions
			 *\return		The current value
			 */
			inline const wxSize & GetSize()const
			{
				return m_size;
			}
			/**
			 *\brief		Retrieves the glyph position
			 *\return		The current value
			 */
			inline const wxPoint & GetPosition()const
			{
				return m_ptPosition;
			}
			/**
			 *\brief		Retrieves the glyph image
			 *\return		The current value
			 */
			inline const ByteArray & GetBitmap()const
			{
				return m_bitmap;
			}
			/**
			 *\brief		Sets the glyph image
			 *\param[in]	val	The new value
			 */
			inline void	SetBitmap( const ByteArray & val )
			{
				m_bitmap = val;
			}
			/**
			 *\brief		Sets the glyph position
			 *\param[in]	val	The new value
			 */
			inline void SetPosition( const wxPoint & val )
			{
				m_ptPosition = val;
			}
			/**
			 *\brief		Sets the glyph dimensions
			 *\param[in]	val	The new value
			 */
			inline void SetSize( const wxSize & val )
			{
				m_size = val;
			}

		private:
			//! The glyph position
			wxPoint m_ptPosition;
			//! The glyph dimensions
			wxSize m_size;
			//! The glyph image
			ByteArray m_bitmap;
		};
		//! Typedef on a glyph vector
		typedef std::vector< Glyph > GlyphArray;

	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 */
		TextOverlay( OpenGl * p_pOpenGl );
		/**
		 *\brief		Destructor
		 */
		~TextOverlay();
		/**
		 *\brief		All OpenGL global initialisations must be made in this function
		 */
		void Initialise();
		/**
		 *\brief		All OpenGL global cleanups must be made in this function
		 */
		void Cleanup();
		/**
		 *\brief		Retrieves font's glyph and initialises the overlay to given parameters
		 *\param[in]	p_iHeight	The font pixel height
		 *\param[in]	p_eStyle	The font style
		 *\param[in]	p_eWeight	The font weight
		 */
		void Create( int p_iHeight, wxFontStyle p_eStyle, wxFontWeight p_eWeight );
		/**
		 *\brief		Draws the text overlay into a given frame buffer
		 *\param[in]	p_frameBuffer	The frame buffer that receives the rendering
		 */
		void Draw( GlFrameBuffer & p_frameBuffer );
		/**
		 *\brief		Defines the text, tells it is modified
		 *\param[in]	p_strText	The new value
		 */
		inline void	SetText( const wxString & p_strText )
		{
			m_strText = p_strText;
			m_bTextModified = true;
		}
		/**
		 *\brief		Defines the overlay size, sets it modified
		 *\param[in]	p_size	The new value
		 */
		inline void	SetSize( const wxSize & p_size )
		{
			m_size = p_size;
			m_bMatrixModified = true;
		}
		/**
		 *\brief		Defines the overlay size, sets it modified
		 *\param[in]	p_size	The new value
		 */
		inline void	SetPosition( const wxPoint & p_position )
		{
			m_position = p_position;
			m_bMatrixModified = true;
		}
		/**
		 *\brief		Defines the overlay background colour
		 *\param[in]	p_colour	The new value
		 */
		void SetBackground( wxColour const & p_colour );
		/**
		 *\brief		Defines the overlay foreground colour
		 *\param[in]	p_colour	The new value
		 */
		void SetForeground( wxColour const & p_colour );
		/**
		 *\brief		Defines the overlay background image
		 *\param[in]	p_image	The new value
		 */
		void SetBackground( wxImage const & p_image );
		/**
		 *\brief		Defines the overlay foreground image
		 *\param[in]	p_image	The new value
		 */
		void SetForeground( wxImage const & p_image );

	private:
		void DoSubRender();

	private:
		//! The OpenGl instance
		OpenGl * m_pOpenGl;
		//! The vertices buffer
		GlVertexBuffer m_vtxBuffer;
		//! The indices buffer
		GlIndexBuffer m_idxBuffer;
		//! The background texture, may be an image or a colour or nothing
		GlTexture m_textureBackground;
		//! Tells if the background has been modified
		bool m_bBackgroundModified;
		//! The foreground texture, may be an image or a colour or nothing
		GlTexture m_textureForeground;
		//! Tells if the foreground has been modified
		bool m_bForegroundModified;
		//! The currently displayed text texture
		GlTexture m_textureText;
		//! The background texture with the text cut into it
		GlTexture m_textureBackgroundCut;
		//! Frame buffer receiving the result of background - text
		GlFrameBuffer m_frameBufferBackgroundCut;
		//! Texture receiving the result of (text * foreground )
		GlTexture m_textureBlendedText;
		//! Frame buffer receiving the result of (text * foreground )
		GlFrameBuffer m_frameBufferBlendedText;
		//! The texture bound to the result frame buffer
		GlTexture m_textureResult;
		//! Frame buffer receiving the result of (background - text ) + (text * foreground )
		GlFrameBuffer m_frameBufferResult;
		//! The font glyphs
		GlyphArray m_arrayGlyphs;
		//! The current overlay text
		wxString m_strText;
		//! Tells if the text has been modified and needs redraw
		bool m_bTextModified;
		//! The maximum height, for line ends
		int m_iMaxHeight;
		//! Blend mode for text colour
		GlTexture::BlendMode m_blendRgbTxt;
		//! Blend mode for text alpha
		GlTexture::BlendMode m_blendAlphaTxt;
		//! Blend mode for background colour
		GlTexture::BlendMode m_blendRgbImg;
		//! Blend mode for background alpha
		GlTexture::BlendMode m_blendAlphaImg;
		//! The overlay dimensions
		wxSize m_size;
		//! The overlay position
		wxPoint m_position;
		//! Tells if the matrix has been modified and needs recomputing
		bool m_bMatrixModified;
		//! Transformation matrix
		GLfloat m_matrix[16];
	};
}

#endif
