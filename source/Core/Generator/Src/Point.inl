#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4127 )	//l'expression conditionnelle est une constante
#endif

namespace ProceduralTextures
{
//*************************************************************************************************

	template< typename T >
	DynPoint< T >::DynPoint( size_t p_uiCount )
		: m_coords( NULL )
		, m_bOwnCoords( true )
		, m_uiCount( p_uiCount )
	{
		if ( m_uiCount )
		{
			m_coords = new T[m_uiCount];
		}
	}

	template< typename T >
	template< typename U >
	DynPoint< T >::DynPoint( std::initializer_list< U > p_values )
		: m_coords( NULL )
		, m_bOwnCoords( true )
		, m_uiCount( p_values.size() )
	{
		if ( m_uiCount )
		{
			m_coords = new T[m_uiCount];
			auto l_it = p_values.begin();
			size_t i = 0;

			while ( l_it != p_values.end() && i < m_uiCount )
			{
				m_coords[i++] = T( *l_it );
				++l_it;
			}
		}
	}

	template< typename T >
	template< typename U >
	DynPoint< T >::DynPoint( size_t p_uiCount, const U * p_pData )
		: m_coords( NULL )
		, m_bOwnCoords( false )
		, m_uiCount( p_uiCount )
	{
		m_coords = ( T * )p_pData;
	}

	template< typename T >
	template< typename U >
	DynPoint< T >::DynPoint( U const & p_vA, U const & p_vB )
		: m_coords( NULL )
		, m_bOwnCoords( true )
		, m_uiCount( 2 )
	{
		m_coords = new T[m_uiCount];
		m_coords[0] = p_vA;
		m_coords[1] = p_vB;
	}

	template< typename T >
	template< typename U >
	DynPoint< T >::DynPoint( U const & p_vA, U const & p_vB, U const & p_vC )
		: m_coords( NULL )
		, m_bOwnCoords( true )
		, m_uiCount( 3 )
	{
		m_coords = new T[m_uiCount];
		m_coords[0] = p_vA;
		m_coords[1] = p_vB;
		m_coords[2] = p_vC;
	}

	template< typename T >
	template< typename U >
	DynPoint< T >::DynPoint( U const & p_vA, U const & p_vB, U const & p_vC, U const & p_vD )
		: m_coords( NULL )
		, m_bOwnCoords( true )
		, m_uiCount( 4 )
	{
		m_coords = new T[m_uiCount];
		m_coords[0] = p_vA;
		m_coords[1] = p_vB;
		m_coords[2] = p_vC;
		m_coords[3] = p_vD;
	}

	template< typename T >
	template< typename U >
	DynPoint< T >::DynPoint( const DynPoint< U > & p_ptPoint )
		: m_coords( NULL )
		, m_bOwnCoords( true )
		, m_uiCount( p_ptPoint.m_uiCount )
	{
		if ( m_uiCount > 0 )
		{
			m_coords = new T[m_uiCount];
		}

		for ( size_t i = 0; i < m_uiCount; i++ )
		{
			m_coords[i] = T( p_ptPoint[i] );
		}
	}

