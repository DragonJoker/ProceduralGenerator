#include "LgCell.h"

using namespace ProceduralTextures;

namespace LifeGame
{
	Cell::Cell()
		:	m_pixel( NULL )
	{
	}

	Cell::~Cell()
	{
	}

	void Cell::Set( UbPixel * p_pixel, bool p_alive )
	{
		m_pixel = p_pixel;
		m_alive = p_alive;

		if ( m_pixel != NULL )
		{
			SetAlive( p_alive );
		}
	}

	void Cell::Die()
	{
		m_alive = false;
		m_pixel->r = 255;
		m_pixel->g = 255;
		m_pixel->b = 255;
	}

	void Cell::Live()
	{
		m_alive = true;
		m_pixel->r = 0;
		m_pixel->g = 0;
		m_pixel->b = 0;
	}

	void Cell::SetAlive( bool p_alive )
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
