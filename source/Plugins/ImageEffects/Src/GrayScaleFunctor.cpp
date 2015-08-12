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
		DynPoint< UbPixel > const * l_pixelsIn = &p_bufferIn[0];
		DynPoint< UbPixel > * l_pixelsOut = &p_bufferOut[0];

		for ( uint32_t i = 0; i < m_size.y(); i++ )
		{
			UbPixel const * l_pixelIn = &( *l_pixelsIn )[1];
			UbPixel * l_pixelOut = &( *l_pixelsOut )[1];

			for ( uint32_t j = 0; j < m_size.x(); j++ )
			{
				uint8_t l_ucFactor = uint8_t( double( l_pixelIn->r ) * 0.21 + double( l_pixelIn->g ) * 0.71 + double( l_pixelIn->b ) * 0.07 );
				l_ucFactor = std::min( l_ucFactor, uint8_t( 255 ) );
				l_pixelOut->Set( l_ucFactor, l_ucFactor, l_ucFactor, uint8_t( 255 ) );
				++l_pixelIn;
				++l_pixelOut;
			}

			++l_pixelsIn;
			++l_pixelsOut;
		}
	}
}