	template< typename T >
	inline DynPoint < T >::~DynPoint()
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
		}
	}
	template< typename T >
	inline void DynPoint < T >::Link( const void * p_pCoords )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
			m_coords = NULL;
		}

		m_coords = ( T * )p_pCoords;
		m_bOwnCoords = false;
	}
	template< typename T >
	inline void DynPoint < T >::Unlink()
	{
		if ( !m_bOwnCoords )
		{
			m_coords = new T[m_uiCount];
		}

		m_bOwnCoords = true;
	}
	template< typename T >
	inline void DynPoint < T >::ToValues( T * p_pResult )const
	{
		if ( m_coords != NULL )
		{
			p_pResult[0] = T( m_coords[0] );

			for ( size_t i = 0; i < m_uiCount; i++ )
			{
				p_pResult[i] = m_coords[i];
			}
		}
	}

	template< typename T >
	DynPoint< T >::DynPoint( const DynPoint< T > & p_pt )
		:	m_coords( NULL )
		,	m_bOwnCoords( p_pt.m_bOwnCoords )
		,	m_uiCount( p_pt.m_uiCount )
	{
		if ( m_bOwnCoords )
		{
			if ( m_uiCount > 0 )
			{
				m_coords = new T[m_uiCount];
			}

			for ( size_t i = 0; i < m_uiCount; i++ )
			{
				m_coords[i] = p_pt[i];
			}
		}
		else
		{
			m_coords = p_pt.m_coords;
		}
	}
	template< typename T >
	DynPoint< T > & DynPoint < T >::operator=( const DynPoint< T > & p_pt )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
			m_coords = NULL;
		}

		m_bOwnCoords = p_pt.m_bOwnCoords;
		m_uiCount = p_pt.m_uiCount;

		if ( m_bOwnCoords )
		{
			if ( m_uiCount > 0 )
			{
				m_coords = new T[m_uiCount];
			}

			for ( size_t i = 0; i < m_uiCount; i++ )
			{
				m_coords[i] = p_pt[i];
			}
		}
		else
		{
			m_coords = p_pt.m_coords;
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	DynPoint< T > & DynPoint < T >::operator=( const DynPoint< U > & p_pt )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
			m_coords = NULL;
		}

		m_bOwnCoords = p_pt.m_bOwnCoords;
		m_uiCount = p_pt.m_uiCount;

		if ( m_bOwnCoords )
		{
			m_coords = new T[m_uiCount];

			for ( size_t i = 0; i < m_uiCount; i++ )
			{
				m_coords[i] = T( p_pt.m_coords[i] );
			}
		}
		else
		{
			m_coords = p_pt.m_coords;
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator+=( const DynPoint< U > & p_pt )
	{
		for ( size_t i = 0; i < m_uiCount && i < p_pt.m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] + p_pt[i] );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator+=( const U * p_coords )
	{
		for ( size_t i = 0; i < m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] + p_coords[i] );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator+=( U const & p_coord )
	{
		for ( size_t i = 0; i < m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] + p_coord );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator-=( const DynPoint< U > & p_pt )
	{
		for ( size_t i = 0; i < m_uiCount && i < p_pt.m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] - p_pt[i] );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator-=( const U * p_coords )
	{
		for ( size_t i = 0; i < m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] - p_coords[i] );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator-=( U const & p_coord )
	{
		for ( size_t i = 0; i < m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] - p_coord );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator*=( const DynPoint< U > & p_pt )
	{
		for ( size_t i = 0; i < m_uiCount && i < p_pt.m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] * p_pt[i] );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator*=( const U * p_coords )
	{
		for ( size_t i = 0; i < m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] * p_coords[i] );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator*=( U const & p_coord )
	{
		for ( size_t i = 0; i < m_uiCount; i++ )
		{
			m_coords[i] = T( m_coords[i] * p_coord );
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator/=( const DynPoint< U > & p_pt )
	{
		for ( size_t i = 0; i < m_uiCount && i < p_pt.m_uiCount; i++ )
		{
			if ( p_pt[i] )
			{
				m_coords[i] = T( m_coords[i] / p_pt[i] );
			}
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator/=( const U * p_coords )
	{
		for ( size_t i = 0; i < m_uiCount; i++ )
		{
			if ( p_coords[i] )
			{
				m_coords[i] = T( m_coords[i] / p_coords[i] );
			}
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator/=( U const & p_coord )
	{
		if ( p_coord )
		{
			for ( size_t i = 0; i < m_uiCount; i++ )
			{
				m_coords[i] = T( m_coords[i] / p_coord );
			}
		}

		return * this;
	}
	template< typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator^=( const DynPoint< U > & p_pt )
	{
		if ( m_uiCount == 3 && p_pt.m_uiCount == 3 )
		{
			T l_valuesA[3], l_valuesB[3];
			p_pt.ToValues( l_valuesA );
			ToValues( l_valuesB );
			m_coords[0] = T( l_valuesB[1] * l_valuesA[2] ) - T( l_valuesA[1] * l_valuesB[2] );
			m_coords[1] = T( l_valuesB[2] * l_valuesA[0] ) - T( l_valuesA[2] * l_valuesB[0] );
			m_coords[2] = T( l_valuesB[0] * l_valuesA[1] ) - T( l_valuesA[0] * l_valuesB[1] );
		}

		return * this;
	}
	template< typename T >
	void DynPoint < T >::swap( DynPoint< T > & p_pt )
	{
		size_t l_uiTmp = p_pt.m_uiCount;
		bool l_bTmp = p_pt.m_bOwnCoords;
		std::vector< T > l_pTmp;
		l_pTmp.insert( l_pTmp.end(), p_pt.m_coords, &p_pt.m_coords[m_uiCount] );
		p_pt.m_uiCount = m_uiCount;
		p_pt.m_bOwnCoords = m_bOwnCoords;
		std::memcpy( p_pt.m_coords, m_coords, sizeof( T ) * m_uiCount );
		m_uiCount = l_uiTmp;
		m_bOwnCoords = l_bTmp;
		std::memcpy( m_coords, l_pTmp.data(), sizeof( T )* m_uiCount );
	}

//*************************************************************************************************

	template< typename T, typename U >
	inline bool operator==( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		bool l_bReturn = ( p_ptA.GetElementCount() == p_ptB.GetElementCount() );

		for ( size_t i = 0; i < p_ptA.GetElementCount() && l_bReturn; i++ )
		{
			l_bReturn = p_ptA[i] == p_ptB[i];
		}

		return l_bReturn;
	}
	template< typename T, typename U >
	inline bool operator!=( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		return !( p_ptA == p_ptB );
	}
	template< typename T >
	inline std::ostream & operator<<( std::ostream & l_streamOut, const DynPoint< T > & p_pt )
	{
		for ( size_t i = 0; i < p_pt.GetElementCount(); i++ )
		{
			l_streamOut << "\t" << p_pt[i];
		}

		l_streamOut << std::endl;
		return l_streamOut;
	}
	template< typename T >
	inline std::istream & operator>>( std::istream & l_streamIn, DynPoint< T > & p_pt )
	{
		for ( size_t i = 0; i < p_pt.GetElementCount(); i++ )
		{
			l_streamIn >> p_pt[i];
		}

		return l_streamIn;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator+( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult += p_ptB;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator+( const DynPoint< T > & p_pt, const U * p_coords )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult += p_coords;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator+( const DynPoint< T > & p_pt, U const & p_coord )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult += p_coord;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator-( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult -= p_ptB;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator-( const DynPoint< T > & p_pt, const U * p_coords )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult -= p_coords;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator-( const DynPoint< T > & p_pt, U const & p_coord )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult -= p_coord;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator*( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult *= p_ptB;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator*( const DynPoint< T > & p_pt, const U * p_coords )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult *= p_coords;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator*( const DynPoint< T > & p_pt, U const & p_coord )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult *= p_coord;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator/( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult /= p_ptB;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator/( const DynPoint< T > & p_pt, const U * p_coords )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult /= p_coords;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator/( const DynPoint< T > & p_pt, U const & p_coord )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult /= p_coord;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator^( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult ^= p_ptB;
		return l_ptResult;
	}

	template< typename T >
	inline DynPoint < T > operator*( int p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint * p_value;
	}
	template< typename T >
	inline DynPoint < T > operator+( int p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint + p_value;
	}
	template< typename T >
	inline DynPoint < T > operator-( int p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint - p_value;
	}
	template< typename T >
	inline DynPoint < T > operator/( int p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint / p_value;
	}
	template< typename T >
	inline DynPoint < T > operator*( double p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint * p_value;
	}
	template< typename T >
	inline DynPoint < T > operator+( double p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint + p_value;
	}
	template< typename T >
	inline DynPoint < T > operator-( double p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint - p_value;
	}
	template< typename T >
	inline DynPoint < T > operator/( double p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint / p_value;
	}

//*************************************************************************************************

	template < typename T, size_t Count >
	Point< T, Count >::Point()
		: m_bOwnCoords( true )
	{
		m_coords.dyn = NULL;

		if ( Count > 0 )
		{
			m_coords.dyn = new T[Count];
		}

		for ( size_t i = 0; i < Count; i++ )
		{
			m_coords.dyn[i] = T();
		}
	}

	template < typename T, size_t Count >
	template< typename U >
	Point< T, Count >::Point( std::initializer_list< U > p_values )
		: m_bOwnCoords( true )
	{
		m_coords.dyn = NULL;

		if ( Count > 0 )
		{
			m_coords.dyn = new T[Count];
		}

		if ( p_values.size() )
		{
			auto l_it = p_values.begin();
			size_t i = 0;

			while ( l_it != p_values.end() && i < Count )
			{
				m_coords.dyn[i++] = T( *l_it );
				++l_it;
			}
		}
		else
		{
			for ( size_t i = 0; i < Count; i++ )
			{
				m_coords.dyn[i] = T();
			}
		}
	}

	template < typename T, size_t Count >
	template< typename U >
	Point< T, Count >::Point( const U * p_pValues )
		: m_bOwnCoords( true )
	{
		m_coords.dyn = NULL;

		if ( Count > 0 )
		{
			m_coords.dyn = new T[Count];
		}

		if ( !p_pValues )
		{
			for ( size_t i = 0; i < Count; i++ )
			{
				m_coords.dyn[i] = T();
			}
		}
		else
		{
			for ( size_t i = 0; i < Count; i++ )
			{
				m_coords.dyn[i] = p_pValues[i];
			}
		}
	}

	template < typename T, size_t Count >
	template< typename U >
	Point< T, Count >::Point( U const & p_vA )
		: m_bOwnCoords( true )
	{
		m_coords.dyn = NULL;

		if ( Count > 0 )
		{
			m_coords.dyn = new T[Count];
			m_coords.dyn[0] = T( p_vA );
		}

		if ( Count > 1 )
		{
			for ( size_t i = 1; i < Count; i++ )
			{
				m_coords.dyn[i] = T();
			}
		}
	}
	template < typename T, size_t Count >
	template< typename U >
	Point< T, Count >::Point( U const & p_vA, U const & p_vB )
		: m_bOwnCoords( true )
	{
		m_coords.dyn = NULL;

		if ( Count > 0 )
		{
			m_coords.dyn = new T[Count];
			m_coords.dyn[0] = T( p_vA );
		}

		if ( Count > 1 )
		{
			m_coords.dyn[1] = T( p_vB );

			for ( size_t i = 2; i < Count; i++ )
			{
				m_coords.dyn[i] = T();
			}
		}
	}
	template < typename T, size_t Count >
	template< typename U >
	Point< T, Count >::Point( U const & p_vA, U const & p_vB, U const & p_vC )
		: m_bOwnCoords( true )
	{
		m_coords.dyn = NULL;

		if ( Count > 0 )
		{
			m_coords.dyn = new T[Count];
			m_coords.dyn[0] = T( p_vA );
		}

		if ( Count > 1 )
		{
			m_coords.dyn[1] = T( p_vB );
		}

		if ( Count > 2 )
		{
			m_coords.dyn[2] = T( p_vC );

			for ( size_t i = 3; i < Count; i++ )
			{
				m_coords.dyn[i] = T();
			}
		}
	}
	template < typename T, size_t Count >
	template< typename U >
	Point< T, Count >::Point( U const & p_vA, U const & p_vB, U const & p_vC, U const & p_vD )
		: m_bOwnCoords( true )
	{
		m_coords.dyn = NULL;

		if ( Count > 0 )
		{
			m_coords.dyn = new T[Count];
			m_coords.dyn[0] = T( p_vA );
		}

		if ( Count > 1 )
		{
			m_coords.dyn[1] = T( p_vB );
		}

		if ( Count > 2 )
		{
			m_coords.dyn[2] = T( p_vC );
		}

		if ( Count > 3 )
		{
			m_coords.dyn[3] = T( p_vD );

			for ( size_t i = 4; i < Count; i++ )
			{
				m_coords.dyn[i] = T();
			}
		}
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	Point< T, Count >::Point( const Point< U, _Count > & p_pt )
		:	m_bOwnCoords( p_pt.m_bOwnCoords )
	{
		m_coords.dyn = NULL;

		if ( m_bOwnCoords )
		{
			if ( Count > 0 )
			{
				m_coords.dyn = new T[Count];
			}

			size_t i;

			for ( i = 0; i < Count && i < _Count; i++ )
			{
				m_coords.dyn[i] = T( p_pt[i] );
			}

			for ( ; i < Count; i++ )
			{
				m_coords.dyn[i] = T();
			}
		}
		else
		{
			m_coords.dyn = ( T * )( p_pt.m_coords.dyn );
		}
	}
	template < typename T, size_t Count >
	inline Point< T, Count >::~Point()
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords.dyn;
		}
	}
	template < typename T, size_t Count >
	Point< T, Count >::Point( const Point< T, Count > & p_pt )
		:	m_bOwnCoords( p_pt.m_bOwnCoords )
	{
		m_coords.dyn = NULL;

		if ( m_bOwnCoords )
		{
			if ( Count > 0 )
			{
				m_coords.dyn = new T[Count];
			}

			for ( size_t i = 0; i < Count; i++ )
			{
				m_coords.dyn[i] = p_pt[i];
			}
		}
		else
		{
			m_coords.dyn = p_pt.m_coords.dyn;
		}
	}
	template < typename T, size_t Count >
	inline Point< T, Count > & Point< T, Count >::operator=( const Point< T, Count > & p_pt )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords.dyn;
			m_coords.dyn = NULL;
		}

		m_bOwnCoords = p_pt.m_bOwnCoords;

		if ( m_bOwnCoords )
		{
			if ( Count > 0 )
			{
				m_coords.dyn = new T[Count];
			}

			for ( size_t i = 0; i < Count; i++ )
			{
				m_coords.dyn[i] = p_pt[i];
			}
		}
		else
		{
			m_coords.dyn = p_pt.m_coords.dyn;
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point< T, Count > & Point< T, Count >::operator=( const Point< U, _Count > & p_pt )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords.dyn;
			m_coords.dyn = NULL;
		}

		m_bOwnCoords = p_pt.m_bOwnCoords;

		if ( m_bOwnCoords )
		{
			if ( Count > 0 )
			{
				m_coords.dyn = new T[Count];
			}

			size_t i;

			for ( i = 0; i < Count && i < _Count; i++ )
			{
				m_coords.dyn[i] = T( p_pt[i] );
			}

			for ( ; i < Count; i++ )
			{
				m_coords.dyn[i] = T();
			}
		}
		else
		{
			m_coords.dyn = ( T * )( p_pt.m_coords.dyn );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point< T, Count > & Point< T, Count >::operator+=( const Point< U, _Count > & p_pt )
	{
		for ( size_t i = 0; i < Count && i < _Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] + p_pt[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point< T, Count > & Point< T, Count >::operator+=( const U * p_coords )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] + p_coords[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point< T, Count > & Point< T, Count >::operator+=( U const & p_coord )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] + p_coord );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point< T, Count > & Point< T, Count >::operator-=( const Point< U, _Count > & p_pt )
	{
		for ( size_t i = 0; i < Count && i < _Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] - p_pt[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point< T, Count > & Point< T, Count >::operator-=( const U * p_coords )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] - p_coords[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point< T, Count > & Point< T, Count >::operator-=( U const & p_coord )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] - p_coord );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point< T, Count > & Point< T, Count >::operator*=( const Point< U, _Count > & p_pt )
	{
		for ( size_t i = 0; i < Count && i < _Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] * p_pt[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point< T, Count > & Point< T, Count >::operator*=( const U * p_coords )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] * p_coords[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point< T, Count > & Point< T, Count >::operator*=( U const & p_coord )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			m_coords.dyn[i] = T( m_coords.dyn[i] * p_coord );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point< T, Count > & Point< T, Count >::operator/=( const Point< U, _Count > & p_pt )
	{
		for ( size_t i = 0; i < Count && i < _Count; i++ )
		{
			if ( p_pt[i] )
			{
				m_coords.dyn[i] = T( m_coords.dyn[i] / p_pt[i] );
			}
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point< T, Count > & Point< T, Count >::operator/=( const U * p_coords )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			if ( p_coords[i] )
			{
				m_coords.dyn[i] = T( m_coords.dyn[i] / p_coords[i] );
			}
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point< T, Count > & Point< T, Count >::operator/=( U const & p_coord )
	{
		if ( p_coord )
		{
			for ( size_t i = 0; i < Count; i++ )
			{
				m_coords.dyn[i] = T( m_coords.dyn[i] / p_coord );
			}
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point< T, Count > & Point< T, Count >::operator^=( const Point< U, _Count > & p_pt )
	{
		if ( Count == 3 )
		{
			T l_valuesA[Count], l_valuesB[Count];
			p_pt.ToValues( l_valuesA );
			ToValues( l_valuesB );
			m_coords.dyn[0] = T( l_valuesB[1] * l_valuesA[2] ) - T( l_valuesA[1] * l_valuesB[2] );
			m_coords.dyn[1] = T( l_valuesB[2] * l_valuesA[0] ) - T( l_valuesA[2] * l_valuesB[0] );
			m_coords.dyn[2] = T( l_valuesB[0] * l_valuesA[1] ) - T( l_valuesA[0] * l_valuesB[1] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	inline void Point< T, Count >::Link( const void * p_pCoords )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords.dyn;
			m_coords.dyn = NULL;
		}

		m_coords.dyn = ( T * )p_pCoords;
		m_bOwnCoords = false;
	}
	template < typename T, size_t Count >
	inline void Point< T, Count >::Unlink()
	{
		if ( !m_bOwnCoords )
		{
			m_coords.dyn = new T[Count];
		}

		m_bOwnCoords = true;
	}
	template < typename T, size_t Count >
	void Point< T, Count >::swap( Point< T, Count > & p_pt )
	{
		std::swap( m_coords.dyn, p_pt.m_coords.dyn );
		std::swap( m_bOwnCoords, p_pt.m_bOwnCoords );
	}
	template < typename T, size_t Count >
	inline void Point< T, Count >::ToValues( T * p_pResult )const
	{
		if ( m_coords.dyn )
		{
			for ( size_t i = 0; i < Count; i++ )
			{
				p_pResult[i] = m_coords.dyn[i];
			}
		}
	}

//*************************************************************************************************

	template < typename T, size_t Count, typename U, size_t _Count >
	inline bool operator==( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		bool l_bReturn = ( Count == _Count );

		for ( size_t i = 0; i < Count && l_bReturn; i++ )
		{
			l_bReturn = p_ptA[i] == p_ptB[i];
		}

		return l_bReturn;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline bool operator!=( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		return !( p_ptA == p_ptB );
	}
	template < typename T, size_t Count >
	inline std::ostream & operator<<( std::ostream & l_streamOut, const Point< T, Count > & p_pt )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			l_streamOut << "\t" << p_pt[i];
		}

		l_streamOut << std::endl;
		return l_streamOut;
	}
	template < typename T, size_t Count >
	inline std::istream & operator>>( std::istream & l_streamIn, Point< T, Count > & p_pt )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			l_streamIn >> p_pt[i];
		}

		return l_streamIn;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point< T, Count > operator+( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point< T, Count > l_ptResult( p_ptA );
		l_ptResult += p_ptB;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point< T, Count > operator+( const Point< T, Count > & p_pt, const U * p_coords )
	{
		Point< T, Count > l_ptResult( p_pt );
		l_ptResult += p_coords;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point< T, Count > operator+( const Point< T, Count > & p_pt, U const & p_coord )
	{
		Point< T, Count > l_ptResult( p_pt );
		l_ptResult += p_coord;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point< T, Count > operator-( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point< T, Count > l_ptResult( p_ptA );
		l_ptResult -= p_ptB;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point< T, Count > operator-( const Point< T, Count > & p_pt, const U * p_coords )
	{
		Point< T, Count > l_ptResult( p_pt );
		l_ptResult -= p_coords;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point< T, Count > operator-( const Point< T, Count > & p_pt, U const & p_coord )
	{
		Point< T, Count > l_ptResult( p_pt );
		l_ptResult -= p_coord;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point< T, Count > operator*( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point< T, Count > l_ptResult( p_ptA );
		l_ptResult *= p_ptB;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point< T, Count > operator*( const Point< T, Count > & p_pt, const U * p_coords )
	{
		Point< T, Count > l_ptResult( p_pt );
		l_ptResult *= p_coords;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point< T, Count > operator*( const Point< T, Count > & p_pt, U const & p_coord )
	{
		Point< T, Count > l_ptResult( p_pt );
		l_ptResult *= p_coord;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point< T, Count > operator/( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point< T, Count > l_ptResult( p_ptA );
		l_ptResult /= p_ptB;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point< T, Count > operator/( const Point< T, Count > & p_pt, const U * p_coords )
	{
		Point< T, Count > l_ptResult( p_pt );
		l_ptResult /= p_coords;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point< T, Count > operator/( const Point< T, Count > & p_pt, U const & p_coord )
	{
		Point< T, Count > l_ptResult( p_pt );
		l_ptResult /= p_coord;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point< T, Count > operator^( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point< T, Count > l_ptResult( p_ptA );
		l_ptResult ^= p_ptB;
		return l_ptResult;
	}

	template < typename T, size_t Count >
	inline Point< T, Count > operator*( int p_value, const Point< T, Count > & p_ptPoint )
	{
		return p_ptPoint * p_value;
	}
	template < typename T, size_t Count >
	inline Point< T, Count > operator+( int p_value, const Point< T, Count > & p_ptPoint )
	{
		return p_ptPoint + p_value;
	}
	template < typename T, size_t Count >
	inline Point< T, Count > operator-( int p_value, const Point< T, Count > & p_ptPoint )
	{
		return p_ptPoint - p_value;
	}
	template < typename T, size_t Count >
	inline Point< T, Count > operator/( int p_value, const Point< T, Count > & p_ptPoint )
	{
		return p_ptPoint / p_value;
	}
	template < typename T, size_t Count >
	inline Point< T, Count > operator*( double p_value, const Point< T, Count > & p_ptPoint )
	{
		return p_ptPoint * p_value;
	}
	template < typename T, size_t Count >
	inline Point< T, Count > operator+( double p_value, const Point< T, Count > & p_ptPoint )
	{
		return p_ptPoint + p_value;
	}
	template < typename T, size_t Count >
	inline Point< T, Count > operator-( double p_value, const Point< T, Count > & p_ptPoint )
	{
		return p_ptPoint - p_value;
	}
	template < typename T, size_t Count >
	inline Point< T, Count > operator/( double p_value, const Point< T, Count > & p_ptPoint )
	{
		return p_ptPoint / p_value;
	}
	template < typename T, size_t Count >
	inline void Reverse( Point< T, Count > const & p_pt )
	{
		for ( size_t i = 0; i < Count; i++ )
		{
			p_pt[i] = -p_pt[i];
		}
	}
	template < typename T, size_t Count >
	inline void Normalise( Point< T, Count > & p_pt )
	{
		T l_length = GetDistance( p_pt, Point< T, Count >() );

		if ( !l_length )
		{
			return;
		}

		p_pt /= ( l_length );
	}
	template < typename T, size_t Count >
	inline Point< T, Count > GetNormalised( Point< T, Count > const & p_pt )
	{
		Point< T, Count > l_ptReturn( p_pt );
		Normalise( l_ptReturn );
		return l_ptReturn;
	}
	template < typename T, size_t Count >
	inline T Dot( Point< T, Count > const & p_a, Point< T, Count > const & p_b )
	{
		T l_tReturn = T();

		for ( size_t i = 0; i < Count; i++ )
		{
			l_tReturn += p_a[i] * p_b[i];
		}

		return l_tReturn;
	}
	template < typename T, size_t Count >
	inline double GetSquaredDistance( Point< T, Count > const & p_a, Point< T, Count > const & p_b )
	{
		Point< T, Count > l_diff( p_a - p_b );
		double l_dReturn = 0.0;

		for ( size_t i = 0; i < Count; i++ )
		{
			l_dReturn += l_diff[i] * l_diff[i];
		}

		return l_dReturn;
	}
	template < typename T, size_t Count >
	inline double GetDistance( Point< T, Count > const & p_a, Point< T, Count > const & p_b )
	{
		return sqrt( GetSquaredDistance( p_a, p_b ) );
	}
	template < typename T, size_t Count >
	inline double GetManhattanDistance( Point< T, Count > const & p_a, Point< T, Count > const & p_b )
	{
		Point< T, Count > l_diff( p_a - p_b );
		double l_dReturn = 0.0;

		for ( size_t i = 0; i < Count; i++ )
		{
			l_dReturn += std::abs( l_diff[i] );
		}

		return l_dReturn;
	}
	template < typename T, size_t Count >
	inline double GetMinkowskiDistance( double p_dOrder, Point< T, Count > const & p_a, Point< T, Count > const & p_b )
	{
		Point< T, Count > l_diff( p_a - p_b );
		double l_dReturn = 0.0;

		for ( size_t i = 0; i < Count; i++ )
		{
			l_dReturn += std::pow( std::abs( l_diff[i] ), p_dOrder );
		}

		l_dReturn = std::pow( l_dReturn, 1.0 / p_dOrder );
		return l_dReturn;
	}
	template < typename T, size_t Count >
	inline double GetChebychevDistance( Point< T, Count > const & p_a, Point< T, Count > const & p_b )
	{
		Point< T, Count > l_diff( p_a - p_b );
		double l_dReturn = 0.0;

		for ( size_t i = 0; i < Count; i++ )
		{
			l_dReturn = std::max( l_dReturn, std::abs( l_diff[i] ) );
		}

		return l_dReturn;
	}
	template < typename T, size_t Count >
	inline double GetCosTheta( Point< T, Count > const & p_a, Point< T, Count > const & p_b )
	{
		return Dot( p_b ) / ( GetDistance( p_a ) * GetDistance( p_b ) );
	}

//*************************************************************************************************
}

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif
