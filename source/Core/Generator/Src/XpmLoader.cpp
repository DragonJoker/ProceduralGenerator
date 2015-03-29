#include "XpmLoader.h"

#include "GeneratorUtils.h"

#include <cassert>

namespace ProceduralTextures
{
	XpmLoader::XpmLoader()
	{
	}

	XpmLoader::~XpmLoader()
	{
	}

	PixelBuffer XpmLoader::LoadImage( char const ** p_data, size_t p_size )
	{
		if ( !p_data )
		{
			throw std::invalid_argument( "XpmImage - Null data" );
		}

		if ( !p_size )
		{
			throw std::invalid_argument( "XpmImage - Empty file" );
		}

		Size l_size;
		uint32_t l_coloursCount = 0;
		uint32_t l_charCount = 0;
		std::stringstream l_stream( p_data[0] );
		l_stream >> l_size.x() >> l_size.y() >> l_coloursCount >> l_charCount;

		if ( !l_size.x() || !l_size.y() || !l_coloursCount || !l_charCount )
		{
			throw std::invalid_argument( "XpmImage - Invalid header" );
		}

		if ( l_size.y() + l_coloursCount != p_size - 1 )
		{
			throw std::invalid_argument( "XpmImage - Invalid data size" );
		}

		// Parse colours
		std::map< std::string, UbPixel > l_colours;
		std::for_each( &p_data[1], &p_data[1 + l_coloursCount], [&l_colours, &l_charCount]( char const * l_line )
		{
			std::string l_code( &l_line[0], &l_line[l_charCount] );
			char const * l_it = std::strstr( l_line, "c " );

			if ( !l_it )
			{
				throw std::invalid_argument( "XpmImage - Malformed colour line" );
			}

			std::string l_value( l_it + 2, &l_line[std::strlen( l_line )] );
			UbPixel l_pixel;

			if ( l_value.find( '#' ) != std::string::npos )
			{
				uint32_t l_r, l_g, l_b;

				if ( std::sscanf( l_value.c_str(), "#%02X%02X%02X", &l_r, &l_g, &l_b ) == EOF )
				{
					throw std::invalid_argument( "XpmImage - Invalid image data" );
				}

				l_pixel.r = l_r;
				l_pixel.g = l_g;
				l_pixel.b = l_b;
				l_pixel.a = 0xFF;
			}

			l_colours[l_code] = l_pixel;
		} );

		// Parse image
		PixelBuffer l_pixels( l_size );
		UbPixel * l_buffer = l_pixels.Ptr();

		for ( char const ** l_it = &p_data[1 + l_coloursCount]; l_it != &p_data[1 + l_coloursCount + l_size.y()]; ++l_it )
		{
			char const * l_line = *l_it;

			for ( uint32_t x = 0; x < l_size.x(); ++x )
			{
				l_buffer->Set( l_colours[std::string( l_line, l_line + l_charCount )] );
				l_line += l_charCount;
				l_buffer++;
			}
		}

		return l_pixels;
	}
}
