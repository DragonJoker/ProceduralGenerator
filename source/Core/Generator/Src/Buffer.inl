#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4127 )	//l'expression conditionnelle est une constante
#endif

#include <cassert>

namespace ProceduralTextures
{
	template< typename T >
	Buffer< T >::Buffer()
		: m_size()
	{
	}

	template< typename T >
	Buffer< T >::Buffer( Size const & p_ptSize )
		: m_size( p_ptSize )
	{
		Initialise();
	}

	template< typename T >
	Buffer< T >::Buffer( const Buffer< T > & p_pixelBuffer )
		: m_size( p_pixelBuffer.m_size )
	{
		Initialise();
		memcpy( m_pixels.data(), p_pixelBuffer.m_pixels.data(), GetSize() );
	}

	template< typename T >
	Buffer< T > & Buffer< T >::operator =( const Buffer< T > & p_pixelBuffer )
	{
		m_size = p_pixelBuffer.m_size;
		Initialise();
		memcpy( m_pixels.data(), p_pixelBuffer.m_pixels.data(), GetSize() );
		return *this;
	}

	template< typename T >
	Buffer< T >::~Buffer()
	{
		Clear();
	}

	template< typename T >
	void Buffer< T >::Clear()
	{
		m_pixels.clear();
	}

	template< typename T >
	void Buffer< T >::Initialise()
	{
		m_pixels = std::vector< T >( GetElementsCount() );
		m_arrayRows.clear();
		m_arrayRows.reserve( m_size.y() );

		for ( size_t i = 0; i < m_size.y(); i++ )
		{
			m_arrayRows.push_back( DynPoint< T >( m_size.x(), &m_pixels[i * m_size.x()] ) );
		}
	}

	template< typename T >
	void Buffer< T >::Initialise( Size const & p_size )
	{
		m_size = p_size;
		Initialise();
	}

	template< typename T >
	void Buffer< T >::Set( Buffer const & p_buffer )
	{
		memcpy( m_pixels.data(), p_buffer.m_pixels.data(), std::min( GetSize(), p_buffer.GetSize() ) );
	}

	template< typename T >
	void Buffer< T >::Set( T const * p_buffer )
	{
		memcpy( m_pixels.data(), p_buffer, GetSize() );
	}

	template< typename T >
	void Buffer< T >::swap( Buffer< T > & p_pixelBuffer )
	{
		std::swap( m_pixels, p_pixelBuffer.m_pixels );
		std::swap( m_arrayRows, p_pixelBuffer.m_arrayRows );
		std::swap( m_size, p_pixelBuffer.m_size );
	}

	template< typename T >
	void Buffer< T >::Flip()
	{
		for ( size_t i = 0; i < m_size.y() / 2; i++ )
		{
			m_arrayRows[i].swap( m_arrayRows[m_size.y() - 1 - i] );
		}
	}

	template< typename T >
	template< typename U, size_t Count1, size_t Count2, bool InvertedX, bool InvertedY >
	void Buffer< T >::Set( const U * p_pBuffer1, const U * p_pBuffer2 )
	{
		BufferCopier< T, U, Count1, Count2, InvertedX, InvertedY >()( this, p_pBuffer1, p_pBuffer2 );
	}

//*************************************************************************************************
}

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif
