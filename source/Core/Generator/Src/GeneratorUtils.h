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
#ifndef ___GENERATOR_UTILS_H___
#define ___GENERATOR_UTILS_H___

#include "GeneratorPrerequisites.h"
#include "Pixel.h"

namespace ProceduralTextures
{
	namespace Utils
	{
		/** Returns the size of a static C-Style array
		@param[in] p_data
			The array
		@return
			The size
		*/
		template< typename T, size_t N >
		inline size_t GetCountOf( T const( & p_data )[N] )
		{
			return N;
		}

		/** Puts a value between two bounds
		@param[in] p_min
			The min bound
		@param[in] p_max
			The max bound
		@param[in] p_value
			The value to clamp
		@return
			The clamped value
		*/
		template< typename T > inline T Clamp( T const & p_min, T const & p_max, T const & p_value )
		{
			return std::max( p_min, std::min( p_max, p_value ) );
		}

		/** Substract a value from another one, the result is At least the given minimum value
		@param[in] p_a
			The left operand
		@param[in] p_b
			The right operand
		@param[in] p_min
			The minimum value
		@return
			The result
		*/
		template< typename T, typename U, typename V > inline V Subtract( T const & p_a, U const & p_b, V const & p_min )
		{
			V l_vReturn;

			if ( p_a >= p_min + p_b )
			{
				l_vReturn = V( p_a - p_b );
			}
			else
			{
				l_vReturn = p_min;
			}

			return l_vReturn;
		}

		/** Substracts 2 pixels given a minimal pixel value for the result
		@param[in] p_a
			The first pixel
		@param[in] p_b
			The second pixel
		@param[in] p_min
			The minimal pixel
		@return
			(\p p_a - \p p_b ) with each component of the result >= to the same component from \p p_min
		*/
		template<> inline UbPixel Subtract< UbPixel, UiPixel, UbPixel >( UbPixel const & p_a, UiPixel const & p_b, UbPixel const & p_min )
		{
			UbPixel l_result;
			l_result.r = Subtract( p_a.r, p_b.r, p_min.r );
			l_result.g = Subtract( p_a.g, p_b.g, p_min.g );
			l_result.b = Subtract( p_a.b, p_b.b, p_min.b );
			l_result.a = Subtract( p_a.a, p_b.a, p_min.a );
			return l_result;
		}

		/** Substracts 2 pixels given a minimal pixel value for the result
		@param[in] p_a
			The first pixel
		@param[in] p_b
			The second pixel
		@param[in] p_min
			The minimal pixel
		@return
			(\p p_a - \p p_b ) with each component of the result >= to the same component from \p p_min
		*/
		template<> inline UbPixel Subtract< UiPixel, UiPixel, UbPixel >( UiPixel const & p_a, UiPixel const & p_b, UbPixel const & p_min )
		{
			UbPixel l_result;
			l_result.r = Subtract( p_a.r, p_b.r, p_min.r );
			l_result.g = Subtract( p_a.g, p_b.g, p_min.g );
			l_result.b = Subtract( p_a.b, p_b.b, p_min.b );
			l_result.a = Subtract( p_a.a, p_b.a, p_min.a );
			return l_result;
		}

		/** Adds a value to another one, the result is At most the given maximum value
		@param[in] p_a
			The left operand
		@param[in] p_b
			The right operand
		@param[in] p_max
			The maximum value
		@return
			The result
		*/
		template< typename T, typename U, typename V > inline V Add( T const & p_a, U const & p_b, V const & p_max )
		{
			V l_vReturn;

			if ( p_a + p_b < p_max )
			{
				l_vReturn = V( p_a + p_b );
			}
			else
			{
				l_vReturn = p_max;
			}

			return l_vReturn;
		}

		/** Adds 2 pixels given a maximal pixel value for the result
		@param[in] p_a
			The first pixel
		@param[in] p_b
			The second pixel
		@param[in] p_max
			The maximal pixel
		@return
			(\p p_a + \p p_b ) with each component of the result <= to the same component from \p p_max
		*/
		template<> inline UbPixel Add< UbPixel, UiPixel, UbPixel >( UbPixel const & p_a, UiPixel const & p_b, UbPixel const & p_max )
		{
			UbPixel l_result;
			l_result.r = Add( p_a.r, p_b.r, p_max.r );
			l_result.g = Add( p_a.g, p_b.g, p_max.g );
			l_result.b = Add( p_a.b, p_b.b, p_max.b );
			l_result.a = Add( p_a.a, p_b.a, p_max.a );
			return l_result;
		}

		/** Adds 2 pixels given a maximal pixel value for the result
		@param[in] p_a
			The first pixel
		@param[in] p_b
			The second pixel
		@param[in] p_max
			The maximal pixel
		@return
			(\p p_a + \p p_b ) with each component of the result <= to the same component from \p p_max
		*/
		template<> inline UbPixel Add< UiPixel, UiPixel, UbPixel >( UiPixel const & p_a, UiPixel const & p_b, UbPixel const & p_max )
		{
			UbPixel l_result;
			l_result.r = Add( p_a.r, p_b.r, p_max.r );
			l_result.g = Add( p_a.g, p_b.g, p_max.g );
			l_result.b = Add( p_a.b, p_b.b, p_max.b );
			l_result.a = Add( p_a.a, p_b.a, p_max.a );
			return l_result;
		}

		/** Computes the pixel components value from an unsigned long value
		@param[in] p_colour
			The colour
		@return
			The pixel
		*/
		inline UbPixel FromRGBColour( uint32_t p_colour )
		{
			UbPixel l_result;
			l_result.r = ( p_colour & 0x00FF0000 ) >> 16;
			l_result.g = ( p_colour & 0x0000FF00 ) >>  8;
			l_result.b = ( p_colour & 0x000000FF ) >>  0;
			l_result.a = 255;
			return l_result;
		}

		/** Computes the pixel components value from an unsigned long value
		@param[in] p_colour
			The colour
		@return
			The pixel
		*/
		inline UbPixel FromBGRColour( uint32_t p_colour )
		{
			UbPixel l_result;
			l_result.b = ( p_colour & 0x00FF0000 ) >> 16;
			l_result.g = ( p_colour & 0x0000FF00 ) >>  8;
			l_result.r = ( p_colour & 0x000000FF ) >>  0;
			l_result.a = 255;
			return l_result;
		}
	}

	namespace System
	{
		/** Retrieves the given screen size
		@param[in] p_screen
			The screen index
		@param[out] p_size
			Receives the screen size
		@return
			\p true if the size was retrieved
		*/
		bool GeneratorAPI GetScreenSize( uint32_t p_screen, Size & p_size );

		/** Counts available CPUs
		@return
			The CPU count
		*/
		int GeneratorAPI GetCPUCount();

		/** Retrieves the executable directory
		@return
			The directory
		*/
		String GeneratorAPI GetExecutableDirectory();

		/** Retrieves the data directory
		@return
			The directory
		*/
		String GeneratorAPI GetDataDirectory();
	}
}

#endif
