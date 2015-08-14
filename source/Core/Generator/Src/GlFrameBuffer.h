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
#ifndef ___GENERATOR_GL_FRAME_BUFFER_H___
#define ___GENERATOR_GL_FRAME_BUFFER_H___

#include "GlObject.h"
#include "OpenGl.h"
#include "Size.h"

namespace ProceduralTextures
{
namespace gl
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		14/02/2010
	@brief
		Framebuffer implementation
	*/
	class GeneratorAPI FrameBuffer
		: public Object< std::function< bool( int, uint32_t * ) >, std::function< bool( int, uint32_t const * ) > >
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		*/
		FrameBuffer( OpenGl & p_openGl );

		/** Destructor
		*/
		virtual ~FrameBuffer();

		/** Initialises the framebuffer
		@param[in] p_size
			The wanted physical framebuffer size
		@return
			true if it is successfully initialised
		*/
		bool Initialise( Size const & p_size );

		/** Cleans up the framebuffer
		*/
		void Cleanup();

		/** Tries to activate the texture At given attachment
		@param[in] p_attachment
			The attachment
		@param[in] p_target
			The texture target (GL_TEXTURE0, ...)
		@return
			false if no texture is attached At wanted attachment or if it is not successfully activated
		*/
		bool ActivateTexture( unsigned int p_attachment, unsigned int p_target = GL_TEXTURE0 );

		/** Deactivate the texture At given attachment
		@param[in] p_attachment
			The attachment
		@param[in] p_target
			The texture target (GL_TEXTURE0, ...)
		*/
		void DeactivateTexture( unsigned int p_attachment, unsigned int p_target = GL_TEXTURE0 );

		/** Attaches a texture to the given attachment
		@param[in] p_attachment
			The attachment
		@param[in] p_texture
			The texture
		*/
		void AttachTexture( unsigned int p_attachment, std::shared_ptr< Texture > p_texture );

		/** Retrieves the texture to the given attachment
		@param[in] p_attachment
			The attachment
		@return
			The texture, NULL if there is no texture At given attachment
		*/
		std::shared_ptr< Texture > GetTexture( unsigned int p_attachment );

		/** Tries to download the image buffer for given attachment
		@param[in] p_attachment
			The attachment
		@param[out] p_buffer
			Receives the downloaded buffer
		@return
			false if no texture is attached At wanted attachment or if it is not successfully downloaded
		*/
		bool DownloadSync( unsigned int p_attachment, PixelBuffer & p_buffer );

		/** Tries to download the image buffer for given attachment
		@param[in] p_attachment
			The attachment
		@param[out] p_buffer
			Receives the downloaded buffer
		@return
			false if no texture is attached At wanted attachment or if it is not successfully downloaded
		*/
		bool DownloadAsync( unsigned int p_attachment, PixelBuffer & p_buffer );

		/** Updates the virtual framebuffer dimensions
		@param[in] p_size
			The new dimensions
		*/
		void Resize( Size const & p_size );

		/** Retrieves the virtual framebuffer dimensions
		@return
			The value
		*/
		Size const & GetSize()const
		{
			return m_size;
		}

	private:
		void DoCleanupPbos();
		void DoInitialisePbos();

		//! Tells if the framebuffer is initialised
		bool m_bInitialised;
		//! The physical framebuffer dimensions
		Size m_sizeImage;
		//! The virtual framebuffer dimensions
		Size m_size;
		//! The attached textures
		std::map< unsigned int, std::weak_ptr< Texture > > m_mapAttachments;
		//! The pixel transfer buffers, used to download the pixels
		std::unique_ptr< DownloadPixelBuffer > m_pDownloadBuffers[2];
		//! The currently active download pixel buffer
		int m_iCurrentDlPbo;
	};
}
}

//*************************************************************************************************

#endif
