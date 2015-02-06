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
#ifndef ___GENERATOR_PIXEL_H___
#define ___GENERATOR_PIXEL_H___

#include "Point.h"

namespace ProceduralTextures
{
#define PG_UINT8_RGBA				uint8_t, 3, 1, false, false
#define PG_UINT8_RGB				uint8_t, 3, 0, false, false
#define PG_UINT8_RGBA_MIRROR		uint8_t, 3, 1, true, false
#define PG_UINT8_RGB_MIRROR			uint8_t, 3, 0, true, false
#define PG_UINT8_RGB_FLIP			uint8_t, 3, 0, false, true
#define PG_UINT8_RGBA_FLIP			uint8_t, 3, 1, false, true
#define PG_UINT8_RGB_MIRROR_FLIP	uint8_t, 3, 0, true, true
#define PG_UINT8_RGBA_MIRROR_FLIP	uint8_t, 3, 1, true, true
	/*!
	\author		Sylvain DOREMUS
	\version	0.6.1.0
	\date		29/08/2011
	\brief		Pixel definition
	\remark		Beware not putting any virtuals here, this is memcopi-ed and memset-ed
	*/
	template< typename T >
	class Pixel
	{
	public:
		/**
		 *\brief		Default constructor
		 */
		Pixel();
		/**
		 *\brief		Specified constructor
		 *\param[in]	p_r, p_g, p_b, p_a	The pixel components
		 */
		template< typename U > Pixel( U p_r, U p_g, U p_b, U p_a );
		/**
		 *\brief		Constructor from another pixel type
		 *\param[in]	p_ptPoint	The object to copy
		 */
		template< typename U > Pixel( const Pixel< U > & p_pxl );
		/**
		 *\~english
		 *\brief		Assignment operator from another pixel type
		 *\param[in]	p_pt	The object to copy
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator =( const Pixel< U > & p_px );
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_pt	The object to add
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator +=( const Pixel< U > & p_px );
		/**
		 *\brief		Substraction assignment operator
		 *\param[in]	p_pt	The object to substract
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator -=( const Pixel< U > & p_px );
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_pt	The object to multiply
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator *=( const Pixel< U > & p_px );
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_pt	The object to divide
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator /=( const Pixel< U > & p_px );
		/**
		 *\brief		Addition assignment operator
		 *\param[in]	p_t	The value to add
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator +=( const U & p_t );
		/**
		 *\brief		Substraction assignment operator
		 *\param[in]	p_coord	The value to substract
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator -=( const U & p_t );
		/**
		 *\brief		Multiplication assignment operator
		 *\param[in]	p_coord	The value to multiply
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator *=( const U & p_t );
		/**
		 *\brief		Division assignment operator
		 *\param[in]	p_coord	The value to divide
		 *\return		A reference to this object
		 */
		template< typename U > Pixel & operator /=( const U & p_t );
		/**
		 *\brief		Binary right decal assignment operator
		 *\param[in]	p_coord	The value to divide
		 *\return		A reference to this object
		 */
		Pixel & operator >>= ( size_t p_t );
		/**
		 *\brief		Binary left decal assignment operator
		 *\param[in]	p_coord	The value to divide
		 *\return		A reference to this object
		 */
		Pixel & operator <<= ( size_t p_t );
		/**
		 *\brief		Sets the pixel components
		 *\param[in]	p_r, p_g, p_b, p_a	The pixel components
		 */
		template< typename U > void Set( U p_r, U p_g, U p_b, U p_a );
		/**
		 *\~english
		 *\brief		Sets the pixel from another pixel type
		 *\param[in]	p_pt	The object to copy
		 */
		template< typename U > void Set( const Pixel< U > & p_px );
		/**
		 *\~english
		 *\brief		Sets the pixel from two buffers
		 *\remarks		One of the buffers may contain colour and the other may contain alpha
		 *\remarks		Count1 and Count2 may be 0, 1, 3 or 4.
		 *\remarks		If one Count equals 3 (RGB compponents for respective p_pBuffer), the other Count must be 0 or 1 (Alpha component for respective p_pBuffer)
		 *\remarks		If one Count equals 4 (RGBA compponents for respective p_pBuffer), the other Count must be 0 (Alpha component for respective p_pBuffer)
		 *\param[in]	p_pBuffer1, p_pBuffer2	The buffers
		 */
		template < size_t Count1, size_t Count2 > void Set( const T * p_pBuffer1, const T * p_pBuffer2 );
		/**
		 *\brief		Retrieves the sum of the RGB components
		 *\return		The sum
		 */
		template< typename U > U Sum()const;

