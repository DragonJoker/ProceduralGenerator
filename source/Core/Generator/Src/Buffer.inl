#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4127 )	//l'expression conditionnelle est une constante
#endif

namespace ProceduralTextures
{
	template< typename T >
	Buffer< T >::Buffer( const Size & p_ptSize )
		:	m_pPixels( NULL )
		,	m_ptSize( p_ptSize )
		,	m_bOwnsPixels( true )
	{
		init();
	}

	template< typename T >
	Buffer< T >::Buffer( const Buffer< T > & p_pixelBuffer )
		:	m_pPixels( NULL )
		,	m_ptSize( p_pixelBuffer.m_ptSize )
		,	m_bOwnsPixels( true )
	{
		init();
		memcpy( m_pPixels, p_pixelBuffer.m_pPixels, m_ptSize[0] * m_ptSize[1] * sizeof( T ) );
	}

	template< typename T >
	Buffer< T >::~Buffer()
	{
		clear();
	}

	template< typename T >
	Buffer< T > & Buffer< T >::operator =( const Buffer< T > & p_pixelBuffer )
	{
		memcpy( m_pPixels, p_pixelBuffer.m_pPixels, m_ptSize[0] * m_ptSize[1] * sizeof( T ) );
		return * this;
	}

	template< typename T >
	Buffer< T > & Buffer< T >::operator =( const T * p_pBuffer )
	{
		memcpy( m_pPixels, p_pBuffer, m_ptSize[0] * m_ptSize[1] * sizeof( T ) );
		return * this;
	}

	template< typename T >
	void Buffer< T >::clear()
	{
		if ( m_bOwnsPixels && m_pPixels )
		{
			delete [] m_pPixels;
		}

		m_pPixels = NULL;
	}

	template< typename T >
	void Buffer< T >::init()
	{
		clear();
		m_pPixels = new T[m_ptSize[0] * m_ptSize[1]];
		memset( m_pPixels, 0, m_ptSize[0] * m_ptSize[1] * sizeof( T ) );
		m_arrayRows.clear();
		m_arrayRows.reserve( m_ptSize[1] );

		for ( size_t i = 0 ; i < m_ptSize[1] ; i++ )
		{
			m_arrayRows.push_back( DynPoint< T >( m_ptSize[0], & m_pPixels[i * m_ptSize[0]] ) );
		}
	}

	template< typename T >
	void Buffer< T >::init( const Size & p_ptSize )
	{
		m_ptSize = p_ptSize;
		init();
	}

	template< typename T >
	void Buffer< T >::swap( Buffer< T > & p_pixelBuffer )
	{
		std::swap( m_pPixels, p_pixelBuffer.m_pPixels );
		std::swap( m_arrayRows, p_pixelBuffer.m_arrayRows );
	}

	template< typename T >
	void Buffer< T >::link( T * p_pBuffer )
	{
		clear();
		m_pPixels = p_pBuffer;

		for ( size_t i = 0; i < m_ptSize[1]; i++ )
		{
			m_arrayRows[i].LinkCoords( &m_pPixels[i * m_ptSize[0]] );
		}
	}

	template< typename T >
	void Buffer< T >::unlink()
	{
		T * l_pTmp = m_pPixels;
		init();
		memcpy( m_pPixels, l_pTmp, size() );
	}

	template< typename T >
	void Buffer< T >::mirror()
	{
	}
	template< typename T >
	void Buffer< T >::flip()
	{
		for ( size_t i = 0; i < m_ptSize[1] / 2; i++ )
		{
			m_arrayRows[i].swap( m_arrayRows[m_ptSize[1] - 1 - i] );
		}
	}

	template< typename T >
	template< typename U, size_t Count1, size_t Count2, bool InvertedX, bool InvertedY >
	void Buffer< T >::set( const U * p_pBuffer1, const U * p_pBuffer2 )
	{
		BufferCopier< T, U, Count1, Count2, InvertedX, InvertedY >()( this, p_pBuffer1, p_pBuffer2 );
	}

//*************************************************************************************************
}

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif
