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
#ifndef ___GENERATOR_SIZE_H___
#define ___GENERATOR_SIZE_H___

#include "Point.h"

#if HAS_DELEGATING_CONSTRUCTORS
#	define DELEGATING_SIZE( x, y ) Size( x, y )
#else
#	define DELEGATING_SIZE( x, y ) m_x( uint32_t( x ) ), m_y( uint32_t( y ) )
#endif
namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		16/02/2015
	\brief		Size representation
	*/
	class Size
	{
	public:
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_a	The first value
		 *\param[in]	p_b	The second value
		 */
		template< typename U, typename V >
		Size( U const & p_a, V const & p_b )
			: m_x( uint32_t( p_a ) )
			, m_y( uint32_t( p_b ) )
		{
		}
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_pt	The value
		 */
		template< typename T >
		Size( Point< T, 2 > const & p_pt )
			: DELEGATING_SIZE( p_pt[0], p_pt[1] )
		{
		}
		/**
		 *\brief		Default constructor
		 */
		Size()
			: DELEGATING_SIZE( 0, 0 )
		{
		}
		/**
		 *\brief		Destructor
		 */
		~Size()
		{
		}
		/**
		 *\brief		Retrieves the x dimension
		 *\return		A constant reference on the dimension
		 */
		inline uint32_t const & x()const
		{
			return m_x;
		}
		/**
		 *\brief		Retrieves the x dimension
		 *\return		A reference on the dimension
		 */
		inline uint32_t & x()
		{
			return m_x;
		}
		/**
		 *\brief		Retrieves the y dimension
		 *\return		A constant reference on the dimension
		 */
		inline uint32_t const & y()const
		{
			return m_y;
		}
		/**
		 *\brief		Retrieves the y dimension
		 *\return		A reference on the dimension
		 */
		inline uint32_t & y()
		{
			return m_y;
		}
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_size	The object to add
		 *\return		A reference to this object
		 */
		inline Size & operator += ( const Size & p_size )
		{
			m_x += p_size.m_x;
			m_y += p_size.m_y;
			return *this;
		}
		/**
		 *\brief		Subtraction assignment operator
		 *\param[in]	p_size	The object to subtract
		 *\return		A reference to this object
		 */
		inline Size & operator -= ( const Size & p_size )
		{
			m_x -= p_size.m_x;
			m_y -= p_size.m_y;
			return *this;
		}
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_coord	The value to add
		 *\return		A reference to this object
		 */
		template < typename U > Size & operator += ( U const & p_coord )
		{
			m_x += uint32_t( p_coord );
			m_y += uint32_t( p_coord );
			return *this;
		}
		/**
		 *\brief		Subtraction assignment operator
		 *\param[in]	p_coord	The value to subtract
		 *\return		A reference to this object
		 */
		template < typename U > Size & operator -= ( U const & p_coord )
		{
			m_x -= uint32_t( p_coord );
			m_y -= uint32_t( p_coord );
			return *this;
		}
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_coord	The value to multiply
		 *\return		A reference to this object
		 */
		template < typename U > Size & operator *= ( U const & p_coord )
		{
			m_x = uint32_t( m_x * p_coord );
			m_y = uint32_t( m_y * p_coord );
			return *this;
		}
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_coord	The value to divide
		 *\return		A reference to this object
		 */
		template < typename U > Size & operator /= ( U const & p_coord )
		{
			if ( p_coord != 0 )
			{
				m_x = uint32_t( m_x / p_coord );
				m_y = uint32_t( m_y / p_coord );
			}

			return *this;
		}

	private:
		//! The x dimension
		uint32_t m_x;
		//! The y dimension
		uint32_t m_y;
	};
	/**
	 *\brief		Equality operator
	 *\param[in]	p_a, p_b	The values to compare
	 *\return		\p true if values have same dimensions and same values
	 */
	inline bool operator == ( const Size & p_a, const Size & p_b )
	{
		return p_a.x() == p_b.x() && p_a.y() == p_b.y();
	}
	/**
	 *\brief		Difference operator
	 *\param[in]	p_a, p_b	The values to compare
	 *\return		\p true if values have different dimensions or At least one different value
	 */
	inline bool operator != ( const Size & p_a, const Size & p_b )
	{
		return p_a.x() != p_b.x() || p_a.y() != p_b.y();
	}
	/**
	 *\brief		Addition operator
	 *\param[in]	p_a, p_b	The values to add
	 *\return		The result
	 */
	inline Size operator + ( const Size & p_a, const Size & p_b )
	{
		Size l_tmp( p_a );
		l_tmp += p_b;
		return l_tmp;
	}
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_a, p_b	The values to subtract
	 *\return		The result
	 */
	inline Size operator - ( const Size & p_a, const Size & p_b )
	{
		Size l_tmp( p_a );
		l_tmp -= p_b;
		return l_tmp;
	}
	/**
	 *\brief		Addition operator
	 *\param[in]	p_size, p_coord	The values to add
	 *\return		The result
	 */
	template< typename U > Size operator + ( const Size & p_size, U const & p_coord )
	{
		Size l_tmp( p_size );
		l_tmp += p_coord;
		return l_tmp;
	}
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_size, p_coord	The values to subtract
	 *\return		The result
	 */
	template< typename U > Size operator - ( const Size & p_size, U const & p_coord )
	{
		Size l_tmp( p_size );
		l_tmp -= p_coord;
		return l_tmp;
	}
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_size, p_coord	The values to multiply
	 *\return		The result
	 */
	template< typename U > Size operator * ( const Size & p_size, U const & p_coord )
	{
		Size l_tmp( p_size );
		l_tmp *= p_coord;
		return l_tmp;
	}
	/**
	 *\brief		Division operator
	 *\param[in]	p_size, p_coord	The values to divide
	 *\return		The result
	 */
	template< typename U > Size operator / ( const Size & p_size, U const & p_coord )
	{
		Size l_tmp( p_size );
		l_tmp /= p_coord;
		return l_tmp;
	}
	/**
	 *\brief		Addition operator
	 *\param[in]	p_value, p_size	The values to add
	 *\return		The result
	 */
	inline Size operator + ( int p_value, const Size & p_size )
	{
		Size l_tmp( p_size );
		l_tmp += p_value;
		return l_tmp;
	}
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_value, p_size	The values to subtract
	 *\return		The result
	 */
	inline Size operator - ( int p_value, const Size & p_size )
	{
		Size l_tmp( p_size );
		l_tmp -= p_value;
		return l_tmp;
	}
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_value, p_size	The values to multiply
	 *\return		The result
	 */
	inline Size operator * ( int p_value, const Size & p_size )
	{
		Size l_tmp( p_size );
		l_tmp *= p_value;
		return l_tmp;
	}
	/**
	 *\brief		Addition operator
	 *\param[in]	p_value, p_size	The values to add
	 *\return		The result
	 */
	inline Size operator + ( double p_value, const Size & p_size )
	{
		Size l_tmp( p_size );
		l_tmp += p_value;
		return l_tmp;
	}
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_value, p_size	The values to subtract
	 *\return		The result
	 */
	inline Size operator - ( double p_value, const Size & p_size )
	{
		Size l_tmp( p_size );
		l_tmp -= p_value;
		return l_tmp;
	}
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_value, p_size	The values to multiply
	 *\return		The result
	 */
	inline Size operator * ( double p_value, const Size & p_size )
	{
		Size l_tmp( p_size );
		l_tmp *= p_value;
		return l_tmp;
	}
}

#endif
