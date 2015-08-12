#include "LinearNoiseReductionFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	const int LinearNoiseReductionFunctor::m_iLnrMask[]	= {	1,	1,	1,
															1,	1,	1,
															1,	1,	1
														  };

	LinearNoiseReductionFunctor::LinearNoiseReductionFunctor()
		:	EffectFunctor( eEFFECT_TYPE_LINEAR_NR, _( "Linear noise reduction" ), true, true )
	{
	}

	LinearNoiseReductionFunctor::~LinearNoiseReductionFunctor()
	{
	}

	void LinearNoiseReductionFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		DynPoint< UbPixel > * l_pixelsOut = &p_bufferOut[1];

		for ( uint32_t i = 1; i < m_size.y() - 1; i++ )
		{
			UbPixel * l_pixelOut = &( *l_pixelsOut )[1];

			for ( uint32_t j = 1; j < m_size.x() - 1; j++ )
			{
				Pixel< double > l_linc( m_iLnrMask[0] * p_bufferIn[i - 1][j - 1] );
				l_linc += m_iLnrMask[1] * p_bufferIn[i + 0][j - 1];
				l_linc += m_iLnrMask[2] * p_bufferIn[i + 1][j - 1];
				l_linc += m_iLnrMask[3] * p_bufferIn[i - 1][j + 0];
				l_linc += m_iLnrMask[4] * p_bufferIn[i + 0][j + 0];
				l_linc += m_iLnrMask[5] * p_bufferIn[i + 1][j + 0];
				l_linc += m_iLnrMask[6] * p_bufferIn[i - 1][j + 1];
				l_linc += m_iLnrMask[7] * p_bufferIn[i + 0][j + 1];
				l_linc += m_iLnrMask[8] * p_bufferIn[i + 1][j + 1];
				l_linc /= 9;
				l_pixelOut->Set( l_linc.r, l_linc.g, l_linc.b, 255.0 );
				++l_pixelOut;
			}

			++l_pixelsOut;
		}
	}
}
