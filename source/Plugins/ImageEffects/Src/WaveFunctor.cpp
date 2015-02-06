#include "WaveFunctor.h"

#include <cmath>

namespace ProceduralTextures
{
	double	WaveFunctor::m_dDegToRad = M_PI / 180.0;
	double	WaveFunctor::m_dPiMult2 = M_PI * 2;
	double	WaveFunctor::m_dCos[360];
	double	WaveFunctor::m_dSin[360];

	WaveFunctor::WaveFunctor()
		:	EffectFunctor( eEFFECT_TYPE_WAVE, _( "Wave" ), true, true )
		,	m_dTimeIndexX( 0.0 )
		,	m_dTimeIndexY( 0.0 )
		,	m_iDecalX( 0 )
		,	m_iDecalY( 0 )
		,	m_dTimeScaleX( 1.5 )
		,	m_dTimeScaleY( 1.0 )
		,	m_dScaleX( 5.0 )
		,	m_dScaleY( 5.0 )
		,	m_dTranslateX( 1.0 )
		,	m_dTranslateY( 0.2 )
		,	m_dCurTranslateX( 0.0 )
		,	m_dCurTranslateY( 0.0 )
		,	m_dNbPixelsX( 5.0 )
		,	m_dNbPixelsY( 5.0 )
	{
		for ( int i = 0 ; i < 360 ; i++ )
		{
			m_dCos[i] = cos( double( i ) * m_dDegToRad );
			m_dSin[i] = sin( double( i ) * m_dDegToRad );
		}
	}

	WaveFunctor::~WaveFunctor()
	{
	}

	void WaveFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		int x;
		int y;
		UbPixel l_pixel;

		for ( int i = 0 ; i < m_iImgHeight ; i++ )
		{
			m_iDecalY = int( m_dCurTranslateY + m_dSin[int( m_dTimeIndexY + m_dScaleY * 360.0 * double( i ) / double( m_iImgHeight ) ) % 360] * m_dNbPixelsY );

			if ( m_iDecalY >= 0 )
			{
				if ( i + m_iDecalY < m_iImgHeight )
				{
					y = i + m_iDecalY;
				}
				else
				{
					y = i + m_iDecalY - m_iImgHeight;
				}
			}
			else
			{
				if ( i + m_iDecalY >= 0 )
				{
					y = i + m_iDecalY;
				}
				else
				{
					y = i + m_iDecalY + m_iImgHeight;
				}
			}

			for ( int j = 0 ; j < m_iImgWidth ; j++ )
			{
				m_iDecalX = int( m_dCurTranslateX + m_dCos[int( m_dTimeIndexX + m_dScaleX * 360.0 * double( j ) / double( m_iImgWidth ) ) % 360] * m_dNbPixelsX );

				if ( m_iDecalX >= 0 )
				{
					if ( j + m_iDecalX < m_iImgWidth )
					{
						x = j + m_iDecalX;
					}
					else
					{
						x = j + m_iDecalX - m_iImgWidth;
					}
				}
				else
				{
					if ( j + m_iDecalX >= 0 )
					{
						x = j + m_iDecalX;
					}
					else
					{
						x = j + m_iDecalX + m_iImgWidth;
					}
				}

				p_bufferOut[i][j].Set( p_bufferIn[y][x] );
			}
		}

		m_dTimeIndexX += m_dTimeScaleX * 10.0;
		m_dTimeIndexY += m_dTimeScaleY * 10.0;
		m_dCurTranslateX += m_dTranslateX;
		m_dCurTranslateY += m_dTranslateY;

		if ( m_dTimeIndexX >= 360.0 )
		{
			m_dTimeIndexX -= 360.0;
		}

		if ( m_dTimeIndexY >= 360.0 )
		{
			m_dTimeIndexY -= 360.0;
		}

		if ( m_dCurTranslateX + m_dNbPixelsX >= m_iImgWidth )
		{
			m_dCurTranslateX -= m_iImgWidth;
		}

		if ( m_dCurTranslateY + m_dNbPixelsY >= m_iImgHeight )
		{
			m_dCurTranslateY -= m_iImgHeight;
		}
	}
}
