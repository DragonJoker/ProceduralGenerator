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
#ifndef ___GENERATOR_GL_INDEX_BUFFER_H___
#define ___GENERATOR_GL_INDEX_BUFFER_H___

#include "GlBufferObject.h"

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
		Index buffer object implementation
	*/
	class GeneratorAPI IndexBuffer
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_mode
				The buffer access mode
		*/
		IndexBuffer( OpenGl & p_openGl, uint32_t p_mode );

		/** Destructor
		*/
		~IndexBuffer();

		/** Creates and initialises the buffer
		@return
			true if it is successfully initialised
		*/
		bool Initialise();

		/** Destroys the buffer
		*/
		void Cleanup();

		/** Tries to activate the buffer
		@return
			true if it is successfully activated
		*/
		bool Bind();

		/** Deactivates the buffer
		*/
		void Unbind();

		//! The index buffer size, 6 since it is a two-triangles quad
		static const int Size = 6;

	private:
		//! The buffer data
		std::array< uint32_t, Size > m_bufferData;
		//! The OpenGL buffer object
		BufferObject m_bufferObject;
	};
}
}

#endif
