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
#ifndef ___GENERATOR_GL_FRAME_BUFFER_H___
#define ___GENERATOR_GL_FRAME_BUFFER_H___

#include "GeneratorPrerequisites.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Framebuffer implementation
	*/
	class GeneratorAPI GlFrameBuffer
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 */
		GlFrameBuffer( OpenGl * p_pOpenGl );
		/**
		 *\brief		Destructor
		 */
		virtual ~GlFrameBuffer();
		/**
		 *\brief		Creates the framebuffer on GPU
		 *\return		true if the object is successfully created
		 */
		bool Create();
		/**
		 *\brief		Destroy the buffer on GPU
		 */
		void Destroy();
		/**
		 *\brief		Initialises the framebuffer
		 *\param[in]	p_size	The wanted physical framebuffer size
		 *\return		true if it is successfully initialised
		 */
		bool Initialise( wxSize const & p_size );
		/**
		 *\brief		Cleans up the framebuffer
		 */
		void Cleanup();
		/**
		 *\brief		Tries to activate the framebuffer
		 *\return		true if it is successfully activated
		 */
		bool Activate();
		/**
		 *\brief		Deactivates the framebuffer
		 */
		void Deactivate();
		/**
		 *\brief		Tries to activate the texture at given attachment
		 *\param[in]	p_attachment	The attachment
		 *\return		false if no texture is attached at wanted attachment or if it is not successfully activated
		 */
		bool ActivateTexture( GLenum p_attachment );
		/**
		 *\brief		Deactivate the texture at given attachment
		 *\param[in]	p_attachment	The attachment
		 */
		void DeactivateTexture( GLenum p_attachment );
		/**
		 *\brief		Attaches a texture to the given attachment
		 *\param[in]	p_attachment	The attachment
		 *\param[in]	p_texture		The texture
		 */
		void AttachTexture( GLenum p_attachment, GlTexture * p_pTexture );
		/**
		 *\brief		Retrieves the texture to the given attachment
		 *\param[in]	p_attachment	The attachment
		 *\return		The texture, NULL if there is no texture at given attachment
		 */
		GlTexture * GetTexture( GLenum p_attachment );
		/**
		 *\brief		Tries to download the image buffer for given attachment
		 *\param[in]	p_attachment	The attachment
		 *\param[out]	p_buffer		Receives the downloaded buffer
		 *\return		false if no texture is attached at wanted attachment or if it is not successfully downloaded
		 */
		bool DownloadSync( GLenum p_attachment, PixelBuffer & p_buffer );
		/**
		 *\brief		Tries to download the image buffer for given attachment
		 *\param[in]	p_attachment	The attachment
		 *\param[out]	p_buffer		Receives the downloaded buffer
		 *\return		false if no texture is attached at wanted attachment or if it is not successfully downloaded
		 */
		bool DownloadAsync( GLenum p_attachment, PixelBuffer & p_buffer );
		/**
		 *\brief		Updates the virtual framebuffer dimensions
		 *\param[in]	p_size	The new dimensions
		 */
		void Resize( const wxSize & p_size );
		/**
		 *\brief		Retrieves the virtual framebuffer dimensions
		 *\return		The value
		 */
		wxSize const & GetSize()const
		{
			return m_size;
		}

	private:
		void DoCleanupPbos();
		void DoInitialisePbos();

		//! The OpenGL instance
		OpenGl * m_pOpenGl;
		//! The OpenGL index for the GPU framebuffer
		GLuint m_uiGlBuffer;
		//! Tells if the framebuffer is initialised
		bool m_bInitialised;
		//! The physical framebuffer dimensions
		wxSize m_sizeImage;
		//! The virtual framebuffer dimensions
		wxSize m_size;
		//! The attached textures
		std::map< GLenum, GlTexture * > m_mapAttachments;
		//! The pixel transfer buffers, used to download the pixels
		GlDownloadPixelBuffer * m_pDownloadBuffers[2];
		//! The currently active download pixel buffer
		int m_iCurrentDlPbo;
	};
}

//*************************************************************************************************

#endif
