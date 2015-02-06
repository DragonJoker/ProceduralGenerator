#include "Vertex.h"


namespace ProceduralTextures
{
	unsigned long long Vertex::Count = 0;
	size_t Vertex::Size = 4;

	Vertex::Vertex()
		:	m_bOwnBuffer( false )
		,	m_pBuffer( NULL )
	{
		Count++;
		UnlinkCoords();
	}

	Vertex::Vertex( const Vertex & p_vertex )
		:	m_bOwnBuffer( false )
		,	m_pBuffer( NULL )
		,	m_position( p_vertex.m_position )
		,	m_texture( p_vertex.m_texture )
	{
		Count++;
		UnlinkCoords();
	}

	Vertex::~Vertex()
	{
		if ( m_bOwnBuffer && m_pBuffer )
		{
			delete [] m_pBuffer;
			m_pBuffer = NULL;
		}

		Count--;
	}

	Vertex & Vertex::operator =( const Vertex & p_vertex )
	{
		m_position = p_vertex.m_position;
		m_texture = p_vertex.m_texture;

		if ( m_bOwnBuffer && m_pBuffer )
		{
			delete [] m_pBuffer;
			m_pBuffer = NULL;
		}

		m_bOwnBuffer = false;
		UnlinkCoords();
		return * this;
	}

	void Vertex::LinkCoords( float * p_pBuffer )
	{
		size_t l_count = GetStride() / sizeof( float );
		std::vector< float > l_pBufferSave( l_count );

		if ( m_pBuffer )
		{
			memcpy( & l_pBufferSave[0], m_pBuffer, GetStride() );
		}
		else
		{
			memcpy( &l_pBufferSave[0], m_position.const_ptr(), 2 * sizeof( float ) );
			memcpy( &l_pBufferSave[2], m_texture.const_ptr(), 2 * sizeof( float ) );
		}

		m_position.UnlinkCoords();
		m_texture.UnlinkCoords();

		if ( m_bOwnBuffer && m_pBuffer )
		{
			delete [] m_pBuffer;
			m_pBuffer = NULL;
		}

		m_pBuffer = p_pBuffer;
		m_bOwnBuffer = false;
		DoLink();
		memcpy( m_pBuffer, & l_pBufferSave[0], GetStride() );
	}

	void Vertex::UnlinkCoords()
	{
		size_t l_count = GetStride() / sizeof( float );
		std::vector< float > l_pBufferSave( l_count );

		if ( m_pBuffer )
		{
			memcpy( &l_pBufferSave[0], m_pBuffer, GetStride() );
		}
		else
		{
			memcpy( &l_pBufferSave[0], m_position.const_ptr(), 2 * sizeof( float ) );
			memcpy( &l_pBufferSave[2], m_texture.const_ptr(), 2 * sizeof( float ) );
		}

		m_position.UnlinkCoords();
		m_texture.UnlinkCoords();

		if ( !m_bOwnBuffer )
		{
			m_pBuffer = new float[l_count];
		}

		m_bOwnBuffer = true;
		DoLink();
		memcpy( m_pBuffer, & l_pBufferSave[0], GetStride() );
	}

	void Vertex::SetCoords( const Point2f & val )
	{
		m_position = val;
	}

	void Vertex::SetCoords( float x, float y )
	{
		m_position[0] = x;
		m_position[1] = y;
	}

	void Vertex::SetCoords( const float * p_pCoords )
	{
		m_position[0] = p_pCoords[0];
		m_position[1] = p_pCoords[1];
	}

	void Vertex::SetTexCoord( const Point2f & val )
	{
		m_texture = val;
	}

	void Vertex::SetTexCoord( float x, float y )
	{
		m_texture[0] = x;
		m_texture[1] = y;
	}

	void Vertex::SetTexCoord( const float * p_pCoords )
	{
		m_texture[0] = p_pCoords[0];
		m_texture[1] = p_pCoords[1];
	}

	void Vertex::DoLink()
	{
		m_position.LinkCoords( &m_pBuffer[0] );
		m_texture.LinkCoords( &m_pBuffer[2] );
	}
}
