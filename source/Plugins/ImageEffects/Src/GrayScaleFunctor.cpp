#include "GrayScaleFunctor.h"

#include <cmath>

namespace ProceduralTextures
{
	GrayScaleFunctor::GrayScaleFunctor()
		:	EffectFunctor( eEFFECT_TYPE_GRAYSCALE, _( "Gray scale" ), true, true )
	{
	}

	GrayScaleFunctor::~GrayScaleFunctor()
	{
	}

	void GrayScaleFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		uint8_t l_ucFactor;

		for ( int i = 0 ; i < m_iImgHeight ; i++ )
		{
			for ( int j = 0 ; j < m_iImgWidth ; j++ )
			{
				l_ucFactor = uint8_t( double( p_bufferIn[i][j].r ) * 0.21 + double( p_bufferIn[i][j].g ) * 0.71 + double( p_bufferIn[i][j].b ) * 0.07 );
				l_ucFactor = std::min( l_ucFactor, uint8_t( 255 ) );
				p_bufferOut[i][j].Set( l_ucFactor, l_ucFactor, l_ucFactor, uint8_t( 255 ) );
				p_bufferOut[i][j].a = 255;
			}
		}
	}
}
