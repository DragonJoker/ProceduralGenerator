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
#ifndef ___GENERATOR_GL_BUFFER_H___
#define ___GENERATOR_GL_BUFFER_H___

#include "Vertex.h"
#include "Pixel.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Base class for the GPU buffer classes
	*/
	class GeneratorAPI GlBuffer
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGL instance
		 *\param[in]	p_target	The buffer target (GL_ARRAY_BUFFER, GL_ELEMENT_ARRY_BUFFER, ...)
		 *\param[in]	p_mode		The buffer mode (GL_STATIC_DRAW, ...)
		 */
		GlBuffer( OpenGl * p_pOpenGl, GLenum p_target, GLenum p_mode );
		/**
		 *\brief		Destructor
		 */
		virtual ~GlBuffer();
		/**
		 *\brief		Creates the buffer on GPU
		 *\return		true if the object is successfully created
		 */
		bool Create();
		/**
		 *\brief		Destroy the buffer on GPU
		 */
		void Destroy();
		/**
		 *\brief		Tries to bind the buffer on GPU
		 *\return		true if it is bound
		 */
		bool Bind();
		/**
		 *\brief		Unbinds the buffer on GPU
		 */
		void Unbind();
		/**
		 *\brief		Updates the buffer data on GPU
		 *\param[in]	p_buffer	The buffer data
		 *\param[in]	p_size		The buffer size
		 */
		void Data( void const * p_buffer, size_t p_size );
		/**
		 *\brief		Maps the buffer data into RAM memory
		 *\param[in]	p_access	The memory access
		 *\return		The mapped memory
		 */
		void * Lock( GLenum p_access );
		/**
		 *\brief		Unmaps the buffer data from RAM memory
		 */
		void Unlock();
		/**
		 *\brief		Initialises the buffer
		 *\return		true if it is successfully initialised
		 */
		virtual bool Initialise() = 0;
		/**
		 *\brief		Tries to activate the buffer
		 *\return		true if it is successfully activated
		 */
		virtual bool Activate() = 0;
		/**
		 *\brief		Deactivates the buffer
		 */
		virtual void Deactivate() = 0;
		/**
		 *\brief		Retrieves the buffer's OpenGL index
		 *\return		The value
		 */
		inline GLuint GetGlIndex()const
		{
			return m_uiGlIndex;
		}

	protected:
		//! The OpenGL instance
		OpenGl * m_pOpenGl;

	private:
		//! The OpenGL index for the GPU buffer
		GLuint m_uiGlIndex;
		//! The buffer target
		GLenum m_target;
		//! The buffer mode
		GLenum m_mode;
	};
}

//*************************************************************************************************

#endif
