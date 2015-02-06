#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4127 )	//l'expression conditionnelle est une constante
#endif

namespace ProceduralTextures
{
//*************************************************************************************************

	template < typename T >
	DynPoint< T >::DynPoint( size_t p_uiCount )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
		,	m_uiCount( p_uiCount )
	{
		if ( m_uiCount > 0 )
		{
			m_coords = new T[m_uiCount];
		}
	}
	template < typename T >
	template< typename U >
	DynPoint< T >::DynPoint( size_t p_uiCount, const U * p_pData )
		:	m_coords( NULL )
		,	m_bOwnCoords( false )
		,	m_uiCount( p_uiCount )
	{
		m_coords = ( T * )p_pData;
	}

	template < typename T >
	template< typename U >
	DynPoint< T >::DynPoint( const U & p_vA, const U & p_vB )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
		,	m_uiCount( 2 )
	{
		m_coords = new T[m_uiCount];
		m_coords[0] = p_vA;
		m_coords[1] = p_vB;
	}

	template < typename T >
	template< typename U >
	DynPoint< T >::DynPoint( const U & p_vA, const U & p_vB, const U & p_vC )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
		,	m_uiCount( 3 )
	{
		m_coords = new T[m_uiCount];
		m_coords[0] = p_vA;
		m_coords[1] = p_vB;
		m_coords[2] = p_vC;
	}

	template < typename T >
	template< typename U >
	DynPoint< T >::DynPoint( const U & p_vA, const U & p_vB, const U & p_vC, const U & p_vD )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
		,	m_uiCount( 4 )
	{
		m_coords = new T[m_uiCount];
		m_coords[0] = p_vA;
		m_coords[1] = p_vB;
		m_coords[2] = p_vC;
		m_coords[3] = p_vD;
	}

	template < typename T >
	template< typename U >
	DynPoint< T >::DynPoint( const DynPoint< U > & p_ptPoint )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
		,	m_uiCount( p_ptPoint.m_uiCount )
	{
		if ( m_uiCount > 0 )
		{
			m_coords = new T[m_uiCount];
		}

		for ( size_t i = 0 ; i < m_uiCount ; i++ )
		{
			Policy< T >::assign( m_coords[i], p_ptPoint[i] );
		}
	}

