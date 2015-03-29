#include "GaussianBlurFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	const int GaussianBlurFunctor::m_iGaussLvl = 7;
	const int GaussianBlurFunctor::m_iGaussMask[] = {1, 6, 15, 20, 15, 6, 1};
	const int GaussianBlurFunctor::m_iGaussSum = 64;

	GaussianBlurFunctor::GaussianBlurFunctor()
		:	EffectFunctor( eEFFECT_TYPE_GAUSSIAN_BLUR, _( "Gaussian blur" ), true, true )
		,	m_temporaryBuffer( Size( 100, 100 ) )
	{
	}

	GaussianBlurFunctor::~GaussianBlurFunctor()
	{
	}

	void GaussianBlurFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		Pixel< double > l_linc;

		for ( uint32_t i = m_iGaussLvl - 1; i < m_size.y(); i++ )
		{
			for ( uint32_t j = 0; j < m_size.x(); j++ )
			{
				l_linc  =	m_iGaussMask[0] * p_bufferIn[i - 6][j];
				l_linc +=	m_iGaussMask[1] * p_bufferIn[i - 5][j];
				l_linc +=	m_iGaussMask[2] * p_bufferIn[i - 4][j];
				l_linc +=	m_iGaussMask[3] * p_bufferIn[i - 3][j];
				l_linc +=	m_iGaussMask[4] * p_bufferIn[i - 2][j];
				l_linc +=	m_iGaussMask[5] * p_bufferIn[i - 1][j];
				l_linc +=	m_iGaussMask[6] * p_bufferIn[i - 0][j];
				l_linc /=	m_iGaussSum;
				m_temporaryBuffer[i - 3][j].Set( l_linc.r, l_linc.g, l_linc.b, 255.0 );
			}
		}

		for ( uint32_t i = 0; i < m_size.y(); i++ )
		{
			for ( uint32_t j = m_iGaussLvl - 1; j < m_size.x(); j++ )
			{
				l_linc  =	m_iGaussMask[0] * m_temporaryBuffer[i][j - 6];
				l_linc +=	m_iGaussMask[1] * m_temporaryBuffer[i][j - 5];
				l_linc +=	m_iGaussMask[2] * m_temporaryBuffer[i][j - 4];
				l_linc +=	m_iGaussMask[3] * m_temporaryBuffer[i][j - 3];
				l_linc +=	m_iGaussMask[4] * m_temporaryBuffer[i][j - 2];
				l_linc +=	m_iGaussMask[5] * m_temporaryBuffer[i][j - 1];
				l_linc +=	m_iGaussMask[6] * m_temporaryBuffer[i][j - 0];
				l_linc /=	m_iGaussSum;
				p_bufferOut[i][j - 3].Set( l_linc.r, l_linc.g, l_linc.b, 255.0 );
			}
		}
	}

	void GaussianBlurFunctor::SetImage( PixelBuffer const & p_image )
	{
		EffectFunctor::SetImage( p_image );
		m_temporaryBuffer.Initialise( m_size );
	}
}
