#include "SharpenFunctor.h"

#include <GeneratorUtils.h>

namespace ProceduralTextures
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

	void SharpenFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		Pixel< double > l_linc;
		UbPixel l_pixel;

		for ( int i = 1 ; i < m_iImgHeight - 1 ; i++ )
		{
			for ( int j = 1 ; j < m_iImgWidth - 1 ; j++ )
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
				p_bufferOut[i][j].Set( utils::clamp( 0.0, 255.0, l_linc.r ), utils::clamp( 0.0, 255.0, l_linc.g ), utils::clamp( 0.0, 255.0, l_linc.b ), 255.0 );
			}
		}
	}
}
