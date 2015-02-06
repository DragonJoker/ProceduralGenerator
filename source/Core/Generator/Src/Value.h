/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_VALUE_H___
#define ___GENERATOR_VALUE_H___

#include "GeneratorPrerequisites.h"

// disable: "< type > needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251 )
// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning (disable : 4275 )

namespace ProceduralTextures
{
	namespace details
	{
		/*!
		\author		Sylvain DOREMUS
		\date		19/10/2011
		\brief		Functor used to delete an object with for_each
		*/
		template < class Object >
		struct ObjectDeleter
		{
			void operator()( Object * p_pPointer )
			{
				delete p_pPointer;
			}
		};
		/**
		 *\brief		Deletes all container's objects
		 *\param[in]	p_container	The container
		 *\param[in]	p_object	A dummy object
		 */
		template < class Container, class Object >
		void ClearContent( Container &, Object )
		{
		}
		/**
		 *\brief		Deletes all container's objects
		 *\param[in]	p_container	The container
		 *\param[in]	p_object	A dummy object
		 */
		template < class Container, class Object >
		void ClearContent( Container & p_container, Object * )
		{
			std::for_each( p_container.begin(), p_container.end(), ObjectDeleter< Object >() );
		}
		/*!
		\author		Sylvain DOREMUS
		\date		19/10/2011
		\brief		Functor used to delete a pair with for_each
		*/
		template< typename KeyType, typename ObjType >
		struct PairDeleter
		{
			void operator()( std::pair< KeyType, ObjType * > p_pair )
			{
				delete p_pair.second;
			}
		};
		/**
		 *\brief		Deletes all container's pairs
		 *\param[in]	p_container	The container
		 *\param[in]	p_object	A dummy pair
		 */
		template< class CtnrType, typename KeyType, typename ObjType >
		void ClearPairContent( CtnrType &, std::pair< KeyType, ObjType > )
		{
		}
		/**
		 *\brief		Deletes all container's pairs
		 *\param[in]	p_container	The container
		 *\param[in]	p_object	A dummy pair
		 */
		template< class CtnrType, typename KeyType, typename ObjType >
		void ClearPairContent( CtnrType & p_container, std::pair< KeyType, ObjType * > )
		{
			std::for_each( p_container.begin(), p_container.end(), PairDeleter< KeyType, ObjType >() );
		}
	}
	/**
	 *\brief		Clears a container
	 *\remark		Deallocates all the content if needed
	 *\param[in]	p_container	The container to clear
	 */
	template < class Container >
	void ClearContainer( Container & p_container )
	{
		typedef typename Container::value_type value_type;
		details::ClearContent( p_container, value_type() );
		Container().swap( p_container );
	}
	/**
	 *\brief		Clears a pair container (like std::map)
	 *\remark		Deallocates all the content if needed
	 *\param[in]	p_container	The container to clear
	 */
	template< class CtnrType >
	void ClearPairContainer( CtnrType & p_container )
	{
		typedef typename CtnrType::value_type value_type;
		details::ClearPairContent( p_container, value_type() );
		CtnrType().swap( p_container );
	}
	/*!
	\author		Sylvain DOREMUS
	\date		19/10/2011
	\brief		Struct used to select best way to put type in parameter : 'value' or 'const reference'
	*/
	template < typename T >
	struct CallTraits
	{
	private:
		template < typename U, bool Big > struct CallTraitsImpl;
		/*!
		\author		Sylvain DOREMUS
		\date		19/10/2011
		\brief		Struct used to select 'reference' parameter type
		*/
		template< typename U >
		struct CallTraitsImpl< U, true >
		{
			typedef U const & ConstType;
			typedef U & Type;
		};
		/*!
		\author		Sylvain DOREMUS
		\date		19/10/2011
		\brief		Struct used to select 'value' parameter type
		*/
		template< typename U >
		struct CallTraitsImpl< U, false >
		{
			typedef U const ConstType;
			typedef U Type;
		};