	public:
		//! Red component
		T r;
		//! Green component
		T g;
		//! Blue component
		T b;
		//! Alpha component
		T a;
	};
	/**
	 *\brief		Equality operator
	 *\param[in]	p_pixel, p_px	The pixels to compare
	 *\return		\p true if pixels have same values
	 */
	template< typename T, typename U > bool operator ==( const Pixel< T > & p_pixel, const Pixel< U > & p_px );
	/**
	 *\brief		Difference operator
	 *\param[in]	p_pixel, p_px	The pixels to compare
	 *\return		\p true if pixels have at least one different value
	 */
	template< typename T, typename U > bool operator !=( const Pixel< T > & p_pixel, const Pixel< U > & p_px );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_pixel, p_px	The pixels to add
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator +( const Pixel< T > & p_pixel, const Pixel< U > & p_px );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_pixel, p_px	The pixels to subtract
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator -( const Pixel< T > & p_pixel, const Pixel< U > & p_px );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_pixel, p_px	The pixels to multiply
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator /( const Pixel< T > & p_pixel, const Pixel< U > & p_px );
	/**
	 *\brief		Division operator
	 *\param[in]	p_pixel, p_px	The pixels to divide
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator *( const Pixel< T > & p_pixel, const Pixel< U > & p_px );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_pixel, p_t	The pixels to add
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator +( const Pixel< T > & p_pixel, const U & p_t );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_pixel, p_t	The pixels to subtract
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator -( const Pixel< T > & p_pixel, const U & p_t );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_pixel, p_t	The pixels to multiply
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator /( const Pixel< T > & p_pixel, const U & p_t );
	/**
	 *\brief		Division operator
	 *\param[in]	p_pixel, p_t	The pixels to divide
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator *( const Pixel< T > & p_pixel, const U & p_t );
	/**
	 *\brief		Addition operator
	 *\param[in]	p_pixel, p_t	The pixels to add
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< U > operator +( const U & p_t, const Pixel< T > & p_pixel );
	/**
	 *\brief		Subtraction operator
	 *\param[in]	p_pixel, p_t	The pixels to subtract
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< U > operator -( const U & p_t, const Pixel< T > & p_pixel );
	/**
	 *\brief		Multiplication operator
	 *\param[in]	p_pixel, p_t	The pixels to multiply
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< U > operator /( const U & p_t, const Pixel< T > & p_pixel );
	/**
	 *\brief		Division operator
	 *\param[in]	p_pixel, p_t	The pixels to divide
	 *\return		The addition result
	 */
	template< typename T, typename U > Pixel< U > operator *( const U & p_t, const Pixel< T > & p_pixel );
    
	/*!
	\author		Sylvain DOREMUS
	\date		19/10/2011
	\brief		'UbPixel' Policy class specialisation
	*/
	template <> class Policy< UbPixel >
	{
	private:
		typedef UbPixel value_type;

	public:
		/**
		 *\return		The neutral value for the type
		 */
		static value_type zero()
		{
			return value_type();
		}
		/**
		 *\return		The unit value for the type
		 */
		static value_type unit()
		{
			return value_type( 1, 1, 1, 1 );
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
			return value_type( -p_a.r, -p_a.g, -p_a.b, -p_a.a );
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
		template< typename Ty > static value_type convert( const	Ty & p_value )
		{
			return value_type( p_value );
		}
		/**
		 *\brief		Tests equality between 2 params of different types. uses std::numeric_limits::epsilon to perform the test
		 *\param[in]	p_a	The first param, whose type is used as the template argument of std::numeric_limits
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before comparing
		 *\return		The compare result
		 */
		template< typename Ty > static bool equals( const value_type	& WXUNUSED( p_a ), const Ty & WXUNUSED( p_b ) )
		{
			return false;
		}
		/**
		 *\brief		Adds two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before adding
		 *\return		The addition result
		 */
		template< typename Ty > static value_type add( const	value_type	& p_a,	const Ty	& p_b )
		{
			return p_a + convert< Ty >( p_b );
		}
		/**
		 *\brief		Substracts two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before substracting
		 *\return		The substraction result
		 */
		template< typename Ty > static value_type	substract( const	value_type	& p_a,	const Ty	& p_b )
		{
			return p_a - convert< Ty >( p_b );
		}
		/**
		 *\brief		Multiplies two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before multiplying
		 *\return		The multiplication result
		 */
		template< typename Ty > static value_type	multiply( const	value_type	& p_a,	const Ty	& p_b )
		{
			return p_a * convert< Ty >( p_b );
		}
		/**
		 *\brief		Divides two params of different type, gives the result
		 *\param[in]	p_a	The first param
		 *\param[in]	p_b	The second param, converted into \p p_a 's type before dividing
		 *\return		The division result
		 */
		template< typename Ty > static value_type	divide( const	value_type	& p_a,	const Ty	& p_b )
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
		template< typename Ty > static value_type	ass_add(	value_type	& p_a,	const Ty	& p_b )
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
		template< typename Ty > static value_type	ass_substract(	value_type	& p_a,	const Ty	& p_b )
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
		template< typename Ty > static value_type	ass_multiply(	value_type	& p_a,	const Ty	& p_b )
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
		template< typename Ty > static value_type	ass_divide(	value_type	& p_a,	const Ty	& p_b )
		{
			return assign( p_a, divide< Ty >( p_a, p_b ) );
		}
		/**
		 *\brief			Converts the second param and assigns the result to the first param
		 *\param[in,out]	p_a	The first param, receives the converted second param
		 *\param[in]		p_b	The second param
		 *\return			Reference to the first param
		 */
		template< typename Ty > static value_type	assign(	value_type	& p_a,	const Ty	& p_b )
		{
			return p_a = convert< Ty >( p_b );
		}
	};
}

#include "Pixel.inl"

#endif
