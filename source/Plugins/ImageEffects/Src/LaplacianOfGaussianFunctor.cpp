#include "LaplacianOfGaussianFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	const int LaplacianOfGaussianFunctor::m_iGaussLvl = 7;
	const int LaplacianOfGaussianFunctor::m_mask[]
	{
		 0,  0, -1,  0,  0,
		 0, -1, -2, -1,  0,
		-1, -2, 16, -2, -1,
		 0, -1, -2, -1,  0,
		 0,  0, -1,  0,  0,
	};
	const int LaplacianOfGaussianFunctor::m_iGaussSum = 64;

	LaplacianOfGaussianFunctor::LaplacianOfGaussianFunctor()
		:	EffectFunctor( eEFFECT_TYPE_GAUSSIAN_BLUR, _( "Laplacian of Gaussian" ), true, true )
		,	m_temporaryBuffer( Size( 100, 100 ) )
	{
	}

	LaplacianOfGaussianFunctor::~LaplacianOfGaussianFunctor()
	{
	}

	void LaplacianOfGaussianFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
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

	void LaplacianOfGaussianFunctor::SetImage( PixelBuffer const & p_image )
	{
		EffectFunctor::SetImage( p_image );
		m_temporaryBuffer.Initialise( m_size );
	}
}
