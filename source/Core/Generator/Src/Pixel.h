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
#ifndef ___GENERATOR_PIXEL_H___
#define ___GENERATOR_PIXEL_H___

#include "Point.h"

#define PG_UINT8_RGBA				uint8_t, 3, 1, false, false
#define PG_UINT8_RGB				uint8_t, 3, 0, false, false
#define PG_UINT8_RGBA_MIRROR		uint8_t, 3, 1, true, false
#define PG_UINT8_RGB_MIRROR			uint8_t, 3, 0, true, false
#define PG_UINT8_RGB_FLIP			uint8_t, 3, 0, false, true
#define PG_UINT8_RGBA_FLIP			uint8_t, 3, 1, false, true
#define PG_UINT8_RGB_MIRROR_FLIP	uint8_t, 3, 0, true, true
#define PG_UINT8_RGBA_MIRROR_FLIP	uint8_t, 3, 1, true, true

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@version
		0.6.1.0
	@date
		29/08/2011
	@brief
		Pixel definition
	@remarks
		Beware not putting any virtuals here, this is memcopi-ed and memset-ed
	*/
	template< typename T >
	class Pixel
	{
	public:
		/** Default constructor
		 */
		Pixel();

		/** Specified constructor
		@param[in] p_r,
			p_g, p_b, p_a	The pixel components
		 */
		template< typename U > Pixel( U p_r, U p_g, U p_b, U p_a );

		/** Constructor from another pixel type
		@param[in] p_pxl
			The object to copy
		 */
		template< typename U > Pixel( const Pixel< U > & p_pxl );

		/** Assignment operator from another pixel type
		@param[in] p_px
			The object to copy
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator =( const Pixel< U > & p_px );

		/** Addition assignment operator
		@param[in] p_px
			The object to add
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator +=( const Pixel< U > & p_px );

		/** Substraction assignment operator
		@param[in] p_px
			The object to subtract
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator -=( const Pixel< U > & p_px );

		/** Multiplication assignment operator
		@param[in] p_px
			The object to multiply
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator *=( const Pixel< U > & p_px );

		/** Division assignment operator
		@param[in] p_px
			The object to divide
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator /=( const Pixel< U > & p_px );

		/** Addition assignment operator
		@param[in] p_t
			The value to add
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator +=( U const & p_t );

		/** Substraction assignment operator
		@param[in] p_t
			The value to subtract
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator -=( U const & p_t );

		/** Multiplication assignment operator
		@param[in] p_t
			The value to multiply
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator *=( U const & p_t );

		/** Division assignment operator
		@param[in] p_t
			The value to divide
		@return
			A reference to this object
		 */
		template< typename U > Pixel & operator /=( U const & p_t );

		/** Binary right decal assignment operator
		@param[in] p_t
			The decal value
		@return
			A reference to this object
		 */
		Pixel & operator >>= ( size_t p_t );

		/** Binary left decal assignment operator
		@param[in] p_t
			The decal value
		@return
			A reference to this object
		 */
		Pixel & operator <<= ( size_t p_t );

		/** Sets the pixel components
		@param[in] p_r, p_g, p_b, p_a
			The pixel components
		 */
		template< typename U > void Set( U p_r, U p_g, U p_b, U p_a );

		/** Sets the pixel from another pixel type
		@param[in] p_px
			The object to copy
		 */
		template< typename U > void Set( const Pixel< U > & p_px );

		/** Sets the pixel from two buffers
		@remarks
			One of the buffers may contain colour and the other may contain alpha
			<br />Count1 and Count2 may be 0, 1, 3 or 4.
			<br />If one Count equals 3 (RGB compponents for respective p_pBuffer), the other Count must be 0 or 1 (Alpha component for respective p_pBuffer)
			<br />If one Count equals 4 (RGBA compponents for respective p_pBuffer), the other Count must be 0 (Alpha component for respective p_pBuffer)
		@param[in] p_pBuffer1,
			p_pBuffer2	The buffers
		 */
		template < size_t Count1, size_t Count2 > void Set( const T * p_pBuffer1, const T * p_pBuffer2 );

		/** Retrieves the sum of the RGB components
		@return
			The sum
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
	/** Equality operator
	@param[in] p_pixel, p_px
		The pixels to compare
	@return
		\p true if pixels have same values
	 */
	template< typename T, typename U > bool operator ==( const Pixel< T > & p_pixel, const Pixel< U > & p_px );

	/** Difference operator
	@param[in] p_pixel, p_px
		The pixels to compare
	@return
		\p true if pixels have At least one different value
	 */
	template< typename T, typename U > bool operator !=( const Pixel< T > & p_pixel, const Pixel< U > & p_px );

	/** Addition operator
	@param[in] p_pixel, p_px
		The pixels to add
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator +( const Pixel< T > & p_pixel, const Pixel< U > & p_px );

	/** Subtraction operator
	@param[in] p_pixel, p_px
		The pixels to subtract
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator -( const Pixel< T > & p_pixel, const Pixel< U > & p_px );

	/** Multiplication operator
	@param[in] p_pixel, p_px
		The pixels to multiply
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator /( const Pixel< T > & p_pixel, const Pixel< U > & p_px );

	/** Division operator
	@param[in] p_pixel, p_px
		The pixels to divide
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator *( const Pixel< T > & p_pixel, const Pixel< U > & p_px );

	/** Addition operator
	@param[in] p_pixel, p_t
		The pixels to add
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator +( const Pixel< T > & p_pixel, U const & p_t );

	/** Subtraction operator
	@param[in] p_pixel, p_t
		The pixels to subtract
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator -( const Pixel< T > & p_pixel, U const & p_t );

	/** Multiplication operator
	@param[in] p_pixel, p_t
		The pixels to multiply
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator /( const Pixel< T > & p_pixel, U const & p_t );

	/** Division operator
	@param[in] p_pixel, p_t
		The pixels to divide
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< T > operator *( const Pixel< T > & p_pixel, U const & p_t );

	/** Addition operator
	@param[in] p_pixel, p_t
		The pixels to add
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< U > operator +( U const & p_t, const Pixel< T > & p_pixel );

	/** Subtraction operator
	@param[in] p_pixel, p_t
		The pixels to subtract
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< U > operator -( U const & p_t, const Pixel< T > & p_pixel );

	/** Multiplication operator
	@param[in] p_pixel, p_t
		The pixels to multiply
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< U > operator /( U const & p_t, const Pixel< T > & p_pixel );

	/** Division operator
	@param[in] p_pixel, p_t
		The pixels to divide
	@return
		The addition result
	 */
	template< typename T, typename U > Pixel< U > operator *( U const & p_t, const Pixel< T > & p_pixel );
}

#include "Pixel.inl"

#endif