	public:
		/*!
		\author		Sylvain DOREMUS
		\date		19/10/2011
		\brief		Typedef over the best way to put type in parameter : 'value' or 'reference'
		*/
		typedef typename CallTraitsImpl < T, ( sizeof( T ) > sizeof( ptrdiff_t ) ) >::Type ParamType;
		/*!
		\author		Sylvain DOREMUS
		\date		19/10/2011
		\brief		Typedef over the best way to put type in parameter : 'const value' or 'const reference'
		*/
		typedef typename CallTraitsImpl < T, ( sizeof( T ) > sizeof( ptrdiff_t ) ) >::ConstType ConstParamType;
	};
	/**
	 *\brief		Returns absolute value of given value
	 *\param[in]	p_val	The given value
	 *\return		The absolute value
	 */
	template < typename T >
	T abs( const T & a )
	{
		return a < 0 ? -a : a;
	}
	/*!
	\author		Sylvain DOREMUS
	\date		19/10/2011
	\brief		Policy for various types
	\remark		Policy created to use various types in template classes such as Point, Matrix, and others.
				<br />Holds  operators, initialisation functions, conversion functions, ...
	*/
	template < typename T > class Policy
	{
	private:
		typedef T value_type;

	public:
		/**
		 *\return		The neutral value for the type
		 */
		static value_type zero()
		{
			return 0;
		}
		/**
		 *\return		The unit value for the type
		 */
		static value_type unit()
		{
			return 1;
		}
		/**
		 *\brief		Initialises the given variable to the neutral value
		 *\param[in]	p_a	The variable to initialise
		 */
		static void init( value_type & p_a )
		{
			p_a = zero();
		}
		/**
		 *\brief		Tests if the given param is equal to neutral value
		 *\param[in]	p_a	The value to test
		 *\return		The test result
		 */
		static bool is_null( const value_type & p_a )
		{
			return equals( p_a, zero() );
		}
		/**
		 *\param[in]	p_a	The value to negate
		 *\return		The negated param (opposite of the value, relative to the neutral value)
		 */
		static value_type negate( const value_type & p_a )
		{
			return ( is_unsigned< value_type >::value ? p_a : -p_a );
		}
		/**
		 *\param[in,out]	p_a	The value to negate, received the negated value
		 *\return			Reference to the param
		 */
		static value_type ass_negate( value_type & p_a )
		{
			return assign( p_a, negate( p_a ) );
		}
		/**
		 *\brief		If the type is a floating type, rounds it, else doesn't do anything
		 *\param[in]	p_a	The value to stick
		 */
		static value_type stick( value_type & p_a )
		{
			if ( is_floating< value_type >::value )
			{
				if ( is_null( p_a ) )
				{
					init( p_a );
				}
				else if ( p_a > 0 )
				{
					if ( equals( p_a, float( int( p_a + 0.5f ) ) ) )
					{
						assign( p_a, float( int( p_a + 0.5f ) ) );
					}
				}
				else
				{
					if ( equals( p_a, float( int( p_a - 0.5f ) ) ) )
					{
						assign( p_a, float( int( p_a - 0.5f ) ) );
					}
				}
			}
		}
		/**
		 *\brief		Converts a given param of a given type to this class template type
		 *\param[in]	p_value	The value to convert
		 *\return		The converted value (static_cast, essentially)
		 */
		template< typename Ty > static value_type convert( const Ty & p_value )
		{
			return static_cast< value_type >( p_value );
		}
		/**
		 *\brief		Tests equality between 2 params of different types. uses std::numeric_limits::epsilon to perform the test
		 *\param[in]	p_a	The first param, whose type is used as the template argument of std::numeric_limits
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before comparing
		 *\return		The compare result
		 */
		template< typename Ty > static bool equals( const value_type & p_a, const Ty & p_b )
		{
			return substract( p_a, convert< Ty >( p_b ) ) <= std::numeric_limits< value_type >::epsilon();
		}
		/**
		 *\brief		Adds two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before adding
		 *\return		The addition result
		 */
		template< typename Ty > static value_type add( const value_type & p_a, const Ty & p_b )
		{
			return p_a + convert< Ty >( p_b );
		}
		/**
		 *\brief		Substracts two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before substracting
		 *\return		The substraction result
		 */
		template< typename Ty > static value_type substract( const value_type & p_a, const Ty & p_b )
		{
			return p_a - convert< Ty >( p_b );
		}
		/**
		 *\brief		Multiplies two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before multiplying
		 *\return		The multiplication result
		 */
		template< typename Ty > static value_type multiply( const value_type & p_a, const Ty & p_b )
		{
			return p_a * convert< Ty >( p_b );
		}
		/**
		 *\brief		Divides two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before dividing
		 *\return	The division result
		 */
		template< typename Ty > static value_type divide( const value_type & p_a, const Ty & p_b )
		{
			return p_a / convert< Ty >( p_b );
		}
		/**
		 *\brief			Adds two params of different type, gives the result
		 *\remark			The first param receives the result
		 *\param[in,out]	p_a	The first param, receives the result
		 *\param[in]		p_b	The second param, converted into \p p_a 's type before adding
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type ass_add( value_type & p_a, const Ty & p_b )
		{
			return assign( p_a, add< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Substracts two params of different type, gives the result
		 *\remark			The first param receives the result
		 *\param[in,out]	p_a	The first param, receives the result
		 *\param[in]		p_b	The second param, converted into \p p_a 's type before substracting
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type ass_substract( value_type & p_a, const Ty & p_b )
		{
			return assign( p_a, substract< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Multiplies two params of different type, gives the result
		 *\remark			The first param receives the result
		 *\param[in,out]	p_a	The first param, receives the result
		 *\param[in]		p_b	The second param, converted into \p p_a 's type before multiplying
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type ass_multiply( value_type & p_a, const Ty & p_b )
		{
			return assign( p_a, multiply< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Divides two params of different type, gives the result
		 *\remark			The first param receives the result
		 *\param[in,out]	p_a	The first param, receives the result
		 *\param[in]		p_b	The second param, converted into \p p_a 's type before dividing
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type ass_divide( value_type & p_a, const Ty & p_b )
		{
			return assign( p_a, divide< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Converts the second param and assigns the result to the first param
		 *\param[in,out]	p_a	The first param, receives the converted second param
		 *\param[in]		p_b	The second param
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type assign( value_type & p_a, const Ty & p_b )
		{
			return p_a = convert< Ty >( p_b );
		}
	};
	/*!
	\author		Sylvain DOREMUS
	\date		19/10/2011
	\brief		'bool' Policy class specialisation
	*/
	template <> class Policy< bool >
	{
	private:
		typedef bool value_type;

	public:
		/**
		 *\return		The neutral value for the type
		 */
		static value_type zero()
		{
			return false;
		}
		/**
		 *\return		The unit value for the type
		 */
		static value_type unit()
		{
			return true;
		}
		/**
		 *\brief		Initialises the given variable to the neutral value
		 *\param[in]	p_a	The variable to initialise
		 */
		static void init( value_type & p_a )
		{
			p_a = zero();
		}
		/**
		 *\brief		Tests if the given param is equal to neutral value
		 *\param[in]	p_a	The value to test
		 *\return		The test result
		 */
		static bool is_null( const value_type & p_a )
		{
			return equals( p_a, zero() );
		}
		/**
		 *\param[in]	p_a	The value to negate
		 *\return		The negated param (opposite of the value, relative to the neutral value)
		 */
		static value_type negate( const value_type & p_a )
		{
			return ! p_a;
		}
		/**
		 *\param[in,out]	p_a	The value to negate, received the negated value
		 *\return			Reference to the param
		 */
		static value_type ass_negate( value_type & p_a )
		{
			return assign( p_a, negate( p_a ) );
		}
		/**
		 *\brief		If the type is a floating type, rounds it, else doesn't do anything
		 *\param[in]	p_a	The value to stick
		 */
		static value_type stick( value_type & p_a )
		{
			return p_a;
		}
		/**
		 *\brief		Converts a given param of a given type to this class template type
		 *\param[in]	p_value	The value to convert
		 *\return		The converted value (static_cast, essentially)
		 */
		template< typename Ty > static value_type convert( const Ty & p_value )
		{
			return p_value != 0;
		}
		/**
		 *\brief		Tests equality between 2 params of different types. uses std::numeric_limits::epsilon to perform the test
		 *\param[in]	p_a	The first param, whose type is used as the template argument of std::numeric_limits
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before comparing
		 *\return		The compare result
		 */
		template< typename Ty > static bool equals( const value_type & p_a, const Ty & p_b )
		{
			return p_a == convert< Ty >( p_b );
		}
		/**
		 *\brief		Adds two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before adding
		 *\return		The addition result
		 */
		template< typename Ty > static value_type add( const value_type & p_a, const Ty & p_b )
		{
			return p_a || convert< Ty >( p_b );
		}
		/**
		 *\brief		Substracts two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before substracting
		 *\return		The substraction result
		 */
		template< typename Ty > static value_type substract( const value_type & p_a, const Ty & p_b )
		{
			return p_a || convert< Ty >( p_b );
		}
		/**
		 *\brief		Multiplies two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before multiplying
		 *\return		The multiplication result
		 */
		template< typename Ty > static value_type multiply( const value_type & p_a, const Ty & p_b )
		{
			return p_a && convert< Ty >( p_b );
		}
		/**
		 *\brief		Divides two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before dividing
		 *\return		The division result
		 */
		template< typename Ty > static value_type divide( const value_type & p_a, const Ty & p_b )
		{
			return p_a && convert< Ty >( p_b );
		}
		/**
		 *\brief			Adds two params of different type, gives the result
		 *\remark			The first param receives the result
		 *\param[in,out]	p_a	The first param, receives the result
		 *\param[in]		p_b	The second param, converted into \p p_a 's type before adding
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type ass_add( value_type & p_a, const Ty & p_b )
		{
			return assign( p_a, add< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Substracts two params of different type, gives the result
		 *\remark			The first param receives the result
		 *\param[in,out]	p_a	The first param, receives the result
		 *\param[in]		p_b	The second param, converted into \p p_a 's type before substracting
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type ass_substract( value_type & p_a, const Ty & p_b )
		{
			return assign( p_a, substract< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Multiplies two params of different type, gives the result
		 *\remark			The first param receives the result
		 *\param[in,out]	p_a	The first param, receives the result
		 *\param[in]		p_b	The second param, converted into \p p_a 's type before multiplying
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type ass_multiply( value_type & p_a, const Ty & p_b )
		{
			return assign( p_a, multiply< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Divides two params of different type, gives the result
		 *\remark			The first param receives the result
		 *\param[in,out]	p_a	The first param, receives the result
		 *\param[in]		p_b	The second param, converted into \p p_a 's type before dividing
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type ass_divide( value_type & p_a, const Ty & p_b )
		{
			return assign( p_a, divide< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Converts the second param and assigns the result to the first param
		 *\param[in,out]	p_a	The first param, receives the converted second param
		 *\param[in]		p_b	The second param
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type assign( value_type & p_a, const Ty & p_b )
		{
			return p_a = convert< Ty >( p_b );
		}
	};
}

#endif
