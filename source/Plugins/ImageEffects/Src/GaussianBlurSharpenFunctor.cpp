#include "GaussianBlurSharpenFunctor.h"

#include <GeneratorUtils.h>

#include <cmath>

namespace ProceduralTextures
{
	const int GaussianBlurSharpenFunctor::m_iGaussLvl			= 7;
	const int GaussianBlurSharpenFunctor::m_iGaussMask[]		= {1, 6, 15, 20, 15, 6, 1};
	const int GaussianBlurSharpenFunctor::m_iGaussSum			= 64;
	const double GaussianBlurSharpenFunctor::m_dSharpenZ		= 1.0;
	const double GaussianBlurSharpenFunctor::m_dSharpenMask[]	= {	0,				-m_dSharpenZ,		0,
																	-m_dSharpenZ,	1 + 4 * m_dSharpenZ,	-m_dSharpenZ,
																	0,				-m_dSharpenZ,		0
																};

	GaussianBlurSharpenFunctor::GaussianBlurSharpenFunctor()
		:	EffectFunctor( eEFFECT_TYPE_GAUSSIAN_BLUR_SHARPEN, _( "Gaussian blur + sharpen" ), true, true )
		,	m_temporaryBuffer1( Size( 100, 100 ) )
		,	m_temporaryBuffer2( Size( 100, 100 ) )
	{
	}

	GaussianBlurSharpenFunctor::~GaussianBlurSharpenFunctor()
	{
	}

	void GaussianBlurSharpenFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		Pixel< double > l_linc;
		UbPixel l_pixel;

		//	for (int i = m_iGaussLvl - 1 ; i < m_iImgHeight - 1 ; i++)
		for ( int i = m_iGaussLvl - 1 ; i < m_iImgHeight ; i++ )
		{
			//		for (int j = 1 ; j < m_iImgWidth - 1  ; j++)
			for ( int j = 0 ; j < m_iImgWidth ; j++ )
			{
				l_linc  = m_iGaussMask[0] * p_bufferIn[i - 6][j];
				l_linc += m_iGaussMask[1] * p_bufferIn[i - 5][j];
				l_linc += m_iGaussMask[2] * p_bufferIn[i - 4][j];
				l_linc += m_iGaussMask[3] * p_bufferIn[i - 3][j];
				l_linc += m_iGaussMask[4] * p_bufferIn[i - 2][j];
				l_linc += m_iGaussMask[5] * p_bufferIn[i - 1][j];
				l_linc += m_iGaussMask[6] * p_bufferIn[i - 0][j];
				l_linc /= m_iGaussSum;
				m_temporaryBuffer1[i - 3][j].Set( l_linc.r, l_linc.g, l_linc.b, 255.0 );
			}
		}

		//	for (int i = 1 ; i < m_iImgHeight - 1 ; i++)
		for ( int i = 0 ; i < m_iImgHeight ; i++ )
		{
			//		for (int j = m_iGaussLvl - 1 ; j < m_iImgWidth - 1 ; j++)
			for ( int j = m_iGaussLvl - 1 ; j < m_iImgWidth ; j++ )
			{
				l_linc  =	m_iGaussMask[0] * m_temporaryBuffer1[i][j - 6];
				l_linc +=	m_iGaussMask[1] * m_temporaryBuffer1[i][j - 5];
				l_linc +=	m_iGaussMask[2] * m_temporaryBuffer1[i][j - 4];
				l_linc +=	m_iGaussMask[3] * m_temporaryBuffer1[i][j - 3];
				l_linc +=	m_iGaussMask[4] * m_temporaryBuffer1[i][j - 2];
				l_linc +=	m_iGaussMask[5] * m_temporaryBuffer1[i][j - 1];
				l_linc +=	m_iGaussMask[6] * m_temporaryBuffer1[i][j - 0];
				l_linc /=	m_iGaussSum;
				m_temporaryBuffer2[i][j - 3].Set( l_linc.r, l_linc.g, l_linc.b, 255.0 );
			}
		}

		for ( int i = 1 ; i < m_iImgHeight - 1 ; i++ )
		{
			for ( int j = 1 ; j < m_iImgWidth - 1 ; j++ )
			{
				l_linc.Set( 0.0, 0.0, 0.0, 255.0 );
				l_linc += m_dSharpenMask[0] * m_temporaryBuffer2[i - 1][j - 1];
				l_linc += m_dSharpenMask[1] * m_temporaryBuffer2[i - 1][j + 0];
				l_linc += m_dSharpenMask[2] * m_temporaryBuffer2[i - 1][j + 1];
				l_linc += m_dSharpenMask[3] * m_temporaryBuffer2[i + 0][j - 1];
				l_linc += m_dSharpenMask[4] * m_temporaryBuffer2[i + 0][j + 0];
				l_linc += m_dSharpenMask[5] * m_temporaryBuffer2[i + 0][j + 1];
				l_linc += m_dSharpenMask[6] * m_temporaryBuffer2[i + 1][j - 1];
				l_linc += m_dSharpenMask[7] * m_temporaryBuffer2[i + 1][j + 0];
				l_linc += m_dSharpenMask[8] * m_temporaryBuffer2[i + 1][j + 1];
				p_bufferOut[i][j].Set( utils::clamp( 0.0, 255.0, l_linc.r ), utils::clamp( 0.0, 255.0, l_linc.g ), utils::clamp( 0.0, 255.0, l_linc.b ), 255.0 );
			}
		}
	}

	void GaussianBlurSharpenFunctor::SetImage( const wxImage & p_image )
	{
		EffectFunctor::SetImage( p_image );
		m_temporaryBuffer1.init( Size( m_iImgWidth, m_iImgHeight ) );
		m_temporaryBuffer2.init( Size( m_iImgWidth, m_iImgHeight ) );
	}
}
