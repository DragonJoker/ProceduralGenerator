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
#ifndef ___GENERATOR_POINT_H___
#define ___GENERATOR_POINT_H___

#include "Value.h"

#include <cmath>

#if HAS_INITIALIZER_LISTS
#	include <initializer_list>
#endif

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Base class for the point classes
	*/
	class GeneratorAPI PointBase
	{
	public:
		/**
		 *\brief		Default constructor
		 */
		PointBase()
		{
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~PointBase()
		{
		}
		/**
		 *\brief		Copy constructor
		 */
		PointBase( PointBase const & )
		{
		}
		/**
		 *\~english
		 *\brief		Copy assignment operator
		 *\return		A reference to this Point object
		 */
		virtual PointBase & operator =( PointBase const & )
		{
			return * this;
		}
		/**
		 *\brief		Retrieves the number of coordinates
		 *\return		The number of coordinates
		 */
		virtual size_t GetElementsCount()const = 0;
		/**
		 *\brief		Retrieves the coordinate type size
		 *\return		The data type size
		 */
		virtual size_t GetElementSize()const = 0;
		/**
		 *\~english
		 *\brief		Assigns this point coordinates to the given ones
		 *\remarks		The point no longer owns its coords buffer
		 *\param[in]	p_pCoords	The coords buffer
		 */
		virtual void Link( const void * p_pCoords ) = 0;
		/**
		 *\~english
		 *\brief		Reassigns this point coordinates to a new buffer
		 *\remarks		The point owns its coords buffer
		 */
		virtual void Unlink() = 0;
		/**
		 *\brief		Retrieves the pointer on datas
		 *\return		The pointer
		 */
		template< typename T > T * Ptr()
		{
			return ( T * )DoPtr();
		}
		/**
		 *\brief		Retrieves a constant pointer on datas
		 *\return		The pointer
		 */
		template< typename T > const T * ConstPtr()const
		{
			return ( const T * )DoConstPtr();
		}

	protected:
		/**
		 *\brief		Retrieves the pointer on datas
		 *\return		The pointer
		 */
		virtual void * DoPtr()
		{
			return NULL;
		}
		/**
		 *\brief		Retrieves a constant pointer on datas
		 *\return		The pointer
		 */
		virtual const void * DoConstPtr()const
		{
			return NULL;
		}
	};

	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Templated dynamic dimensions point representation
	*/
	template< typename T >
	class DynPoint
		: public PointBase
	{
	private:
		template< typename U > friend class DynPoint;

		typedef T value_type;
		typedef value_type & reference;
		typedef value_type * pointer;
		typedef value_type const & const_reference;
		typedef const value_type * const_pointer;
		typedef DynPoint< value_type > point;
		typedef DynPoint< value_type > & point_reference;
		typedef DynPoint< value_type > * point_pointer;
		typedef const DynPoint< value_type > & const_point_reference;
		typedef const DynPoint< value_type > * const_point_pointer;

	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_uiCount	The values count
		 */
		DynPoint( size_t p_uiCount );
#if HAS_INITIALIZER_LISTS
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_values	The values list
		 */
		template< typename U > DynPoint( std::initializer_list< U > p_values );
#endif
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_uiCount	The values count
		 *\param[in]	p_pValues	The data buffer
		 */
		template< typename U > DynPoint( size_t p_uiCount, const U * p_pValues );
		/**
		 *\brief		Specified constructor, initialises the point to a 2 coordinates one
		 *\param[in]	p_vA	The first value
		 *\param[in]	p_vB	The second value
		 */
		template< typename U > DynPoint( U const & p_vA, U const & p_vB );
		/**
		 *\brief		Specified constructor, initialises the point to a 3 coordinates one
		 *\param[in]	p_vA	The first value
		 *\param[in]	p_vB	The second value
		 *\param[in]	p_vC	The third value
		 */
		template< typename U > DynPoint( U const & p_vA, U const & p_vB, U const & p_vC );
		/**
		 *\brief		Specified constructor, initialises the point to a 4 coordinates one
		 *\param[in]	p_vA	The first value
		 *\param[in]	p_vB	The second value
		 *\param[in]	p_vC	The third value
		 *\param[in]	p_vD	The fourth value
		 */
		template< typename U > DynPoint( U const & p_vA, U const & p_vB, U const & p_vC, U const & p_vD );
		/**
		 *\brief		Constructor from another point type
		 *\param[in]	p_ptPoint	The Point object to copy
		 */
		template< typename U > DynPoint( const DynPoint< U > & p_ptPoint );
		/**
		 *\brief		Destructor
		 */
		virtual ~DynPoint();
		/**
		 *\brief		Copy constructor
		 *\param[in]	p_ptPoint	The Point object to copy
		 */
		DynPoint( const DynPoint< T > & p_ptPoint );
		/**
		 *\~english
		 *\brief		Copy assignment operator
		 *\param[in]	p_pt	The Point object to copy
		 *\return		A reference to this Point object
		 */
		DynPoint< T > & operator =( const DynPoint< T > & p_pt );
		/**
		 *\~english
		 *\brief		Assignment operator from another point type
		 *\param[in]	p_pt	The Point object to copy
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator =( const DynPoint< U > & p_pt );
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_pt	The Point object to add
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator +=( const DynPoint< U > & p_pt );
		/**
		 *\brief		Substraction assignment operator
		 *\param[in]	p_pt	The Point object to subtract
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator -=( const DynPoint< U > & p_pt );
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_pt	The Point object to multiply
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator *=( const DynPoint< U > & p_pt );
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_pt	The Point object to divide
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator /=( const DynPoint< U > & p_pt );
		/**
		 *\brief		Scalar product assignment operator
		 *\param[in]	p_pt	The other Point object
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator ^=( const DynPoint< U > & p_pt );
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_coords	The values to add
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator +=( const U * p_coords );
		/**
		 *\brief		Substraction assignment operator
		 *\param[in]	p_coords	The values to subtract
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator -=( const U * p_coords );
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_coords	The values to multiply
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator *=( const U * p_coords );
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_coords	The values to divide
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator /=( const U * p_coords );
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_coord	The value to add
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator +=( U const & p_coord );
		/**
		 *\brief		Substraction assignment operator
		 *\param[in]	p_coord	The value to subtract
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator -=( U const & p_coord );
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_coord	The value to multiply
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator *=( U const & p_coord );
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_coord	The value to divide
		 *\return		A reference to this Point object
		 */
		template< typename U > DynPoint< T > & operator /=( U const & p_coord );
		/**
		 *\brief		Swaps this pont data with the parameter ones
		 *\param[in]	p_pt	The point to swap
		 */
		void swap( DynPoint< T > & p_pt );
		/**
		 *\brief		Retrieves the number of coordinates
		 *\return		The number of coordinates
		 */
		virtual size_t GetElementsCount()const
		{
			return m_uiCount;
		}
		/**
		 *\brief		Retrieves the coordinate type size
		 *\return		The data type size
		 */
		virtual size_t GetElementSize()const
		{
			return sizeof( T );
		}
		/**
		 *\~english
		 *\brief		Assigns this point coordinates to the given ones
		 *\remarks		The point no longer owns its coords buffer
		 *\param[in]	p_pCoords	The coords buffer
		 */
		virtual void Link( const void * p_pCoords );
		/**
		 *\~english
		 *\brief		Reassigns this point coordinates to a new buffer
		 *\remarks		The point owns its coords buffer
		 */
		virtual void Unlink();
		/**
		 *\brief			Retrieves data from the point and put it into an array
		 *\param[in,out]	p_pResult	Receives the point data, needs to be allocated by the caller
		 */
		void ToValues( T * p_pResult )const;
		/**
		 *\brief		Retrieves the data At given index
		 *\remarks		No check is made, if you make an index error, expect a crash
		 *\param[in]	p_pos	The index
		 *\return		A constant reference on data At wanted index
		 */
		inline T const & operator[]( size_t p_pos )const
		{
			return m_coords[p_pos];
		}
		/**
		 *\brief		Retrieves the data At given index
		 *\remarks		No check is made, if you make an index error, expect a crash
		 *\param[in]	p_pos	The index
		 *\return		A reference on data At wanted index
		 */
		inline T & operator[]( size_t p_pos )
		{
			return m_coords[p_pos];
		}
		/**
		 *\brief		Retrieves the data At given index
		 *\remarks		This fonction checks the index and throws an exception if it is out of bounds
		 *\param[in]	p_pos	The index
		 *\return		A constant reference on data At wanted index
		 */
		inline T const & At( size_t p_pos )const
		{
			return m_coords[p_pos];
		}
		/**
		 *\brief		Retrieves the data At given index
		 *\remarks		This fonction checks the index and throws an exception if it is out of bounds
		 *\param[in]	p_pos	The index
		 *\return		A reference on data At wanted index
		 */
		inline T & At( size_t p_pos )
		{
			return m_coords[p_pos];
		}
		/**
		 *\brief		Retrieves the pointer on datas
		 *\return		The pointer
		 */
		inline T * Ptr()
		{
			return m_coords;
		}
		/**
		 *\brief		Retrieves a constant pointer on datas
		 *\return		The pointer
		 */
		inline const T * ConstPtr()const
		{
			return m_coords;
		}

	private:
		inline void * DoPtr()
		{
			return m_coords;
		}
		inline const void * DoConstPtr()const
		{
			return m_coords;
		}

	private:
		//! The coordinates count
		size_t m_uiCount;
		//! Tells if the point owns its coords buffer
		bool m_bOwnCoords;
		//! The coordinates buffer
		T * m_coords;

	};
	/**
	 *\brief		Equality operator
	 *\param[in]	p_ptA, p_ptB	The points to compare
	 *\return		\p true if points have same dimensions and same values
	 */
	template < typename T, typename U > bool operator == ( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB );
	/**
	 *\brief		Difference operator
	 *\param[in]	p_ptA, p_ptB	The points to compare
	 *\return		\p true if points have different dimensions or At least one different value
	 */
	template < typename T, typename U > bool operator != ( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB );
	/**
	 *\brief			Stream operator
	 *\param[in,out]	p_streamOut	The stream receiving point's data
	 *\param[in]		p_pt		The input point
	 *\return			A reference to the stream
	 */
	template< typename T > std::ostream & operator << ( std::ostream & p_streamOut, const DynPoint< T > & p_pt );
	/**
	 *\brief			Stream operator
	 *\param[in,out]	p_streamIn	The stream holding point's data
	 *\param[in,out]	p_pt		The output point
	 *\return			A reference to the stream
	 */
	template< typename T > std::istream & operator >> ( std::istream & p_streamIn, DynPoint< T > & p_pt );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_ptA, p_ptB	The points to add
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator + ( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_ptA, p_ptB	The points to subtract
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator - ( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_ptA, p_ptB	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator * ( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB );
	/**
	 *\brief		Division operator
	 *\param[in]	p_ptA, p_ptB	The points to divide
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator / ( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB );
	/**
	 *\brief		Scalar product operator
	 *\param[in]	p_ptA, p_ptB	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator ^ ( const DynPoint< T > & p_ptA, const DynPoint< U > & p_ptB );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_pt, p_coords	The points to add
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator + ( const DynPoint< T > & p_pt, const U * p_coords );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_pt, p_coords	The points to subtract
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator - ( const DynPoint< T > & p_pt, const U * p_coords );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_pt, p_coords	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator * ( const DynPoint< T > & p_pt, const U * p_coords );
	/**
	 *\brief		Division operator
	 *\param[in]	p_pt, p_coords	The points to divide
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator / ( const DynPoint< T > & p_pt, const U * p_coords );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_pt, p_coord	The points to add
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator + ( const DynPoint< T > & p_pt, U const & p_coord );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_pt, p_coord	The points to subtract
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator - ( const DynPoint< T > & p_pt, U const & p_coord );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_pt, p_coord	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator * ( const DynPoint< T > & p_pt, U const & p_coord );
	/**
	 *\brief		Division operator
	 *\param[in]	p_pt, p_coord	The points to divide
	 *\return		The addition result
	 */
	template < typename T, typename U > DynPoint< T > operator / ( const DynPoint< T > & p_pt, U const & p_coord );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_value, p_pt	The points to multiply
	 *\return		The addition result
	 */
	template< typename T > DynPoint< T > operator * ( int p_value, const DynPoint< T > & p_pt );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_value, p_pt	The points to add
	 *\return		The addition result
	 */
	template< typename T > DynPoint< T > operator + ( int p_value, const DynPoint< T > & p_pt );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_value, p_pt	The points to subtract
	 *\return		The addition result
	 */
	template< typename T > DynPoint< T > operator - ( int p_value, const DynPoint< T > & p_pt );
	/**
	 *\brief		Division operator
	 *\param[in]	p_value, p_pt	The points to divide
	 *\return		The addition result
	 */
	template< typename T > DynPoint< T > operator / ( int p_value, const DynPoint< T > & p_pt );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_value, p_pt	The points to multiply
	 *\return		The addition result
	 */
	template< typename T > DynPoint< T > operator * ( double p_value, const DynPoint< T > & p_pt );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_value, p_pt	The points to add
	 *\return		The addition result
	 */
	template< typename T > DynPoint< T > operator + ( double p_value, const DynPoint< T > & p_pt );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_value, p_pt	The points to subtract
	 *\return		The addition result
	 */
	template< typename T > DynPoint< T > operator - ( double p_value, const DynPoint< T > & p_pt );
	/**
	 *\brief		Division operator
	 *\param[in]	p_value, p_pt	The points to divide
	 *\return		The addition result
	 */
	template< typename T > DynPoint< T > operator / ( double p_value, const DynPoint< T > & p_pt );

	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Templated static dimensions point representation
	*/
	template < typename T, size_t Count >
	class Point
		: public PointBase
	{
	private:
		template < typename U, size_t _Count > friend class Point;

		typedef T value_type;
		typedef value_type & reference;
		typedef value_type * pointer;
		typedef value_type const & const_reference;
		typedef const value_type * const_pointer;
		typedef Point< value_type, Count > point;
		typedef Point< value_type, Count > & point_reference;
		typedef Point< value_type, Count > * point_pointer;
		typedef const Point< value_type, Count > & const_point_reference;
		typedef const Point< value_type, Count > * const_point_pointer;

	public:
		/**
		 *\brief		Default constructor
		 */
		Point();
#if HAS_INITIALIZER_LISTS
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_values	The values list
		 */
		template< typename U > Point( std::initializer_list< U > p_values );
#endif
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_pValues	The data buffer
		 */
		template< typename U > Point( const U * p_pValues );
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_vA	The first value
		 */
		template< typename U > Point( U const & p_vA );
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_vA	The first value
		 *\param[in]	p_vB	The second value
		 */
		template< typename U > Point( U const & p_vA, U const & p_vB );
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_vA	The first value
		 *\param[in]	p_vB	The second value
		 *\param[in]	p_vC	The third value
		 */
		template< typename U > Point( U const & p_vA, U const & p_vB, U const & p_vC );
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_vA	The first value
		 *\param[in]	p_vB	The second value
		 *\param[in]	p_vC	The third value
		 *\param[in]	p_vD	The fourth value
		 */
		template< typename U > Point( U const & p_vA, U const & p_vB, U const & p_vC, U const & p_vD );
		/**
		 *\brief		Constructor from another point type
		 *\param[in]	p_ptPoint	The Point object to copy
		 */
		template < typename U, size_t _Count > Point( const Point< U, _Count > & p_ptPoint );
		/**
		 *\brief		Destructor
		 */
		virtual ~Point();
		/**
		 *\brief		Copy constructor
		 *\param[in]	p_ptPoint	The Point object to copy
		 */
		Point( const Point< T, Count > & p_ptPoint );
		/**
		 *\~english
		 *\brief		Copy assignment operator
		 *\param[in]	p_pt	The Point object to copy
		 *\return		A reference to this Point object
		 */
		Point< T, Count > & operator =( const Point< T, Count > & p_pt );
		/**
		 *\~english
		 *\brief		Assignment operator from another point type
		 *\param[in]	p_pt	The Point object to copy
		 *\return		A reference to this Point object
		 */
		template < typename U, size_t _Count > Point< T, Count > & operator = ( const Point< U, _Count > & p_pt );
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_pt	The Point object to add
		 *\return		A reference to this Point object
		 */
		template < typename U, size_t _Count > Point< T, Count > & operator += ( const Point< U, _Count > & p_pt );
		/**
		 *\brief		Substraction assignment operator
		 *\param[in]	p_pt	The Point object to subtract
		 *\return		A reference to this Point object
		 */
		template < typename U, size_t _Count > Point< T, Count > & operator -= ( const Point< U, _Count > & p_pt );
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_pt	The Point object to multiply
		 *\return		A reference to this Point object
		 */
		template < typename U, size_t _Count > Point< T, Count > & operator *= ( const Point< U, _Count > & p_pt );
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_pt	The Point object to divide
		 *\return		A reference to this Point object
		 */
		template < typename U, size_t _Count > Point< T, Count > & operator /= ( const Point< U, _Count > & p_pt );
		/**
		 *\brief		Scalar product assignment operator
		 *\param[in]	p_pt	The other Point object
		 *\return		A reference to this Point object
		 */
		template < typename U, size_t _Count > Point< T, Count > & operator ^= ( const Point< U, _Count > & p_pt );
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_coords	The values to add
		 *\return		A reference to this Point object
		 */
		template < typename U > Point< T, Count > & operator += ( const U * p_coords );
		/**
		 *\brief		Substraction assignment operator
		 *\param[in]	p_coords	The values to subtract
		 *\return		A reference to this Point object
		 */
		template < typename U > Point< T, Count > & operator -= ( const U * p_coords );
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_coords	The values to multiply
		 *\return		A reference to this Point object
		 */
		template < typename U > Point< T, Count > & operator *= ( const U * p_coords );
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_coords	The values to divide
		 *\return		A reference to this Point object
		 */
		template < typename U > Point< T, Count > & operator /= ( const U * p_coords );
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_coord	The value to add
		 *\return		A reference to this Point object
		 */
		template < typename U > Point< T, Count > & operator += ( U const & p_coord );
		/**
		 *\brief		Substraction assignment operator
		 *\param[in]	p_coord	The value to subtract
		 *\return		A reference to this Point object
		 */
		template < typename U > Point< T, Count > & operator -= ( U const & p_coord );
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_coord	The value to multiply
		 *\return		A reference to this Point object
		 */
		template < typename U > Point< T, Count > & operator *= ( U const & p_coord );
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_coord	The value to divide
		 *\return		A reference to this Point object
		 */
		template < typename U > Point< T, Count > & operator /= ( U const & p_coord );
		/**
		 *\~english
		 *\brief		Assigns this point coordinates to the given ones
		 *\remarks		The point no longer owns its coords buffer
		 *\param[in]	p_pCoords	The coords buffer
		 */
		void Link( const void * p_pCoords );
		/**
		 *\~english
		 *\brief		Reassigns this point coordinates to a new buffer
		 *\remarks		The point owns its coords buffer
		 */
		void Unlink();
		/**
		 *\brief		Swaps this pont data with the parameter ones
		 *\param[in]	p_pt	The point to swap
		 */
		void swap( point_reference p_pt );
		/**
		 *\brief			Retrieves data from the point and put it into an array
		 *\param[in,out]	p_pResult	Receives the point data, needs to be allocated by the caller
		 */
		void ToValues( T * p_pResult )const;
		/**
		 *\brief		Retrieves the number of coordinates
		 *\return		The number of coordinates
		 */
		inline size_t GetElementsCount()const
		{
			return Count;
		}
		/**
		 *\brief		Retrieves the coordinate type size
		 *\return		The data type size
		 */
		inline size_t GetElementSize()const
		{
			return sizeof( T );
		}
		/**
		 *\brief		Retrieves the data At given index
		 *\remarks		No check is made, if you make an index error, expect a crash
		 *\return		A reference on data At wanted index
		 */
		inline T const & operator[]( size_t p_pos )const
		{
			return m_coords.sta->table[p_pos];
		}
		/**
		 *\brief		Retrieves the data At given index
		 *\remarks		No check is made, if you make an index error, expect a crash
		 *\return		A constant reference on data At wanted index
		 */
		inline T & operator[]( size_t p_pos )
		{
			return m_coords.sta->table[p_pos];
		}
		/**
		 *\brief		Retrieves the data At given index
		 *\remarks		This fonction checks the index and throws an exception if it is out of bounds
		 *\return		A constant reference on data At wanted index
		 */
		inline T const & At( size_t p_pos )const
		{
			return m_coords.sta->table[p_pos];
		}
		/**
		 *\brief		Retrieves the data At given index
		 *\remarks		This fonction checks the index and throws an exception if it is out of bounds
		 *\return		A reference on data At wanted index
		 */
		inline T & At( size_t p_pos )
		{
			return m_coords.sta->table[p_pos];
		}
		/**
		 *\brief		Retrieves the pointer on datas
		 *\return		The pointer
		 */
		inline T * Ptr()
		{
			return m_coords.dyn;
		}
		/**
		 *\brief		Retrieves a constant pointer on datas
		 *\return		The pointer
		 */
		inline const T * ConstPtr()const
		{
			return m_coords.dyn;
		}

	private:
		inline void * DoPtr()
		{
			return m_coords.dyn;
		}
		inline const void * DoConstPtr()const
		{
			return m_coords.dyn;
		}

	private:
		//! Tells if the point owns its coords buffer
		bool m_bOwnCoords;
		struct StaticTable
		{
			T table[Count];
		};
		//! The coords buffer
		union
		{
			T * dyn;
			StaticTable * sta;
		}
		m_coords;
	};
	/**
	 *\brief		Equality operator
	 *\param[in]	p_ptA, p_ptB	The points to compare
	 *\return		\p true if points have same dimensions and same values
	 */
	template < typename T, size_t Count, typename U, size_t _Count > bool operator == ( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB );
	/**
	 *\brief		Difference operator
	 *\param[in]	p_ptA, p_ptB	The points to compare
	 *\return		\p true if points have different dimensions or At least one different value
	 */
	template < typename T, size_t Count, typename U, size_t _Count > bool operator != ( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB );
	/**
	 *\brief			Stream operator
	 *\param[in,out]	p_streamOut	The stream receiving point's data
	 *\param[in]		p_pt		The input point
	 *\return			A reference to the stream
	 */
	template < typename T, size_t Count > std::ostream & operator << ( std::ostream & p_streamOut, const Point< T, Count > & p_pt );
	/**
	 *\brief			Stream operator
	 *\param[in,out]	p_streamIn	The stream holding point's data
	 *\param[in,out]	p_pt		The output point
	 *\return			A reference to the stream
	 */
	template < typename T, size_t Count > std::istream & operator >> ( std::istream & p_streamIn, Point< T, Count > & p_pt );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_ptA, p_ptB	The points to add
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U, size_t _Count > Point< T, Count > operator + ( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_ptA, p_ptB	The points to subtract
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U, size_t _Count > Point< T, Count > operator - ( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_ptA, p_ptB	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U, size_t _Count > Point< T, Count > operator * ( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB );
	/**
	 *\brief		Division operator
	 *\param[in]	p_ptA, p_ptB	The points to divide
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U, size_t _Count > Point< T, Count > operator / ( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB );
	/**
	 *\brief		Scalar product operator
	 *\param[in]	p_ptA, p_ptB	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U, size_t _Count > Point< T, Count > operator ^ ( const Point< T, Count > & p_ptA, const Point< U, _Count > & p_ptB );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_pt, p_coords	The points to add
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U > Point< T, Count > operator + ( const Point< T, Count > & p_pt, const U * p_coords );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_pt, p_coords	The points to subtract
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U > Point< T, Count > operator - ( const Point< T, Count > & p_pt, const U * p_coords );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_pt, p_coords	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U > Point< T, Count > operator * ( const Point< T, Count > & p_pt, const U * p_coords );
	/**
	 *\brief		Division operator
	 *\param[in]	p_pt, p_coords	The points to divide
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U > Point< T, Count > operator / ( const Point< T, Count > & p_pt, const U * p_coords );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_pt, p_coord	The points to add
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U > Point< T, Count > operator + ( const Point< T, Count > & p_pt, U const & p_coord );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_pt, p_coord	The points to subtract
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U > Point< T, Count > operator - ( const Point< T, Count > & p_pt, U const & p_coord );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_pt, p_coord	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U > Point< T, Count > operator * ( const Point< T, Count > & p_pt, U const & p_coord );
	/**
	 *\brief		Division operator
	 *\param[in]	p_pt, p_coord	The points to divide
	 *\return		The addition result
	 */
	template < typename T, size_t Count, typename U > Point< T, Count > operator / ( const Point< T, Count > & p_pt, U const & p_coord );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_value, p_pt	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, size_t Count > Point< T, Count > operator * ( int p_value, const Point< T, Count > & p_pt );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_value, p_pt	The points to add
	 *\return		The addition result
	 */
	template < typename T, size_t Count > Point< T, Count > operator + ( int p_value, const Point< T, Count > & p_pt );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_value, p_pt	The points to subtract
	 *\return		The addition result
	 */
	template < typename T, size_t Count > Point< T, Count > operator - ( int p_value, const Point< T, Count > & p_pt );
	/**
	 *\brief		Division operator
	 *\param[in]	p_value, p_pt	The points to divide
	 *\return		The addition result
	 */
	template < typename T, size_t Count > Point< T, Count > operator / ( int p_value, const Point< T, Count > & p_pt );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_value, p_pt	The points to multiply
	 *\return		The addition result
	 */
	template < typename T, size_t Count > Point< T, Count > operator * ( double p_value, const Point< T, Count > & p_pt );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_value, p_pt	The points to add
	 *\return		The addition result
	 */
	template < typename T, size_t Count > Point< T, Count > operator + ( double p_value, const Point< T, Count > & p_pt );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_value, p_pt	The points to subtract
	 *\return		The addition result
	 */
	template < typename T, size_t Count > Point< T, Count > operator - ( double p_value, const Point< T, Count > & p_pt );
	/**
	 *\brief		Division operator
	 *\param[in]	p_value, p_pt	The points to divide
	 *\return		The addition result
	 */
	template < typename T, size_t Count > Point< T, Count > operator / ( double p_value, const Point< T, Count > & p_pt );
	/**
	 *\brief		Negates every value of the point's data
	 *\param[in]	p_pt	The point
	 */
	template < typename T, size_t Count > void Reverse( Point< T, Count > const & p_pt );
	/**
	 *\brief		Normalises the point
	 *\remarks		A point is normalised when it's length equals 1.0
					<br />So each data is divided by the point's length
	 *\param[in]	p_pt	The point
	 */
	template < typename T, size_t Count > void Normalise( Point< T, Count > & p_pt );
	/**
	 *\brief		Returns the normalised form of a point
	 *\param[in]	p_pt	The point
	 *\return		The normalised point
	 */
	template < typename T, size_t Count > Point< T, Count > GetNormalised( Point< T, Count > const & p_pt );
	/**
	 *\brief		Computes the dot product of two points
	 *\param[in]	p_a, p_b	The points
	 *\return		The dot product
	 */
	template < typename T, size_t Count > T Dot( Point< T, Count > const & p_a, Point< T, Count > const & p_b );
	/**
	 *\brief		Computes the trigonometric cosine of the angle between two points
	 *\param[in]	p_a, p_b	The points
	 *\return		The cosine
	 */
	template < typename T, size_t Count > double GetCosTheta( Point< T, Count > const & p_a, Point< T, Count > const & p_b );
	/**
	 *\brief		Computes the squared Euclidian distance between the two given points
	 *\param[in]	p_a, p_b	The points
	 *\return		The squared Euclidian distance
	 */
	template < typename T, size_t Count > double GetSquaredDistance( Point< T, Count > const & p_a, Point< T, Count > const & p_b = Point< T, Count >() );
	/**
	 *\brief		Computes the Euclidian distance between the two given points
	 *\param[in]	p_a, p_b	The points
	 *\return		The Euclidian distance
	 */
	template < typename T, size_t Count > double GetDistance( Point< T, Count > const & p_a, Point< T, Count > const & p_b = Point< T, Count >() );
	/**
	 *\brief		Computes the Manhattan distance between the two given points
	 *\param[in]	p_a, p_b	The points
	 *\return		The Manhattan distance
	 */
	template < typename T, size_t Count > double GetManhattanDistance( Point< T, Count > const & p_a, Point< T, Count > const & p_b = Point< T, Count >() );
	/**
	 *\brief		Computes the Minkowski distance between the two given points
	 *\param[in]	p_a, p_b	The points
	 *\param[in]	p_order	The Minkowski order
	 *\return		The Minkowski distance
	 */
	template < typename T, size_t Count > double GetMinkowskiDistance( double p_order, Point< T, Count > const & p_a, Point< T, Count > const & p_b = Point< T, Count >() );
	/**
	 *\brief		Computes the Chebychev distance between the two given points
	 *\param[in]	p_a, p_b	The points
	 *\return		The Chebychev distance
	 */
	template < typename T, size_t Count > double GetChebychevDistance( Point< T, Count > const & p_a, Point< T, Count > const & p_b = Point< T, Count >() );
}

#include "Point.inl"

#endif
