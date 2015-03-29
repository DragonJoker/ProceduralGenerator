#include "SharpenFunctor.h"

#include <GeneratorUtils.h>

using namespace ProceduralTextures;

namespace ImageEffects
{
	const double SharpenFunctor::m_dSharpenZ = 1.0;
	const double SharpenFunctor::m_dSharpenMask[]	= {	0, -m_dSharpenZ, 0,
														-m_dSharpenZ, 1 + 4 * m_dSharpenZ, -m_dSharpenZ,
														0, -m_dSharpenZ, 0
													};

	SharpenFunctor::SharpenFunctor()
		:	EffectFunctor( eEFFECT_TYPE_SHARPEN, _( "Sharpen" ), true, true )
	{
	}

	SharpenFunctor::~SharpenFunctor()
	{
	}

	void SharpenFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		Pixel< double > l_linc;
		UbPixel l_pixel;

		for ( uint32_t i = 1; i < m_size.y() - 1; i++ )
		{
			for ( uint32_t j = 1; j < m_size.x() - 1; j++ )
			{
				l_linc.Set( 0.0, 0.0, 0.0, 255.0 );
				l_linc += m_dSharpenMask[0] * p_bufferIn[i - 1][j - 1];
				l_linc += m_dSharpenMask[1] * p_bufferIn[i - 1][j + 0];
				l_linc += m_dSharpenMask[2] * p_bufferIn[i - 1][j + 1];
				l_linc += m_dSharpenMask[3] * p_bufferIn[i + 0][j - 1];
				l_linc += m_dSharpenMask[4] * p_bufferIn[i + 0][j + 0];
				l_linc += m_dSharpenMask[5] * p_bufferIn[i + 0][j + 1];
				l_linc += m_dSharpenMask[6] * p_bufferIn[i + 1][j - 1];
				l_linc += m_dSharpenMask[7] * p_bufferIn[i + 1][j + 0];
				l_linc += m_dSharpenMask[8] * p_bufferIn[i + 1][j + 1];
				p_bufferOut[i][j].Set( Utils::Clamp( 0.0, 255.0, l_linc.r ), Utils::Clamp( 0.0, 255.0, l_linc.g ), Utils::Clamp( 0.0, 255.0, l_linc.b ), 255.0 );
			}
		}
	}
}
