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
#ifndef ___GENERATOR_UTILS_H___
#define ___GENERATOR_UTILS_H___

#include "GeneratorPrerequisites.h"
#include "Pixel.h"

namespace ProceduralTextures
{
	namespace utils
	{
		/**
		 *\brief		Puts a value between two bounds
		 *\param[in]	p_min		The min bound
		 *\param[in]	p_max		The max bound
		 *\param[in]	p_value		The value to clamp
		 *\return		The clamped value
		 */
		template < typename T > inline T clamp( const T & p_min, const T & p_max, const T & p_value )
		{
			return std::max( p_min, std::min( p_max, p_value ) );
		}
		/**
		 *\brief		Substract a value from another one, the result is at least the given minimum value
		 *\param[in]	p_a		The left operand
		 *\param[in]	p_b		The right operand
		 *\param[in]	p_min	The minimum value
		 *\return		The result
		 */
		template< typename T, typename U, typename V > inline V substract( T const & p_a, U const & p_b, V const & p_min )
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
		/**
		 *\brief		Substracts 2 pixels given a minimal pixel value for the result
		 *\param[in]	p_a		The first pixel
		 *\param[in]	p_b		The second pixel
		 *\param[in]	p_min	The minimal pixel
		 *\return		(\p p_a - \p p_b ) with each component of the result >= to the same component from \p p_min
		 */
		template<> inline UbPixel substract< UbPixel, UiPixel, UbPixel >( const UbPixel & p_a, const UiPixel & p_b, const UbPixel & p_min )
		{
			UbPixel l_result;
			l_result.r = substract( p_a.r, p_b.r, p_min.r );
			l_result.g = substract( p_a.g, p_b.g, p_min.g );
			l_result.b = substract( p_a.b, p_b.b, p_min.b );
			l_result.a = substract( p_a.a, p_b.a, p_min.a );
			return l_result;
		}
		/**
		 *\brief		Substracts 2 pixels given a minimal pixel value for the result
		 *\param[in]	p_a		The first pixel
		 *\param[in]	p_b		The second pixel
		 *\param[in]	p_min	The minimal pixel
		 *\return		(\p p_a - \p p_b ) with each component of the result >= to the same component from \p p_min
		 */
		template<> inline UbPixel substract< UiPixel, UiPixel, UbPixel >( const UiPixel & p_a, const UiPixel & p_b, const UbPixel & p_min )
		{
			UbPixel l_result;
			l_result.r = substract( p_a.r, p_b.r, p_min.r );
			l_result.g = substract( p_a.g, p_b.g, p_min.g );
			l_result.b = substract( p_a.b, p_b.b, p_min.b );
			l_result.a = substract( p_a.a, p_b.a, p_min.a );
			return l_result;
		}
		/**
		 *\brief		Adds a value to another one, the result is at most the given maximum value
		 *\param[in]	p_a		The left operand
		 *\param[in]	p_b		The right operand
		 *\param[in]	p_min	The minimum value
		 *\return		The result
		 */
		template< typename T, typename U, typename V > inline V add( T const & p_a, U const & p_b, V const & p_max )
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
		/**
		 *\brief		Adds 2 pixels given a maximal pixel value for the result
		 *\param[in]	p_a		The first pixel
		 *\param[in]	p_b		The second pixel
		 *\param[in]	p_max	The maximal pixel
		 *\return		(\p p_a + \p p_b ) with each component of the result <= to the same component from \p p_max
		 */
		template<> inline UbPixel add< UbPixel, UiPixel, UbPixel >( const UbPixel & p_a, const UiPixel & p_b, const UbPixel & p_max )
		{
			UbPixel l_result;
			l_result.r = add( p_a.r, p_b.r, p_max.r );
			l_result.g = add( p_a.g, p_b.g, p_max.g );
			l_result.b = add( p_a.b, p_b.b, p_max.b );
			l_result.a = add( p_a.a, p_b.a, p_max.a );
			return l_result;
		}
		/**
		 *\brief		Adds 2 pixels given a maximal pixel value for the result
		 *\param[in]	p_a		The first pixel
		 *\param[in]	p_b		The second pixel
		 *\param[in]	p_max	The maximal pixel
		 *\return		(\p p_a + \p p_b ) with each component of the result <= to the same component from \p p_max
		*/
		template<> inline UbPixel add< UiPixel, UiPixel, UbPixel >( const UiPixel & p_a, const UiPixel & p_b, const UbPixel & p_max )
		{
			UbPixel l_result;
			l_result.r = add( p_a.r, p_b.r, p_max.r );
			l_result.g = add( p_a.g, p_b.g, p_max.g );
			l_result.b = add( p_a.b, p_b.b, p_max.b );
			l_result.a = add( p_a.a, p_b.a, p_max.a );
			return l_result;
		}
		/**
		 *\brief		Computes euclidian length of the point (like it is a vector )
		 *\param[in]	p_ptPoint	The point
		 *\return		The length
		*/
		inline double length( wxPoint p_ptPoint )
		{
			return sqrt( double( p_ptPoint.x * p_ptPoint.x + p_ptPoint.y * p_ptPoint.y ) );
		}
		/**
		 *\brief		Writes a line in a console, and VisualStudio debug console, if it is used
		 *\param[in]	p_msg	The message
		*/
		bool Trace( wxString const & p_msg );
		/**
		 *\brief		Writes a line in a console, and VisualStudio debug console, if it is used
		 *\param[in]	p_szFormat	The line format, see printf
		 *\param[in]	...			The parameters, according to line format
		*/
		bool Trace( char const * p_szFormat, ... );
		/**
		 *\brief		Writes a line in a console, and VisualStudio debug console, if it is used
		 *\param[in]	p_szFormat	The line format, see printf
		 *\param[in]	...			The parameters, according to line format
		*/
		bool Trace( wchar_t const * p_szFormat, ... );
	}
}

#ifndef NDEBUG
#	define TRACE utils::Trace
#else
#	define TRACE false && utils::Trace
#endif

#endif
