/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___TFX_GPU_STEP_H___
#define ___TFX_GPU_STEP_H___

#include <GpuStep.h>

namespace TextEffects
{
	/*!
	@author
		Sylvain DOREMUS
	\version	2.0.0
	@date
		23/02/2015
	@brief
		The common variables for shader programs
	*/
	struct Program
	{
		/** Constructor
		@param[in] p_gl
			The OpenGl instance
		 */
		Program( std::shared_ptr< ProceduralTextures::gl::OpenGl > p_gl );
		/** Loads the two GLSL programs in parameter, initialises the uniforms an vertex attributes
		@param[in] p_vertex
			The vertex program
		@param[in] p_pixel
			The pixel program
		 */
		void Initialise( ProceduralTextures::String const & p_vertex, ProceduralTextures::String const & p_pixel );
		/** Cleans up the program
		 */
		void Cleanup();

		// !The shader program
		std::shared_ptr< ProceduralTextures::gl::ShaderProgram > m_program;
		// The uniform holding the background image index
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformBackground;
		// The uniform holding the text mask image index
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformTextMask;
		//! The vertex attribute "vertex" location in the program
		uint32_t m_vertex;
		//! The vertex attribute "texture" location in the program
		uint32_t m_texture;
	};
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Text effects GPU step
	*/
	class GpuStep
		:	public ProceduralTextures::GpuStep
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The displayed surface dimensions
		@param[in] p_bordersSize
			The windows' borders size
		 */
		GpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize );
		/** Destructor
		 */
		virtual ~GpuStep();
		/** Sets the text font file
		@param[in] p_value
			The new value
		 */
		inline void SetFont( ProceduralTextures::String const & p_strFont )
		{
			m_strFontFile = p_strFont;
			m_bFontModified = true;
		}
		/** Sets the text
		@param[in] p_value
			The new value
		 */
		inline void	SetText( ProceduralTextures::String const & p_strText )
		{
			m_strText = p_strText;
			m_bTextModified = true;
		}
		/** Sets the foreground type
		@param[in] p_value
			The new value
		 */
		void SetForegroundType( int p_value );
		/** Sets the background image
		@param[in] p_value
			The new value
		 */
		void SetBackgroundImage( ProceduralTextures::PixelBuffer const & p_value );
		/** Sets the foreground colour
		@param[in] p_value
			The new value
		 */
		void SetForegroundColour( uint32_t p_value );
		/** Sets the foreground image
		@param[in] p_value
			The new value
		 */
		void SetForegroundImage( ProceduralTextures::PixelBuffer const & p_value );
		/** Sets the background image
		@param[in] p_value
			The new value
		 */
		void OnSize( ProceduralTextures::Size const & p_value );

	private:
		/** @copydoc ProceduralTexture::GpuStep::DoInitialise
		 */
		virtual void DoInitialise();
		/** @copydoc ProceduralTexture::GpuStep::DoCleanup
		 */
		virtual void DoCleanup();
		/** @copydoc ProceduralTexture::GpuStep::DoResize
		 */
		virtual void DoResize();
		/** @copydoc ProceduralTexture::GpuStep::DoPreRender
		 */
		virtual void DoPreRender();
		/** @copydoc ProceduralTexture::GpuStep::DoRender
		 */
		virtual void DoRender( bool & p_bChanged );
		/** @copydoc ProceduralTexture::GpuStep::DoPostRender
		 */
		virtual void DoPostRender();
		/** Loads the font file, puts the loaded glyphs in the member glyphs array
		 */
		void DoLoadFont();
		/** Cleans up the glyphs array
		 */
		void DoCleanupGlyphs();
		/** Updates the text mask texture
		 */
		void DoUpdateText();

	private:
		//! The current text
		ProceduralTextures::String m_strText;
		//! The previous text (in fact the displayed one)
		ProceduralTextures::String m_strPreviousText;
		//! The font file path
		ProceduralTextures::String m_strFontFile;
		//! The font
		std::weak_ptr< ProceduralTextures::Font > m_font;
		// Tells that the font has been modified
		bool m_bFontModified;
		//! Tells that the text has changed
		bool m_bTextModified;
		//! The maximum glyphs top position
		int m_iMaxTop;
		//! The background texture
		std::shared_ptr< ProceduralTextures::gl::Texture > m_textureBackground;
		//! The maske, used to display the text
		std::shared_ptr< ProceduralTextures::gl::Texture > m_textureTextMask;
		//! The foreground colour
		ProceduralTextures::UbPixel m_colourForeground;
		//! The foreground texture
		std::shared_ptr< ProceduralTextures::gl::Texture > m_textureForeground;
		//! Tells that the background texture has changed
		bool m_newBackgroundTexture;
		//! Tells that the foreground texture has changed
		bool m_newForegroundTexture;
		ProceduralTextures::eMATERIAL_TYPE m_foregroundType;
		//! The shader program used when the text mode is colour
		std::shared_ptr< Program > m_programColour;
		// The uniform holding the foreground colour
		std::weak_ptr< ProceduralTextures::gl::Vec4FrameVariable< float > > m_uniformColourForeground;
		//! The shader program used when the text mode is texture
		std::shared_ptr< Program > m_programTexture;
		// The uniform holding the foreground texture index
		std::weak_ptr< ProceduralTextures::gl::FrameVariable< int > > m_uniformTextureForeground;
		//! The active shader program
		std::weak_ptr< Program > m_currentProgram;
	};
}

#endif
