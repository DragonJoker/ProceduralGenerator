#include "SepiaFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	SepiaFunctor::SepiaFunctor()
		:	EffectFunctor( eEFFECT_TYPE_SEPIA, _( "Sepia" ), true, true )
	{
	}

	SepiaFunctor::~SepiaFunctor()
	{
	}

	void SepiaFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		DynPoint< UbPixel > const * l_pixelsIn = &p_bufferIn[0];
		DynPoint< UbPixel > * l_pixelsOut = &p_bufferOut[0];

		for ( uint32_t i = 0; i < m_size.y(); i++ )
		{
			UbPixel const * l_pixelIn = &( *l_pixelsIn )[1];
			UbPixel * l_pixelOut = &( *l_pixelsOut )[1];

			for ( uint32_t j = 0; j < m_size.x(); j++ )
			{
				l_pixelOut->Set( UbPixel(
					uint8_t( std::min( 255.0, ( l_pixelIn->r * 0.393 ) + ( l_pixelIn->g * 0.769 ) + ( l_pixelIn->b * 0.189 ) ) ),
					uint8_t( std::min( 255.0, ( l_pixelIn->r * 0.349 ) + ( l_pixelIn->g * 0.686 ) + ( l_pixelIn->b * 0.168 ) ) ),
					uint8_t( std::min( 255.0, ( l_pixelIn->r * 0.272 ) + ( l_pixelIn->g * 0.534 ) + ( l_pixelIn->b * 0.131 ) ) ),
					uint8_t( 255 )
				) );
				++l_pixelIn;
				++l_pixelOut;
			}

			++l_pixelsOut;
			++l_pixelsIn;
		}
	}
}
