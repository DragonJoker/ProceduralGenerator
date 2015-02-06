#include "ExplosionFunctor.h"

#include <cmath>

namespace ProceduralTextures
{
	ExplosionFunctor::ExplosionFunctor()
		:	EffectFunctor( eEFFECT_TYPE_EXPLOSION, _( "Explosion" ), true, true )
	{
	}

	ExplosionFunctor::~ExplosionFunctor()
	{
	}

	void ExplosionFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		Point2d l_ptCen;
		Point2d l_ptMCen;
		Point2i l_ptCoord;
		Point2i l_ptSize( m_iImgHeight, m_iImgWidth );

		for ( int i = 0 ; i < m_iImgHeight ; i++ )
		{
			l_ptCen = Point2d( 0.5 - double( i ) / double( m_iImgHeight ), 0.0 );

			for ( int j = 0 ; j < m_iImgWidth ; j++ )
			{
				l_ptCen = Point2d( l_ptCen[0], 0.5 - double( j ) / double( m_iImgWidth ) );
				l_ptMCen = l_ptCen.GetNormalised() * ( -0.07 * log( l_ptCen.GetLength() ) );
				l_ptCoord = l_ptMCen * l_ptSize;
				l_ptCoord[0] = std::max( 0, std::min( m_iImgHeight - 1, i + l_ptCoord[0] ) );
				l_ptCoord[1] = std::max( 0, std::min( m_iImgWidth - 1, j + l_ptCoord[1] ) );
				p_bufferOut[i][j].Set( p_bufferIn[l_ptCoord[0]][l_ptCoord[1]] );
			}
		}
	}
}
