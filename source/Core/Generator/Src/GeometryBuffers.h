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
#ifndef ___GENERATOR_GEOMETRY_BUFFERS_H___
#define ___GENERATOR_GEOMETRY_BUFFERS_H___

#include "GlBufferObject.h"

#include "GlVertexBuffer.h"
#include "GlIndexBuffer.h"
#include "Vertex.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		23/02/2015
	@version
		2.0.0
	@brief
		Holds the vertex and index buffers for a geometry
	*/
	template< typename PosType >
	class GeneratorAPI TGeometryBuffers
		: public gl::Holder
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_mode
			The vertex buffer access mode
		@param[in] p_hasIndexBuffer
			Tells the geometry buffers holds an index buffer
		 */
		TGeometryBuffers( gl::OpenGl & p_openGl, uint32_t p_mode, bool p_hasIndexBuffer = true )
			: gl::Holder( p_openGl )
			, m_vertexBuffer( p_openGl, p_mode )
			, m_indexBuffer( p_hasIndexBuffer ? std::make_unique< gl::IndexBuffer >( p_openGl, p_mode ) : nullptr )
		{
		}

		/** Destructor
		 */
		~TGeometryBuffers()
		{
		}

		/** Initialises the buffers
		@return
			true if it is successfully initialised
		 */
		bool Initialise()
		{
			bool l_return = m_vertexBuffer.Initialise();

			if ( l_return && m_indexBuffer )
			{
				l_return = m_indexBuffer->Initialise();
			}

			return l_return;
		}

		/** Cleans the buffers and destroys it
		 */
		void Cleanup()
		{
			if ( m_indexBuffer )
			{
				m_indexBuffer->Destroy();
			}

			m_vertexBuffer.Destroy();
		}

		/** Tries to activate the buffers and to draw it
		@param[in] p_vertex
			The "vertex" attribute location
		@param[in] p_texture
			The "texture" attribute location
		 */
		void Draw( uint32_t p_vertex, uint32_t p_texture )
		{
			if ( m_vertexBuffer.Activate( p_vertex, p_texture ) )
			{
				if ( m_indexBuffer )
				{
					if ( m_indexBuffer->Activate() )
					{
						GetOpenGl().DrawElements( GL_TRIANGLES, gl::IndexBuffer::Size, GL_UNSIGNED_INT, 0 );
						m_indexBuffer->Deactivate();
					}
				}
				else
				{
					GetOpenGl().DrawArrays( GL_TRIANGLES, 0, int( m_vertexBuffer.GetBuffer().size() ) );
				}

				m_vertexBuffer.Deactivate();
			}
		}

		/** Retrieves the vertex buffer
		@return
			The buffer
		 */
		gl::TVertexBuffer< PosType > const & GetVertexBuffer()const
		{
			return m_vertexBuffer;
		}

		/** Retrieves the vertex buffer
		@return
			The buffer
		 */
		gl::TVertexBuffer< PosType > & GetVertexBuffer()
		{
			return m_vertexBuffer;
		}

		/** Retrieves the index buffer
		@return
			The buffer
		 */
		gl::IndexBuffer const & GetIndexBuffer()const
		{
			return m_indexBuffer;
		}

		/** Retrieves the index buffer
		@return
			The buffer
		 */
		gl::IndexBuffer & GetIndexBuffer()
		{
			return m_indexBuffer;
		}

	private:
		//! The vertex buffer
		gl::TVertexBuffer< PosType > m_vertexBuffer;
		//! The index buffer
		std::unique_ptr< gl::IndexBuffer > m_indexBuffer;
	};
}

#endif
