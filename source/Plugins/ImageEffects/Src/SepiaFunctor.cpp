#include "SepiaFunctor.h"

#include <cmath>

namespace ProceduralTextures
{
	SepiaFunctor::SepiaFunctor()
		:	EffectFunctor( eEFFECT_TYPE_SEPIA, _( "Sepia" ), true, true )
	{
	}

	SepiaFunctor::~SepiaFunctor()
	{
	}

	void SepiaFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		UbPixel l_pixel;

		for ( int i = 0 ; i < m_iImgHeight ; i++ )
		{
			for ( int j = 0 ; j < m_iImgWidth ; j++ )
			{
				l_pixel.r = uint8_t( std::min( 255.0, ( p_bufferIn[i][j].r * 0.393 ) + ( p_bufferIn[i][j].g * 0.769 ) + ( p_bufferIn[i][j].b * 0.189 ) ) );
				l_pixel.g = uint8_t( std::min( 255.0, ( p_bufferIn[i][j].r * 0.349 ) + ( p_bufferIn[i][j].g * 0.686 ) + ( p_bufferIn[i][j].b * 0.168 ) ) );
				l_pixel.b = uint8_t( std::min( 255.0, ( p_bufferIn[i][j].r * 0.272 ) + ( p_bufferIn[i][j].g * 0.534 ) + ( p_bufferIn[i][j].b * 0.131 ) ) );
				l_pixel.a = 255;
				p_bufferOut[i][j].Set( l_pixel );
			}
		}
	}
}

//*************************************************************************************************
