#include "LinearNoiseReductionFunctor.h"

#include <cmath>

namespace ProceduralTextures
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

	void LinearNoiseReductionFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		Pixel< double > l_linc;

		for ( int i = 1 ; i < m_iImgHeight - 1 ; i++ )
		{
			for ( int j = 1 ; j < m_iImgWidth - 1 ; j++ )
			{
				l_linc  =	m_iLnrMask[0] * p_bufferIn[i - 1][j - 1];
				l_linc +=	m_iLnrMask[1] * p_bufferIn[i + 0][j - 1];
				l_linc +=	m_iLnrMask[2] * p_bufferIn[i + 1][j - 1];
				l_linc +=	m_iLnrMask[3] * p_bufferIn[i - 1][j + 0];
				l_linc +=	m_iLnrMask[4] * p_bufferIn[i + 0][j + 0];
				l_linc +=	m_iLnrMask[5] * p_bufferIn[i + 1][j + 0];
				l_linc +=	m_iLnrMask[6] * p_bufferIn[i - 1][j + 1];
				l_linc +=	m_iLnrMask[7] * p_bufferIn[i + 0][j + 1];
				l_linc +=	m_iLnrMask[8] * p_bufferIn[i + 1][j + 1];
				l_linc /=	9;
				p_bufferOut[i][j].Set( l_linc.r, l_linc.g, l_linc.b, 255.0 );
			}
		}
	}
}
