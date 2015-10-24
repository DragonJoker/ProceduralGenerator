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
#ifndef ___GENERATOR_GL_VERTEX_BUFFER_H___
#define ___GENERATOR_GL_VERTEX_BUFFER_H___

#include "GlBufferObject.h"

#include "Vertex.h"
#include "OpenGl.h"

#define GL_BUFFER_OFFSET( n ) ( ( char * )NULL + ( n ) )

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
		Vertex buffer object implementation
	*/
	template< typename PosType >
	class GeneratorAPI TVertexBuffer
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_mode
			The buffer access mode
		*/
		TVertexBuffer( OpenGl & p_openGl, uint32_t p_mode )
			: m_bufferObject( p_openGl, GL_ARRAY_BUFFER, p_mode )
			, m_bufferData( 4 * ( 2 * sizeof( PosType ) + 2 * sizeof( float ) ) )
			, m_changed( true )
		{
			TVertex< PosType > l_vtxTmp;
			size_t l_stride = 2 * sizeof( PosType ) + 2 * sizeof( float );
			m_arrayVertex.push_back( l_vtxTmp );
			m_arrayVertex.push_back( l_vtxTmp );
			m_arrayVertex.push_back( l_vtxTmp );
			m_arrayVertex.push_back( l_vtxTmp );
			m_arrayVertex[0].Link( &m_bufferData[l_stride * 0] );
			m_arrayVertex[1].Link( &m_bufferData[l_stride * 1] );
			m_arrayVertex[2].Link( &m_bufferData[l_stride * 2] );
			m_arrayVertex[3].Link( &m_bufferData[l_stride * 3] );
			m_arrayVertex[0].SetPosition( PosType( -1 ), PosType( -1 ) );
			m_arrayVertex[1].SetPosition( PosType( 1 ), PosType( -1 ) );
			m_arrayVertex[2].SetPosition( PosType( 1 ), PosType( 1 ) );
			m_arrayVertex[3].SetPosition( PosType( -1 ), PosType( 1 ) );
			m_arrayVertex[0].SetTexture( 0.0f, 0.0f );
			m_arrayVertex[1].SetTexture( 1.0f, 0.0f );
			m_arrayVertex[2].SetTexture( 1.0f, 1.0f );
			m_arrayVertex[3].SetTexture( 0.0f, 1.0f );
		}

		/** Destructor
		*/
		~TVertexBuffer()
		{
		}

		/** Creates and initialises the buffer
		@return
			true if it is successfully initialised
		*/
		inline bool Initialise()
		{
			bool l_return = m_bufferObject.Create();

			if ( l_return )
			{
				l_return = m_bufferObject.Bind();

				if ( l_return )
				{
					m_bufferObject.Data( m_bufferData.data(), m_bufferData.size() );
					m_bufferObject.Unbind();
					m_changed = false;
				}
			}

			return l_return;
		}

		/** Destroys the buffer
		*/
		inline void Cleanup()
		{
			m_bufferObject.Destroy();
		}

		/** Tries to activate the buffer
		@param[in] p_vertex
			The "vertex" attribute location
		@param[in] p_texture
			The "texture" attribute location
		@return
			true if the VBO and all its attributes are successfully activated
		*/
		inline bool Bind( uint32_t p_vertex, uint32_t p_texture )
		{
			GLsizei l_iStride = GLsizei( m_arrayVertex[0].GetStride() );
			bool l_bReturn = m_bufferObject.Bind();
			OpenGl & l_gl = m_bufferObject.GetOpenGl();

			if ( m_changed )
			{
				m_bufferObject.Data( m_bufferData.data(), m_bufferData.size() );
			}

			l_bReturn &= l_gl.EnableVertexAttribArray( p_vertex );
			l_bReturn &= l_gl.VertexAttribPointer( p_vertex, 2, m_dataType, false, l_iStride, GL_BUFFER_OFFSET( 0 ) );

			if ( p_texture != GL_INVALID_INDEX )
			{
				l_bReturn &= l_gl.EnableVertexAttribArray( p_texture );
				l_bReturn &= l_gl.VertexAttribPointer( p_texture, 2, GL_FLOAT, false, l_iStride, GL_BUFFER_OFFSET( 2 * sizeof( PosType ) ) );
				l_bReturn &= l_gl.ClientActiveTexture( GL_TEXTURE0 );
				l_bReturn &= l_gl.ClientActiveTexture( GL_TEXTURE1 );
				l_bReturn &= l_gl.ClientActiveTexture( GL_TEXTURE2 );
				l_bReturn &= l_gl.ClientActiveTexture( GL_TEXTURE3 );
			}

			return l_bReturn;
		}

		/** Deactivates the buffer
		*/
		inline void Unbind()
		{
			OpenGl & l_gl = m_bufferObject.GetOpenGl();
			l_gl.DisableVertexAttribArray( 0 );
			l_gl.DisableVertexAttribArray( 1 );
			m_bufferObject.Unbind();
		}

		/** Sets the vertices buffer
		@param[in] p_buffer
			The buffer
		*/
		inline void SetBuffer( std::vector< TVertex< PosType > > const & p_buffer )
		{
			m_arrayVertex.clear();
			m_arrayVertex.resize( p_buffer.size() );
			std::copy( p_buffer.begin(), p_buffer.end(), m_arrayVertex.begin() );
			size_t l_stride = 2 * sizeof( PosType ) + 2 * sizeof( float );
			m_bufferData.resize( l_stride * m_arrayVertex.size() );
			uint8_t * l_buffer = &m_bufferData[0];

			for ( auto & l_vertex : m_arrayVertex )
			{
				l_vertex.Link( l_buffer );
				l_buffer += l_stride;
			}

			m_changed = true;
		}

		/** Retrieves the vertices buffer
		@return
			The buffer
		*/
		inline std::vector< TVertex< PosType > > const & GetVertex()const
		{
			return m_arrayVertex;
		}

	private:
		//! The buffer vertices
		std::vector< TVertex< PosType > > m_arrayVertex;
		//! Buffer data, count * [2(vertex position ) + 2(texture coordinates )]
		std::vector< uint8_t > m_bufferData;
		//! The changed status
		bool m_changed;
		//! The OpenGL buffer object
		BufferObject m_bufferObject;
		//! The OpenGL data type
		static uint32_t m_dataType;
	};
}
}

#endif