	template < typename T >
	inline DynPoint < T >::~DynPoint()
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
		}
	}
	template < typename T >
	inline void DynPoint < T >::LinkCoords( const void * p_pCoords )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
			m_coords = NULL;
		}

		m_coords = ( T * )p_pCoords;
		m_bOwnCoords = false;
	}
	template < typename T >
	inline void DynPoint < T >::UnlinkCoords()
	{
		if ( ! m_bOwnCoords )
		{
			m_coords = new T[m_uiCount];
		}

		m_bOwnCoords = true;
	}
	template < typename T >
	inline void DynPoint < T >::ToValues( T * p_pResult )const
	{
		if ( m_coords != NULL )
		{
			Policy< T >::assign( p_pResult[0], m_coords[0] );

			for ( size_t i = 0 ; i < m_uiCount ; i++ )
			{
				Policy< T >::assign( p_pResult[i], m_coords[i] );
			}
		}
	}

	template < typename T >
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

			for ( size_t i = 0 ; i < m_uiCount ; i++ )
			{
				m_coords[i] = p_pt[i];
			}
		}
		else
		{
			m_coords = p_pt.m_coords;
		}
	}
	template < typename T >
	DynPoint< T > & DynPoint < T >::operator =( const DynPoint< T > & p_pt )
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

			for ( size_t i = 0 ; i < m_uiCount ; i++ )
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
	template < typename T >
	template< typename U >
	DynPoint< T > & DynPoint < T >::operator =( const DynPoint< U > & p_pt )
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

			for ( size_t i = 0 ; i < m_uiCount ; i++ )
			{
				Policy< T >::assign( m_coords[i], p_pt.m_coords[i] );
			}
		}
		else
		{
			m_coords = p_pt.m_coords;
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator +=( const DynPoint< U > & p_pt )
	{
		for ( size_t i = 0 ; i < m_uiCount && i < p_pt.m_uiCount ; i++ )
		{
			Policy< T >::ass_add( at( i ), p_pt[i] );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator +=( const U * p_coords )
	{
		for ( size_t i = 0 ; i < m_uiCount ; i++ )
		{
			Policy< T >::ass_add( at( i ), p_coords[i] );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator +=( const U & p_coord )
	{
		for ( size_t i = 0 ; i < m_uiCount ; i++ )
		{
			Policy< T >::ass_add( at( i ), p_coord );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator -=( const DynPoint< U > & p_pt )
	{
		for ( size_t i = 0 ; i < m_uiCount && i < p_pt.m_uiCount ; i++ )
		{
			Policy< T >::ass_substract( at( i ), p_pt[i] );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator -=( const U * p_coords )
	{
		for ( size_t i = 0 ; i < m_uiCount ; i++ )
		{
			Policy< T >::ass_substract( at( i ), p_coords[i] );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator -=( const U & p_coord )
	{
		for ( size_t i = 0 ; i < m_uiCount ; i++ )
		{
			Policy< T >::ass_substract( at( i ), p_coord );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator *=( const DynPoint< U > & p_pt )
	{
		for ( size_t i = 0 ; i < m_uiCount && i < p_pt.m_uiCount ; i++ )
		{
			Policy< T >::ass_multiply( at( i ), p_pt[i] );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator *=( const U * p_coords )
	{
		for ( size_t i = 0 ; i < m_uiCount ; i++ )
		{
			Policy< T >::ass_multiply( at( i ), p_coords[i] );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator *=( const U & p_coord )
	{
		for ( size_t i = 0 ; i < m_uiCount ; i++ )
		{
			Policy< T >::ass_multiply( at( i ), p_coord );
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator /=( const DynPoint< U > & p_pt )
	{
		for ( size_t i = 0 ; i < m_uiCount && i < p_pt.m_uiCount ; i++ )
		{
			if ( ! Policy< T >::is_null( p_pt[i] ) )
			{
				Policy< T >::ass_divide( at( i ), p_pt[i] );
			}
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator /=( const U * p_coords )
	{
		for ( size_t i = 0 ; i < m_uiCount ; i++ )
		{
			if ( ! Policy< T >::is_null( p_coords[i] ) )
			{
				Policy< T >::ass_divide( at( i ), p_coords[i] );
			}
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator /=( const U & p_coord )
	{
		if ( ! Policy< T >::is_null( p_coord ) )
		{
			for ( size_t i = 0 ; i < m_uiCount ; i++ )
			{
				Policy< T >::ass_divide( at( i ), p_coord );
			}
		}

		return * this;
	}
	template < typename T >
	template< typename U >
	inline DynPoint < T > & DynPoint < T >::operator ^=( const DynPoint< U > & p_pt )
	{
		if ( m_uiCount == 3 && p_pt.m_uiCount == 3 )
		{
			T l_valuesA[3], l_valuesB[3];
			p_pt.ToValues( l_valuesA );
			ToValues( l_valuesB );
			Policy< T >::assign( m_coords[0], Policy< T >::substract( Policy< T >::multiply( l_valuesB[1], l_valuesA[2] ), Policy< T >::multiply( l_valuesA[1], l_valuesB[2] ) ) );
			Policy< T >::assign( m_coords[1], Policy< T >::substract( Policy< T >::multiply( l_valuesB[2], l_valuesA[0] ), Policy< T >::multiply( l_valuesA[2], l_valuesB[0] ) ) );
			Policy< T >::assign( m_coords[2], Policy< T >::substract( Policy< T >::multiply( l_valuesB[0], l_valuesA[1] ), Policy< T >::multiply( l_valuesA[0], l_valuesB[1] ) ) );
		}

		return * this;
	}
	template < typename T >
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
	inline bool operator ==( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		bool l_bReturn = ( p_ptA.GetElementCount() == p_ptB.GetElementCount() );

		for ( size_t i = 0 ; i < p_ptA.GetElementCount() && l_bReturn ; i++ )
		{
			l_bReturn = Policy< T >::equals( p_ptA[i], p_ptB[i] );
		}

		return l_bReturn;
	}
	template< typename T, typename U >
	inline bool operator !=( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		return !( p_ptA == p_ptB );
	}
	template < typename T >
	inline std::ostream & operator << ( std::ostream & l_streamOut, const DynPoint< T > & p_pt )
	{
		for ( size_t i = 0 ; i < p_pt.GetElementCount() ; i++ )
		{
			l_streamOut << "\t" << p_pt[i];
		}

		l_streamOut << std::endl;
		return l_streamOut;
	}
	template < typename T >
	inline std::istream & operator >> ( std::istream & l_streamIn, DynPoint< T > & p_pt )
	{
		for ( size_t i = 0 ; i < p_pt.GetElementCount() ; i++ )
		{
			l_streamIn >> p_pt[i];
		}

		return l_streamIn;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator +( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult += p_ptB;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator +( const DynPoint< T > & p_pt, const U * p_coords )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult += p_coords;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator +( const DynPoint< T > & p_pt, const U & p_coord )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult += p_coord;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator -( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult -= p_ptB;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator -( const DynPoint< T > & p_pt, const U * p_coords )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult -= p_coords;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator -( const DynPoint< T > & p_pt, const U & p_coord )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult -= p_coord;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator *( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult *= p_ptB;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator *( const DynPoint< T > & p_pt, const U * p_coords )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult *= p_coords;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator *( const DynPoint< T > & p_pt, const U & p_coord )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult *= p_coord;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator /( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult /= p_ptB;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator /( const DynPoint< T > & p_pt, const U * p_coords )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult /= p_coords;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator /( const DynPoint< T > & p_pt, const U & p_coord )
	{
		DynPoint < T > l_ptResult( p_pt );
		l_ptResult /= p_coord;
		return l_ptResult;
	}
	template< typename T, typename U >
	inline DynPoint < T > operator ^( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB )
	{
		DynPoint < T > l_ptResult( p_ptA );
		l_ptResult ^= p_ptB;
		return l_ptResult;
	}

	template < typename T >
	inline DynPoint < T > operator *( int p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint * p_value;
	}
	template < typename T >
	inline DynPoint < T > operator +( int p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint + p_value;
	}
	template < typename T >
	inline DynPoint < T > operator -( int p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint - p_value;
	}
	template < typename T >
	inline DynPoint < T > operator /( int p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint / p_value;
	}
	template < typename T >
	inline DynPoint < T > operator *( double p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint * p_value;
	}
	template < typename T >
	inline DynPoint < T > operator +( double p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint + p_value;
	}
	template < typename T >
	inline DynPoint < T > operator -( double p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint - p_value;
	}
	template < typename T >
	inline DynPoint < T > operator /( double p_value, const DynPoint < T > & p_ptPoint )
	{
		return p_ptPoint / p_value;
	}

//*************************************************************************************************

	template < typename T, size_t Count >
	Point < T, Count >::Point()
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
	{
		if ( Count > 0 )
		{
			m_coords = new T[Count];
		}

		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::init( at( i ) );
		}
	}

	template < typename T, size_t Count >
	template< typename U >
	Point < T, Count >::Point( const U * p_pValues )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
	{
		if ( Count > 0 )
		{
			m_coords = new T[Count];
		}

		if ( p_pValues == NULL )
		{
			for ( size_t i = 0 ; i < Count ; i++ )
			{
				Policy< T >::init( at( i ) );
			}
		}
		else
		{
			for ( size_t i = 0 ; i < Count ; i++ )
			{
				Policy< T >::assign( at( i ), p_pValues[i] );
			}
		}
	}
	template < typename T, size_t Count >
	template< typename U >
	Point < T, Count >::Point( const U & p_vA )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
	{
		if ( Count > 0 )
		{
			m_coords = new T[Count];
			Policy< T >::assign( at( 0 ), p_vA );
		}

		if ( Count > 1 )
		{
			for ( size_t i = 1 ; i < Count ; i++ )
			{
				Policy< T >::init( at( i ) );
			}
		}
	}
	template < typename T, size_t Count >
	template< typename U >
	Point < T, Count >::Point( const U & p_vA, const U & p_vB )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
	{
		if ( Count > 0 )
		{
			m_coords = new T[Count];
			Policy< T >::assign( at( 0 ), p_vA );
		}

		if ( Count > 1 )
		{
			Policy< T >::assign( at( 1 ), p_vB );

			for ( size_t i = 2 ; i < Count ; i++ )
			{
				Policy< T >::init( at( i ) );
			}
		}
	}
	template < typename T, size_t Count >
	template< typename U >
	Point < T, Count >::Point( const U & p_vA, const U & p_vB, const U & p_vC )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
	{
		if ( Count > 0 )
		{
			m_coords = new T[Count];
			Policy< T >::assign( at( 0 ), p_vA );
		}

		if ( Count > 1 )
		{
			Policy< T >::assign( at( 1 ), p_vB );
		}

		if ( Count > 2 )
		{
			Policy< T >::assign( at( 2 ), p_vC );

			for ( size_t i = 3 ; i < Count ; i++ )
			{
				Policy< T >::init( at( i ) );
			}
		}
	}
	template < typename T, size_t Count >
	template< typename U >
	Point < T, Count >::Point( const U & p_vA, const U & p_vB, const U & p_vC, const U & p_vD )
		:	m_coords( NULL )
		,	m_bOwnCoords( true )
	{
		if ( Count > 0 )
		{
			m_coords = new T[Count];
			Policy< T >::assign( at( 0 ), p_vA );
		}

		if ( Count > 1 )
		{
			Policy< T >::assign( at( 1 ), p_vB );
		}

		if ( Count > 2 )
		{
			Policy< T >::assign( at( 2 ), p_vC );
		}

		if ( Count > 3 )
		{
			Policy< T >::assign( at( 3 ), p_vD );

			for ( size_t i = 4 ; i < Count ; i++ )
			{
				Policy< T >::init( at( i ) );
			}
		}
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	Point < T, Count >::Point( const Point< U, _Count > & p_pt )
		:	m_coords( NULL )
		,	m_bOwnCoords( p_pt.m_bOwnCoords )
	{
		if ( m_bOwnCoords )
		{
			if ( Count > 0 )
			{
				m_coords = new T[Count];
			}

			size_t i;

			for ( i = 0 ; i < Count && i < _Count ; i++ )
			{
				Policy< T >::assign( m_coords[i], p_pt[i] );
			}

			for ( ; i < Count ; i++ )
			{
				Policy< T >::init( at( i ) );
			}
		}
		else
		{
			m_coords = ( T * )( p_pt.m_coords );
		}
	}
	template < typename T, size_t Count >
	inline Point < T, Count >::~Point()
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
		}
	}
	template < typename T, size_t Count >
	Point < T, Count >::Point( const Point< T, Count > & p_pt )
		:	m_coords( NULL )
		,	m_bOwnCoords( p_pt.m_bOwnCoords )
	{
		if ( m_bOwnCoords )
		{
			if ( Count > 0 )
			{
				m_coords = new T[Count];
			}

			for ( size_t i = 0 ; i < Count ; i++ )
			{
				m_coords[i] = p_pt[i];
			}
		}
		else
		{
			m_coords = p_pt.m_coords;
		}
	}
	template < typename T, size_t Count >
	inline Point< T, Count > & Point < T, Count >::operator =( const Point< T, Count > & p_pt )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
			m_coords = NULL;
		}

		m_bOwnCoords = p_pt.m_bOwnCoords;

		if ( m_bOwnCoords )
		{
			if ( Count > 0 )
			{
				m_coords = new T[Count];
			}

			for ( size_t i = 0 ; i < Count ; i++ )
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
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point< T, Count > & Point < T, Count >::operator =( const Point< U, _Count > & p_pt )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
			m_coords = NULL;
		}

		m_bOwnCoords = p_pt.m_bOwnCoords;

		if ( m_bOwnCoords )
		{
			if ( Count > 0 )
			{
				m_coords = new T[Count];
			}

			size_t i;

			for ( i = 0 ; i < Count && i < _Count ; i++ )
			{
				Policy< T >::assign( m_coords[i], p_pt[i] );
			}

			for ( ; i < Count ; i++ )
			{
				Policy< T >::init( m_coords[i] );
			}
		}
		else
		{
			m_coords = ( T * )( p_pt.m_coords );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point < T, Count > & Point < T, Count >::operator +=( const Point< U, _Count > & p_pt )
	{
		for ( size_t i = 0 ; i < Count && i < _Count ; i++ )
		{
			Policy< T >::ass_add( at( i ), p_pt[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point < T, Count > & Point < T, Count >::operator +=( const U * p_coords )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::ass_add( at( i ), p_coords[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point < T, Count > & Point < T, Count >::operator +=( const U & p_coord )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::ass_add( at( i ), p_coord );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point < T, Count > & Point < T, Count >::operator -=( const Point< U, _Count > & p_pt )
	{
		for ( size_t i = 0 ; i < Count && i < _Count ; i++ )
		{
			Policy< T >::ass_substract( at( i ), p_pt[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point < T, Count > & Point < T, Count >::operator -=( const U * p_coords )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::ass_substract( at( i ), p_coords[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point < T, Count > & Point < T, Count >::operator -=( const U & p_coord )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::ass_substract( at( i ), p_coord );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point < T, Count > & Point < T, Count >::operator *=( const Point< U, _Count > & p_pt )
	{
		for ( size_t i = 0 ; i < Count && i < _Count ; i++ )
		{
			Policy< T >::ass_multiply( at( i ), p_pt[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point < T, Count > & Point < T, Count >::operator *=( const U * p_coords )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::ass_multiply( at( i ), p_coords[i] );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point < T, Count > & Point < T, Count >::operator *=( const U & p_coord )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::ass_multiply( at( i ), p_coord );
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point < T, Count > & Point < T, Count >::operator /=( const Point< U, _Count > & p_pt )
	{
		for ( size_t i = 0 ; i < Count && i < _Count ; i++ )
		{
			if ( ! Policy< T >::is_null( p_pt[i] ) )
			{
				Policy< T >::ass_divide( at( i ), p_pt[i] );
			}
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point < T, Count > & Point < T, Count >::operator /=( const U * p_coords )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			if ( ! Policy< T >::is_null( p_coords[i] ) )
			{
				Policy< T >::ass_divide( at( i ), p_coords[i] );
			}
		}

		return * this;
	}
	template < typename T, size_t Count >
	template< typename U >
	inline Point < T, Count > & Point < T, Count >::operator /=( const U & p_coord )
	{
		if ( ! Policy< T >::is_null( p_coord ) )
		{
			for ( size_t i = 0 ; i < Count ; i++ )
			{
				Policy< T >::ass_divide( at( i ), p_coord );
			}
		}

		return * this;
	}
	template < typename T, size_t Count >
	template < typename U, size_t _Count >
	inline Point < T, Count > & Point < T, Count >::operator ^=( const Point< U, _Count > & p_pt )
	{
		if ( Count == 3 )
		{
			T l_valuesA[Count], l_valuesB[Count];
			p_pt.ToValues( l_valuesA );
			ToValues( l_valuesB );
			Policy< T >::assign( m_coords[0], Policy< T >::substract( Policy< T >::multiply( l_valuesB[1], l_valuesA[2] ), Policy< T >::multiply( l_valuesA[1], l_valuesB[2] ) ) );
			Policy< T >::assign( m_coords[1], Policy< T >::substract( Policy< T >::multiply( l_valuesB[2], l_valuesA[0] ), Policy< T >::multiply( l_valuesA[2], l_valuesB[0] ) ) );
			Policy< T >::assign( m_coords[2], Policy< T >::substract( Policy< T >::multiply( l_valuesB[0], l_valuesA[1] ), Policy< T >::multiply( l_valuesA[0], l_valuesB[1] ) ) );
		}

		return * this;
	}
	template < typename T, size_t Count >
	inline void Point < T, Count >::Reverse()
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::ass_negate( at( i ) );
		}
	}
	template < typename T, size_t Count >
	inline void Point < T, Count >::Normalise()
	{
		T l_length = GetLength();

		if ( Policy< T >::is_null( l_length ) )
		{
			return;
		}

		operator /=( l_length );
	}
	template < typename T, size_t Count >
	inline Point< T, Count > Point < T, Count >::GetNormalised()const
	{
		Point< T, Count > l_ptReturn( * this );
		l_ptReturn.Normalise();
		return l_ptReturn;
	}
	template < typename T, size_t Count >
	inline T Point < T, Count >::Dot( const Point< T, Count > & p_vertex )const
	{
		T l_tReturn;
		Policy< T >::init( l_tReturn );

		for ( size_t i = 0 ; i < Count ; i++ )
		{
			Policy< T >::ass_add( l_tReturn, Policy< T >::multiply( at( i ), p_vertex[i] ) );
		}

		return l_tReturn;
	}
	template < typename T, size_t Count >
	inline double Point < T, Count >::GetSquaredLength()const
	{
		double l_dReturn = 0.0;

		for ( size_t i = 0 ; i < Count ; i++ )
		{
			l_dReturn += Policy< T >::multiply( at( i ), at( i ) );
		}

		return l_dReturn;
	}
	template < typename T, size_t Count >
	inline double Point < T, Count >::GetLength()const
	{
		return sqrt( this->GetSquaredLength() );
	}
	template < typename T, size_t Count >
	inline double Point < T, Count >::GetManhattanLength()const
	{
		double l_dReturn = 0.0;

		for ( size_t i = 0 ; i < Count ; i++ )
		{
			l_dReturn += abs< double >( at( i ) );
		}

		return l_dReturn;
	}
	template < typename T, size_t Count >
	inline double Point < T, Count >::GetMinkowskiLength( double p_dOrder )const
	{
		double l_dReturn = 0.0;

		for ( size_t i = 0 ; i < Count ; i++ )
		{
			l_dReturn += std::pow( abs< double >( at( i ) ), p_dOrder );
		}

		l_dReturn = std::pow( l_dReturn, 1.0 / p_dOrder );
		return l_dReturn;
	}
	template < typename T, size_t Count >
	inline double Point < T, Count >::GetChebychevLength()const
	{
		double l_dReturn = 0.0;

		for ( size_t i = 0 ; i < Count ; i++ )
		{
			l_dReturn = std::max( l_dReturn, abs< double >( at( i ) ) );
		}

		return l_dReturn;
	}
	template < typename T, size_t Count >
	inline double Point < T, Count >::GetCosTheta( const Point< T, Count > & p_vector )const
	{
		return Dot( p_vector ) / ( GetLength() * p_vector.GetLength() );
	}
	template < typename T, size_t Count >
	inline void Point < T, Count >::LinkCoords( const void * p_pCoords )
	{
		if ( m_bOwnCoords )
		{
			delete [] m_coords;
			m_coords = NULL;
		}

		m_coords = ( T * )p_pCoords;
		m_bOwnCoords = false;
	}
	template < typename T, size_t Count >
	inline void Point < T, Count >::UnlinkCoords()
	{
		if ( ! m_bOwnCoords )
		{
			m_coords = new T[Count];
		}

		m_bOwnCoords = true;
	}
	template < typename T, size_t Count >
	void Point < T, Count >::swap( Point< T, Count > & p_pt )
	{
		std::swap( m_coords, p_pt.m_coords );
		std::swap( m_bOwnCoords, p_pt.m_bOwnCoords );
	}
	template < typename T, size_t Count >
	inline void Point < T, Count >::ToValues( T * p_pResult )const
	{
		if ( m_coords != NULL )
		{
			Policy< T >::assign( p_pResult[0], m_coords[0] );

			for ( size_t i = 0 ; i < Count ; i++ )
			{
				Policy< T >::assign( p_pResult[i], m_coords[i] );
			}
		}
	}

//*************************************************************************************************

	template < typename T, size_t Count, typename U, size_t _Count >
	inline bool operator ==( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		bool l_bReturn = ( Count == _Count );

		for ( size_t i = 0 ; i < Count && l_bReturn ; i++ )
		{
			l_bReturn = Policy< T >::equals( p_ptA[i], p_ptB[i] );
		}

		return l_bReturn;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline bool operator !=( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		return !( p_ptA == p_ptB );
	}
	template < typename T, size_t Count >
	inline std::ostream & operator << ( std::ostream & l_streamOut, const Point< T, Count > & p_pt )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			l_streamOut << "\t" << p_pt[i];
		}

		l_streamOut << std::endl;
		return l_streamOut;
	}
	template < typename T, size_t Count >
	inline std::istream & operator >> ( std::istream & l_streamIn, Point< T, Count > & p_pt )
	{
		for ( size_t i = 0 ; i < Count ; i++ )
		{
			l_streamIn >> p_pt[i];
		}

		return l_streamIn;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point < T, Count > operator +( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point < T, Count > l_ptResult( p_ptA );
		l_ptResult += p_ptB;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point < T, Count > operator +( const Point< T, Count > & p_pt, const U * p_coords )
	{
		Point < T, Count > l_ptResult( p_pt );
		l_ptResult += p_coords;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point < T, Count > operator +( const Point< T, Count > & p_pt, const U & p_coord )
	{
		Point < T, Count > l_ptResult( p_pt );
		l_ptResult += p_coord;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point < T, Count > operator -( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point < T, Count > l_ptResult( p_ptA );
		l_ptResult -= p_ptB;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point < T, Count > operator -( const Point< T, Count > & p_pt, const U * p_coords )
	{
		Point < T, Count > l_ptResult( p_pt );
		l_ptResult -= p_coords;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point < T, Count > operator -( const Point< T, Count > & p_pt, const U & p_coord )
	{
		Point < T, Count > l_ptResult( p_pt );
		l_ptResult -= p_coord;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point < T, Count > operator *( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point < T, Count > l_ptResult( p_ptA );
		l_ptResult *= p_ptB;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point < T, Count > operator *( const Point< T, Count > & p_pt, const U * p_coords )
	{
		Point < T, Count > l_ptResult( p_pt );
		l_ptResult *= p_coords;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point < T, Count > operator *( const Point< T, Count > & p_pt, const U & p_coord )
	{
		Point < T, Count > l_ptResult( p_pt );
		l_ptResult *= p_coord;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point < T, Count > operator /( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point < T, Count > l_ptResult( p_ptA );
		l_ptResult /= p_ptB;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point < T, Count > operator /( const Point< T, Count > & p_pt, const U * p_coords )
	{
		Point < T, Count > l_ptResult( p_pt );
		l_ptResult /= p_coords;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U >
	inline Point < T, Count > operator /( const Point< T, Count > & p_pt, const U & p_coord )
	{
		Point < T, Count > l_ptResult( p_pt );
		l_ptResult /= p_coord;
		return l_ptResult;
	}
	template < typename T, size_t Count, typename U, size_t _Count >
	inline Point < T, Count > operator ^( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB )
	{
		Point < T, Count > l_ptResult( p_ptA );
		l_ptResult ^= p_ptB;
		return l_ptResult;
	}

	template < typename T, size_t Count >
	inline Point < T, Count > operator *( int p_value, const Point < T, Count > & p_ptPoint )
	{
		return p_ptPoint * p_value;
	}
	template < typename T, size_t Count >
	inline Point < T, Count > operator +( int p_value, const Point < T, Count > & p_ptPoint )
	{
		return p_ptPoint + p_value;
	}
	template < typename T, size_t Count >
	inline Point < T, Count > operator -( int p_value, const Point < T, Count > & p_ptPoint )
	{
		return p_ptPoint - p_value;
	}
	template < typename T, size_t Count >
	inline Point < T, Count > operator /( int p_value, const Point < T, Count > & p_ptPoint )
	{
		return p_ptPoint / p_value;
	}
	template < typename T, size_t Count >
	inline Point < T, Count > operator *( double p_value, const Point < T, Count > & p_ptPoint )
	{
		return p_ptPoint * p_value;
	}
	template < typename T, size_t Count >
	inline Point < T, Count > operator +( double p_value, const Point < T, Count > & p_ptPoint )
	{
		return p_ptPoint + p_value;
	}
	template < typename T, size_t Count >
	inline Point < T, Count > operator -( double p_value, const Point < T, Count > & p_ptPoint )
	{
		return p_ptPoint - p_value;
	}
	template < typename T, size_t Count >
	inline Point < T, Count > operator /( double p_value, const Point < T, Count > & p_ptPoint )
	{
		return p_ptPoint / p_value;
	}

//*************************************************************************************************
}

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif
