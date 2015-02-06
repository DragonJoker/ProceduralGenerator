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
#ifndef ___GENERATOR_GL_VERTEX_BUFFER_H___
#define ___GENERATOR_GL_VERTEX_BUFFER_H___

#include "GlBuffer.h"

#include "Vertex.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Vertex buffer object implementation
	*/
	class GeneratorAPI GlVertexBuffer
		: public GlBuffer
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGL instance
		 */
		GlVertexBuffer( OpenGl * p_pOpenGl );
		/**
		 *\brief		Destructor
		 */
		virtual ~GlVertexBuffer();
		/**
		 *\brief		Initialises the buffer
		 *\return		true if it is successfully initialised
		 */
		virtual bool Initialise();
		/**
		 *\brief		Tries to activate the buffer
		 *\return		true if the VBO and all its attributes are successfully activated
		 */
		virtual bool Activate();
		/**
		 *\brief		Deactivates the buffer
		 */
		virtual void Deactivate();

	private:
		//! The buffer vertices
		std::vector< Vertex > m_arrayVertex;
		//! Buffer data, 4 * [2(vertex position ) + 2(texture coordinates )]
		GLfloat m_pBuffer[16];
	};
}

#endif
