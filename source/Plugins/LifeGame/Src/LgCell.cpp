#include "LgCell.h"

namespace ProceduralTextures
{
	LgCell::LgCell()
		:	m_pixel( NULL )
	{
	}

	LgCell::LgCell( const LgCell & cell )
		:	m_pixel( cell.m_pixel )
		,	m_alive( cell.m_alive )
	{
	}

	LgCell::~LgCell()
	{
	}

	LgCell & LgCell::operator =( const LgCell & cell )
	{
		m_pixel = cell.m_pixel;
		m_alive = cell.m_alive;
		return * this;
	}

	void LgCell::Set( UbPixel * p_pixel, bool p_alive )
	{
		m_pixel = p_pixel;
		m_alive = p_alive;

		if ( m_pixel != NULL )
		{
			SetAlive( p_alive );
		}
	}

	void LgCell::Die()
	{
		m_alive = false;
		m_pixel->r = 255;
		m_pixel->g = 255;
		m_pixel->b = 255;
	}

	void LgCell::Live()
	{
		m_alive = true;
		m_pixel->r = 0;
		m_pixel->g = 0;
		m_pixel->b = 0;
	}

	void LgCell::SetAlive( bool p_alive )
	{
		if ( p_alive )
		{
			Live();
		}
		else
		{
			Die();
		}
	}
}
