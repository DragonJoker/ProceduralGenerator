#include "LgCell.h"

using namespace ProceduralTextures;

namespace LifeGame
{
	Cell::Cell()
		: m_pixel( NULL )
		, m_alive( 0 )
	{
	}

	Cell::~Cell()
	{
	}

	void Cell::Set( UbPixel * p_pixel, int p_alive )
	{
		m_pixel = p_pixel;
		m_alive = p_alive;

		if ( m_pixel )
		{
			SetAlive( p_alive );
		}
	}

	void Cell::Die()
	{
		m_alive = 0;
		m_pixel->r = 255;
		m_pixel->g = 255;
		m_pixel->b = 255;
	}

	void Cell::Live()
	{
		m_alive = 1;
		m_pixel->r = 0;
		m_pixel->g = 0;
		m_pixel->b = 0;
	}

	void Cell::SetAlive( int p_alive )
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

	int Cell::CountAliveNeighbours()
	{
		int l_return = 0;

		for ( auto && l_cell: m_neighbours )
		{
			l_return += l_cell->m_alive;
		}

		return l_return;
	}
}
