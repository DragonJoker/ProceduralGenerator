#include "GlVertexBuffer.h"
#include "OpenGl.h"

#define GL_BUFFER_OFFSET( n)	((char*)NULL + ( n))

namespace ProceduralTextures
{
	GlVertexBuffer::GlVertexBuffer( OpenGl * p_pOpenGl )
		:	GlBuffer( p_pOpenGl, GL_ARRAY_BUFFER, GL_STATIC_DRAW )
	{
		Vertex l_vtxTmp;
		m_arrayVertex.push_back( l_vtxTmp );
		m_arrayVertex.push_back( l_vtxTmp );
		m_arrayVertex.push_back( l_vtxTmp );
		m_arrayVertex.push_back( l_vtxTmp );
		m_arrayVertex[0].LinkCoords( &m_pBuffer[0] );
		m_arrayVertex[1].LinkCoords( &m_pBuffer[4] );
		m_arrayVertex[2].LinkCoords( &m_pBuffer[8] );
		m_arrayVertex[3].LinkCoords( &m_pBuffer[12] );
		m_arrayVertex[0].SetCoords( 0.0f, 0.0f );
		m_arrayVertex[1].SetCoords( 1.0f, 0.0f );
		m_arrayVertex[2].SetCoords( 1.0f, 1.0f );
		m_arrayVertex[3].SetCoords( 0.0f, 1.0f );
		m_arrayVertex[0].SetTexCoord( 0.0f, 0.0f );
		m_arrayVertex[1].SetTexCoord( 1.0f, 0.0f );
		m_arrayVertex[2].SetTexCoord( 1.0f, 1.0f );
		m_arrayVertex[3].SetTexCoord( 0.0f, 1.0f );
	}

	GlVertexBuffer::~GlVertexBuffer()
	{
	}

	bool GlVertexBuffer::Initialise()
	{
		bool l_return = Create();

		if ( l_return )
		{
			l_return = Bind();

			if ( l_return )
			{
				Data( m_pBuffer, sizeof( m_pBuffer ) );
				Unbind();
			}
		}

		return l_return;
	}

	bool GlVertexBuffer::Activate()
	{
		GLsizei l_iStride = GLsizei( m_arrayVertex[0].GetStride() );
		bool l_bReturn = Bind();
		l_bReturn &= m_pOpenGl->EnableClientState( GL_VERTEX_ARRAY );
		l_bReturn &= m_pOpenGl->VertexPointer( 2, GL_FLOAT, l_iStride, GL_BUFFER_OFFSET( 0 ) );
		l_bReturn &= m_pOpenGl->ClientActiveTexture( GL_TEXTURE0 );
		l_bReturn &= m_pOpenGl->EnableClientState( GL_TEXTURE_COORD_ARRAY );
		l_bReturn &= m_pOpenGl->TexCoordPointer( 2, GL_FLOAT, l_iStride, GL_BUFFER_OFFSET( 8 ) );
		l_bReturn &= m_pOpenGl->ClientActiveTexture( GL_TEXTURE1 );
		l_bReturn &= m_pOpenGl->EnableClientState( GL_TEXTURE_COORD_ARRAY );
		l_bReturn &= m_pOpenGl->TexCoordPointer( 2, GL_FLOAT, l_iStride, GL_BUFFER_OFFSET( 8 ) );
		l_bReturn &= m_pOpenGl->ClientActiveTexture( GL_TEXTURE2 );
		l_bReturn &= m_pOpenGl->EnableClientState( GL_TEXTURE_COORD_ARRAY );
		l_bReturn &= m_pOpenGl->TexCoordPointer( 2, GL_FLOAT, l_iStride, GL_BUFFER_OFFSET( 8 ) );
		l_bReturn &= m_pOpenGl->ClientActiveTexture( GL_TEXTURE3 );
		l_bReturn &= m_pOpenGl->EnableClientState( GL_TEXTURE_COORD_ARRAY );
		l_bReturn &= m_pOpenGl->TexCoordPointer( 2, GL_FLOAT, l_iStride, GL_BUFFER_OFFSET( 8 ) );
		return l_bReturn;
	}

	void GlVertexBuffer::Deactivate()
	{
		m_pOpenGl->DisableClientState( GL_VERTEX_ARRAY );
		m_pOpenGl->ClientActiveTexture( GL_TEXTURE0 );
		m_pOpenGl->DisableClientState( GL_TEXTURE_COORD_ARRAY );
		m_pOpenGl->ClientActiveTexture( GL_TEXTURE1 );
		m_pOpenGl->DisableClientState( GL_TEXTURE_COORD_ARRAY );
		m_pOpenGl->ClientActiveTexture( GL_TEXTURE2 );
		m_pOpenGl->DisableClientState( GL_TEXTURE_COORD_ARRAY );
		m_pOpenGl->ClientActiveTexture( GL_TEXTURE3 );
		m_pOpenGl->DisableClientState( GL_TEXTURE_COORD_ARRAY );
		Unbind();
	}
}
