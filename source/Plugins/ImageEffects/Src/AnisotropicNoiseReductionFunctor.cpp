#include "AnisotropicNoiseReductionFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	const int AnisotropicNoiseReductionFunctor::m_iAdThreshold = 16;
	const int AnisotropicNoiseReductionFunctor::m_iAdSum = 9;
	const int AnisotropicNoiseReductionFunctor::m_iAdMask[]	= {	0,	2,	0,
																2,	1,	2,
																0,	2,	0
															  };

	AnisotropicNoiseReductionFunctor::AnisotropicNoiseReductionFunctor()
		:	EffectFunctor( eEFFECT_TYPE_ANISOTROPIC_NR, _( "Anisotropic noise reduction" ), true, true )
	{
	}

	AnisotropicNoiseReductionFunctor::~AnisotropicNoiseReductionFunctor()
	{
	}

	void AnisotropicNoiseReductionFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		DynPoint< UbPixel > * l_pixelsOut = &p_bufferOut[1];

		for ( uint32_t i = 1; i < m_size.y() - 1; i++ )
		{
			UbPixel * l_pixelOut = &( *l_pixelsOut )[1];

			for ( uint32_t j = 1; j < m_size.x() - 1; j++ )
			{
				UbPixel l_pixel = p_bufferIn[i][j];
				Pixel< int > l_avrg = m_iAdMask[0] * p_bufferIn[i - 1][j - 1];
				l_avrg += m_iAdMask[1] * p_bufferIn[i + 0][j - 1];
				l_avrg += m_iAdMask[2] * p_bufferIn[i + 1][j - 1];
				l_avrg += m_iAdMask[3] * p_bufferIn[i - 1][j + 0];
				l_avrg += m_iAdMask[4] * p_bufferIn[i + 0][j + 0];
				l_avrg += m_iAdMask[5] * p_bufferIn[i + 1][j + 0];
				l_avrg += m_iAdMask[6] * p_bufferIn[i - 1][j + 1];
				l_avrg += m_iAdMask[7] * p_bufferIn[i + 0][j + 1];
				l_avrg += m_iAdMask[8] * p_bufferIn[i + 1][j + 1];
				l_avrg /= m_iAdSum;
				Pixel< int > l_diff( l_avrg - l_pixel );

				if ( l_diff.r + l_diff.g + l_diff.b > m_iAdThreshold )
				{
					l_avrg = l_pixel;
				}

				l_pixelOut->Set( l_avrg.r, l_avrg.g, l_avrg.b, 255 );
				++l_pixelOut;
			}

			++l_pixelsOut;
		}
	}
}
