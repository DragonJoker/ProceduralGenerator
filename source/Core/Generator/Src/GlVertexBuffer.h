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
		: public BufferObject
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_mode
			The buffer access mode
		 */
		TVertexBuffer( std::shared_ptr< OpenGl > p_openGl, uint32_t p_mode )
			: BufferObject( p_openGl, GL_ARRAY_BUFFER, p_mode )
			, m_buffer( 4 * ( 2 * sizeof( PosType ) + 2 * sizeof( float ) ) )
			, m_changed( true )
		{
			TVertex< PosType > l_vtxTmp;
			size_t l_stride = 2 * sizeof( PosType ) + 2 * sizeof( float );
			m_arrayVertex.push_back( l_vtxTmp );
			m_arrayVertex.push_back( l_vtxTmp );
			m_arrayVertex.push_back( l_vtxTmp );
			m_arrayVertex.push_back( l_vtxTmp );
			m_arrayVertex[0].Link( &m_buffer[l_stride * 0] );
			m_arrayVertex[1].Link( &m_buffer[l_stride * 1] );
			m_arrayVertex[2].Link( &m_buffer[l_stride * 2] );
			m_arrayVertex[3].Link( &m_buffer[l_stride * 3] );
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
		virtual ~TVertexBuffer()
		{
		}

		/** Initialises the buffer
		@return
			true if it is successfully initialised
		 */
		virtual bool Initialise()
		{
			bool l_return = Create();

			if ( l_return )
			{
				l_return = Bind();

				if ( l_return )
				{
					Data( m_buffer.data(), m_buffer.size() );
					Unbind();
					m_changed = false;
				}
			}

			return l_return;
		}

		/** Tries to activate the buffer
		@param[in] p_vertex
			The "vertex" attribute location
		@param[in] p_texture
			The "texture" attribute location
		@return
			true if the VBO and all its attributes are successfully activated
		 */
		virtual bool Activate( uint32_t p_vertex, uint32_t p_texture )
		{
			GLsizei l_iStride = GLsizei( m_arrayVertex[0].GetStride() );
			bool l_bReturn = Bind();

			if ( m_changed )
			{
				Data( m_buffer.data(), m_buffer.size() );
			}

			l_bReturn &= GetOpenGl()->EnableVertexAttribArray( p_vertex );
			l_bReturn &= GetOpenGl()->VertexAttribPointer( p_vertex, 2, m_dataType, false, l_iStride, GL_BUFFER_OFFSET( 0 ) );

			if ( p_texture != GL_INVALID_INDEX )
			{
				l_bReturn &= GetOpenGl()->EnableVertexAttribArray( p_texture );
				l_bReturn &= GetOpenGl()->VertexAttribPointer( p_texture, 2, GL_FLOAT, false, l_iStride, GL_BUFFER_OFFSET( 2 * sizeof( PosType ) ) );
			}

			l_bReturn &= GetOpenGl()->ClientActiveTexture( GL_TEXTURE0 );
			l_bReturn &= GetOpenGl()->ClientActiveTexture( GL_TEXTURE1 );
			l_bReturn &= GetOpenGl()->ClientActiveTexture( GL_TEXTURE2 );
			l_bReturn &= GetOpenGl()->ClientActiveTexture( GL_TEXTURE3 );
			return l_bReturn;
		}

		/** Deactivates the buffer
		 */
		virtual void Deactivate()
		{
			GetOpenGl()->DisableVertexAttribArray( 0 );
			GetOpenGl()->DisableVertexAttribArray( 1 );
			Unbind();
		}

		/** Sets the vertices buffer
		@param[in] p_buffer
			The buffer
		 */
		void SetBuffer( std::vector< TVertex< PosType > > const & p_buffer )
		{
			m_arrayVertex.clear();
			m_arrayVertex.resize( p_buffer.size() );
			std::copy( p_buffer.begin(), p_buffer.end(), m_arrayVertex.begin() );
			size_t l_stride = 2 * sizeof( PosType ) + 2 * sizeof( float );
			m_buffer.resize( l_stride * m_arrayVertex.size() );
			uint8_t * l_buffer = &m_buffer[0];

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
		std::vector< TVertex< PosType > > const & GetBuffer()const
		{
			return m_arrayVertex;
		}

	private:
		/** Dummy
		@return
			false
		 */
		virtual bool Activate()
		{
			return false;
		}

		//! The buffer vertices
		std::vector< TVertex< PosType > > m_arrayVertex;
		//! Buffer data, count * [2(vertex position ) + 2(texture coordinates )]
		std::vector< uint8_t > m_buffer;
		//! The changed status
		bool m_changed;
		//! The OpenGL data type
		static uint32_t m_dataType;
	};
}
}

#endif
