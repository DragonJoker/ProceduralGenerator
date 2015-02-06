#include "DlaCell.h"

namespace ProceduralTextures
{
	DlaCell::DlaCell()
		:	m_pixel( NULL )
		,	m_whiteNeighbour( false )
		,	m_white( false )
		,	m_iValue( 0 )
	{
	}

	DlaCell::DlaCell( const DlaCell & cell )
		:	m_pixel( cell.m_pixel )
		,	m_whiteNeighbour( cell.m_whiteNeighbour )
		,	m_white( cell.m_white )
		,	m_iValue( cell.m_iValue )
	{
	}

	DlaCell::~DlaCell()
	{
	}

	DlaCell & DlaCell::operator =( const DlaCell & cell )
	{
		m_pixel = cell.m_pixel;
		m_whiteNeighbour = cell.m_whiteNeighbour;
		m_white = cell.m_white;
		m_iValue = cell.m_iValue;
		return * this;
	}

	void DlaCell::Set( UbPixel * p_pixel, bool p_white )
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

	void DlaCell::SetWhite( double p_dDistance )
	{
		m_white = true;
		m_pixel->r = 255;
		m_pixel->g = uint8_t( 255 * p_dDistance );
		m_pixel->b = uint8_t( 255 * p_dDistance );
	}

	bool DlaCell::IncValue( int p_iMinValue )
	{
		m_iValue++;

		if ( ! m_white )
		{
//			m_pixel->r = (255 * m_iValue ) / p_iMinValue;
//			m_pixel->g = (255 * m_iValue ) / p_iMinValue;
//			m_pixel->b = (255 * m_iValue ) / p_iMinValue;
		}

		return m_iValue >= p_iMinValue;
	}
}
