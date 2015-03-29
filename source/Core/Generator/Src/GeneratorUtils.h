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
		/**
		 *\brief		Returns the size of a static C-Style array
		 *\param[in]	p_data	The array
		 *\return		The size
		 */
		template< typename T, size_t N >
		inline size_t GetCountOf( T const( & p_data )[N] )
		{
			return N;
		}
		/**
		 *\brief		Puts a value between two bounds
		 *\param[in]	p_min		The min bound
		 *\param[in]	p_max		The max bound
		 *\param[in]	p_value		The value to clamp
		 *\return		The clamped value
		 */
		template< typename T > inline T Clamp( T const & p_min, T const & p_max, T const & p_value )
		{
			return std::max( p_min, std::min( p_max, p_value ) );
		}
		/**
		 *\brief		Substract a value from another one, the result is At least the given minimum value
		 *\param[in]	p_a		The left operand
		 *\param[in]	p_b		The right operand
		 *\param[in]	p_min	The minimum value
		 *\return		The result
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
		/**
		 *\brief		Substracts 2 pixels given a minimal pixel value for the result
		 *\param[in]	p_a		The first pixel
		 *\param[in]	p_b		The second pixel
		 *\param[in]	p_min	The minimal pixel
		 *\return		(\p p_a - \p p_b ) with each component of the result >= to the same component from \p p_min
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
		/**
		 *\brief		Substracts 2 pixels given a minimal pixel value for the result
		 *\param[in]	p_a		The first pixel
		 *\param[in]	p_b		The second pixel
		 *\param[in]	p_min	The minimal pixel
		 *\return		(\p p_a - \p p_b ) with each component of the result >= to the same component from \p p_min
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
		/**
		 *\brief		Adds a value to another one, the result is At most the given maximum value
		 *\param[in]	p_a		The left operand
		 *\param[in]	p_b		The right operand
		 *\param[in]	p_max	The maximum value
		 *\return		The result
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
		/**
		 *\brief		Adds 2 pixels given a maximal pixel value for the result
		 *\param[in]	p_a		The first pixel
		 *\param[in]	p_b		The second pixel
		 *\param[in]	p_max	The maximal pixel
		 *\return		(\p p_a + \p p_b ) with each component of the result <= to the same component from \p p_max
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
		/**
		 *\brief		Adds 2 pixels given a maximal pixel value for the result
		 *\param[in]	p_a		The first pixel
		 *\param[in]	p_b		The second pixel
		 *\param[in]	p_max	The maximal pixel
		 *\return		(\p p_a + \p p_b ) with each component of the result <= to the same component from \p p_max
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
		/**
		 *\brief		Computes the pixel components value from an unsigned long value
		 *\param[in]	p_colour	The colour
		 *\return		The pixel
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
		/**
		 *\brief		Computes the pixel components value from an unsigned long value
		 *\param[in]	p_colour	The colour
		 *\return		The pixel
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
		/*!
		\author 	Sylvain Doremus
		\version	2.0.0
		\date		10/02/2015
		\brief		Trace class
		*/
		class GeneratorAPI Tracer
		{
		public:
			/**
			 *\brief		Writes a line in a console, and VisualStudio debug console, if it is used
			 *\param[in]	p_msg	The message
			 */
			static bool Trace( std::string const & p_msg );
			/**
			 *\brief		Writes a line in a console, and VisualStudio debug console, if it is used
			 *\param[in]	p_msg	The message
			 */
			static bool Trace( std::wstring const & p_msg );
			/**
			 *\brief		Writes a line in a console, and VisualStudio debug console, if it is used
			 *\param[in]	p_szFormat	The line format, see printf
			 *\param[in]	...			The parameters, according to line format
			 */
			static bool Trace( char const * p_szFormat, ... );
			/**
			 *\brief		Writes a line in a console, and VisualStudio debug console, if it is used
			 *\param[in]	p_szFormat	The line format, see printf
			 *\param[in]	...			The parameters, according to line format
			 */
			static bool Trace( wchar_t const * p_szFormat, ... );
		};
	}

	namespace System
	{
		/**
		 *\brief		Retrieves the given screen size
		 *\param[in]	p_screen	The screen index
		 *\param[out]	p_size		Receives the screen size
		 *\return		\p true if the size was retrieved
		 */
		bool GeneratorAPI GetScreenSize( uint32_t p_screen, Size & p_size );
		/**
		 *\brief		Counts available CPUs
		 *\return		The CPU count
		 */
		int GeneratorAPI GetCPUCount();
		/**
		 *\brief		Retrieves the executable directory
		 *\return		The directory
		 */
		String GeneratorAPI GetExecutableDirectory();
	}
}

#ifndef NDEBUG
#	define TRACE ProceduralTextures::Utils::Tracer::Trace
#else
#	define TRACE false && ProceduralTextures::Utils::Tracer::Trace
#endif

#endif
