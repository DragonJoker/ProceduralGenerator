#include "GrayScaleFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	GrayScaleFunctor::GrayScaleFunctor()
		:	EffectFunctor( eEFFECT_TYPE_GRAYSCALE, _( "Gray scale" ), true, true )
	{
	}

	GrayScaleFunctor::~GrayScaleFunctor()
	{
	}

	void GrayScaleFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		uint8_t l_ucFactor;

		for ( uint32_t i = 0; i < m_size.y(); i++ )
		{
			for ( uint32_t j = 0; j < m_size.x(); j++ )
			{
				l_ucFactor = uint8_t( double( p_bufferIn[i][j].r ) * 0.21 + double( p_bufferIn[i][j].g ) * 0.71 + double( p_bufferIn[i][j].b ) * 0.07 );
				l_ucFactor = std::min( l_ucFactor, uint8_t( 255 ) );
				p_bufferOut[i][j].Set( l_ucFactor, l_ucFactor, l_ucFactor, uint8_t( 255 ) );
				p_bufferOut[i][j].a = 255;
			}
		}
	}
}
