#include "ExplosionFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	ExplosionFunctor::ExplosionFunctor()
		:	EffectFunctor( eEFFECT_TYPE_EXPLOSION, _( "Explosion" ), true, true )
	{
	}

	ExplosionFunctor::~ExplosionFunctor()
	{
	}

	void ExplosionFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		Point2d l_ptCen;
		Point2d l_ptMCen;
		Point2i l_ptCoord;
		Point2i l_ptSize( m_size.y(), m_size.x() );

		for ( uint32_t i = 0; i < m_size.y(); i++ )
		{
			l_ptCen = Point2d( 0.5 - double( i ) / double( m_size.y() ), 0.0 );

			for ( uint32_t j = 0; j < m_size.x(); j++ )
			{
				l_ptCen = Point2d( l_ptCen[0], 0.5 - double( j ) / double( m_size.x() ) );
				l_ptMCen = GetNormalised( l_ptCen ) * ( -0.07 * log( GetDistance( l_ptCen ) ) );
				l_ptCoord = l_ptMCen * l_ptSize;
				l_ptCoord[0] = std::max( 0, std::min< int >( m_size.y() - 1, i + l_ptCoord[0] ) );
				l_ptCoord[1] = std::max( 0, std::min< int >( m_size.x() - 1, j + l_ptCoord[1] ) );
				p_bufferOut[i][j].Set( p_bufferIn[l_ptCoord[0]][l_ptCoord[1]] );
			}
		}
	}
}
