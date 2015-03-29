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
#ifndef ___GENERATOR_GL_TEXTURE_H___
#define ___GENERATOR_GL_TEXTURE_H___

#include "GlObject.h"
#include "Pixel.h"
#include "OpenGl.h"
#include "Size.h"

namespace ProceduralTextures
{
namespace gl
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Texture implementation
	*/
	class GeneratorAPI Texture
		: public Object< std::function< bool( int, uint32_t * ) >, std::function< bool( int, uint32_t const * ) > >
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGL instance
		 */
		Texture( std::shared_ptr< OpenGl > p_openGl );
		/**
		 *\brief		Destructor
		 */
		~Texture();
		/**
		 *\brief		Initialises the texture from an external buffer
		 *\return		true if it is successfully initialised
		 */
		bool Initialise( std::shared_ptr< PixelBuffer > p_buffer );
		/**
		 *\brief		Initialises the texture from a size
		 *\return		true if it is successfully initialised
		 */
		bool Initialise( Size const & p_size );
		/**
		 *\brief		Cleans up the texture
		 */
		void Cleanup();
		/**
		 *\brief		Tries to activate the texture in given configuration
		 *\param[in]	p_texTarget	The texture target (GL_TEXTURE0, ...)
		 *\param[in]	p_bUpload	Tells if the image must be uploaded
		 */
		bool Activate( unsigned int p_texTarget = GL_TEXTURE0, bool p_bUpload = false );
		/**
		 *\brief		Deactivates the texture
		 *\param[in]	p_texTarget		The texture target (GL_TEXTURE0, ...)
		 */
		void Deactivate( unsigned int p_texTarget = GL_TEXTURE0 );
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
		void Resize( Size const & p_size );
		/**
		 *\brief		Retrieves the texture image buffer
		 *\return		The value
		 */
		inline PixelBuffer const & GetBuffer()const
		{
			return ( *m_pPixels );
		}
		/**
		 *\brief		Retrieves the texture image buffer
		 *\return		The value
		 */
		inline PixelBuffer & GetBuffer()
		{
			return ( *m_pPixels );
		}

	private:
		void DoInitialise();
		void DoDeleteBuffers();
		void DoCleanupPbos();
		void DoInitialisePbos();

	private:
		//! Tells if the texture is initialised
		bool m_bInitialised;
		//! The texture dimensions
		Size m_size;
		//! The texture buffer
		std::shared_ptr< PixelBuffer > m_pPixels;
		//! The pixel transfer buffers, used to upload the pixels
		std::array< std::unique_ptr< UploadPixelBuffer >, 2 > m_pUploadBuffers;
		//! The currently active upload pixel buffer
		int m_iCurrentUlPbo;
		//! The pixel transfer buffers, used to download the pixels
		std::array< std::unique_ptr< DownloadPixelBuffer >, 2 > m_pDownloadBuffers;
		//! The currently active download pixel buffer
		int m_iCurrentDlPbo;
		//! Tells if the texture has been resized
		bool m_resized;
	};
}
}

//*************************************************************************************************

#endif
