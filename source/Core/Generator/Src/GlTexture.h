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
#ifndef ___GENERATOR_GL_TEXTURE_H___
#define ___GENERATOR_GL_TEXTURE_H___

#include "GeneratorPrerequisites.h"
#include "Pixel.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Texture implementation
	*/
	class GeneratorAPI GlTexture
	{
	public:
		/*!
		\author		Sylvain DOREMUS
		\date		14/02/2010
		\brief		The blend mode structure, used to implement multitexturing combinations
		*/
		struct BlendMode
		{
			//! The blending function
			GLenum function;
			//! The first source
			GLenum source1;
			//! The second source
			GLenum source2;
			//! The third source
			GLenum source3;
			//! The first operand
			GLenum operand1;
			//! The second operand
			GLenum operand2;
			//! The third operand
			GLenum operand3;
		};

	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGL instance
		 */
		GlTexture( OpenGl * p_pOpenGl );
		/**
		 *\brief		Destructor
		 */
		~GlTexture();
		/**
		 *\brief		Creates the texture on GPU
		 *\return		true if the object is successfully created
		 */
		bool Create();
		/**
		 *\brief		Destroy the texture on GPU
		 */
		void Destroy();
		/**
		 *\brief		Initialises the texture from an external buffer
		 *\remarks		The texture doesn't own its buffer
		 *\return		true if it is successfully initialised
		 */
		bool Initialise( PixelBuffer * p_buffer );
		/**
		 *\brief		Initialises the texture from a size
		 *\remarks		The texture owns its buffer
		 *\return		true if it is successfully initialised
		 */
		bool Initialise( const wxSize & p_size );
		/**
		 *\brief		Cleans up the texture
		 */
		void Cleanup();
		/**
		 *\brief		Tries to activate the texture in given configuration
		 *\param[in]	p_texTarget		The texture target (GL_TEXTURE0, ...)
		 *\param[in]	p_mode			The texture environment mode (GL_MODULATE, ...)
		 *\param[in]	p_blendRgb		The blending operations for RGB components
		 *\param[in]	p_blendAlpha	The blending operations for Alpha component
		 *\param[in]	p_bUpload		Tells if the image must be uploaded
		 */
		bool Activate( GLenum p_texTarget = GL_TEXTURE0, GLenum p_mode = GL_MODULATE, BlendMode * p_blendRgb = NULL, BlendMode * p_blendAlpha = NULL, bool p_bUpload = false );
		/**
		 *\brief		Deactivates the texture
		 *\param[in]	p_texTarget		The texture target (GL_TEXTURE0, ...)
		 */
		void Deactivate( GLenum p_texTarget = GL_TEXTURE0 );
		/**
		 *\brief		Uploads the image pixels in asynchonous mode
		 */
		void UploadAsync();
		/**
		 *\brief		Uploads the image pixels in synchonous mode
		 */
		void UploadSync();
		/**
		 *\brief		Downloads the image pixels in asynchronous mode
		 */
		void DownloadAsync();
		/**
		 *\brief		Downloads the image pixels in synchronous mode
		 */
		void DownloadSync();
		/**
		 *\brief		Resets the texture dimensions
		 *\remarks		The texture must be initialised again, after that call
		 *\param[in]	p_size	The new dimensions
		 */
		void Resize( const wxSize & p_size );
		/**
		 *\brief		Retrieves the texture OpenGL index
		 *\return		The value
		 */
		inline GLuint GetGlIndex()const
		{
			return m_uiGlTexture;
		}
		/**
		 *\brief		Retrieves the texture image buffer
		 *\return		The value
		 */
		inline const PixelBuffer & GetBuffer()const
		{
			return ( * m_pPixels );
		}
		/**
		 *\brief		Retrieves the texture image buffer
		 *\return		The value
		 */
		inline PixelBuffer 	& GetBuffer()
		{
			return ( * m_pPixels );
		}
		/**
		 *\brief		Defines the texture blend colour
		 *\param[in]	p_value	The new value
		 */
		inline void SetColour( const UbPixel & p_value )
		{
			m_pxColour.Set( p_value.r / 255.0f, p_value.g / 255.0f, p_value.b / 255.0f, p_value.a / 255.0f );
		}
		/**
		 *\brief		Defines the texture blend colour
		 *\param[in]	p_value	The new value
		 */
		inline void SetColour( const Pixel< float > & p_value )
		{
			m_pxColour = p_value;
		}

	private:
		void DoInitialise();
		void DoDeleteBuffers();
		void DoCleanupPbos();
		void DoInitialisePbos();

	private:
		//! The OpenGL instance
		OpenGl * m_pOpenGl;
		//! The texture index on GPU
		GLuint m_uiGlTexture;
		//! Tells if the texture is initialised
		bool m_bInitialised;
		//! The texture dimensions
		wxSize m_size;
		//! The texture buffer
		PixelBuffer * m_pPixels;
		//! Tells if the texture owns its buffer
		bool m_bOwnsPixels;
		//! The pixel transfer buffers, used to upload the pixels
		GlUploadPixelBuffer * m_pUploadBuffers[2];
		//! The currently active upload pixel buffer
		int m_iCurrentUlPbo;
		//! The pixel transfer buffers, used to download the pixels
		GlDownloadPixelBuffer * m_pDownloadBuffers[2];
		//! The currently active download pixel buffer
		int m_iCurrentDlPbo;
		//! The blending colour
		Pixel< float > m_pxColour;
	};
}

//*************************************************************************************************

#endif
