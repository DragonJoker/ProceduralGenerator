#include "DlaCell.h"

using namespace ProceduralTextures;

namespace DiffusionLimitedAggregation
{
	Cell::Cell()
		:	m_pixel( NULL )
		,	m_whiteNeighbour( false )
		,	m_white( false )
		,	m_iValue( 0 )
	{
	}

	Cell::~Cell()
	{
	}

	void Cell::Set( UbPixel * p_pixel, bool p_white )
	{
		m_pixel = p_pixel;
		m_whiteNeighbour = false;
		m_white = p_white;
		m_iValue = 0;

		if ( p_white && m_pixel != NULL )
		{
			SetWhite( 1.0 );
		}
	}

	void Cell::SetWhite( double p_dDistance )
	{
		m_white = true;
		m_pixel->r = 255;
		m_pixel->g = uint8_t( 255 * p_dDistance );
		m_pixel->b = uint8_t( 255 * p_dDistance );
	}

	bool Cell::IncValue( int p_iMinValue )
	{
		m_iValue++;

		if ( !m_white )
		{
//			m_pixel->r = (255 * m_iValue ) / p_iMinValue;
//			m_pixel->g = (255 * m_iValue ) / p_iMinValue;
//			m_pixel->b = (255 * m_iValue ) / p_iMinValue;
		}

		return m_iValue >= p_iMinValue;
	}
}
