#include "ClgCell.h"

#include <GeneratorUtils.h>

using namespace ProceduralTextures;

namespace ColoredLifeGame
{
	Cell::Cell()
		: m_nextPixel( NULL )
		, m_deadPixel( NULL )
		, m_alivePixel( NULL )
		, m_stepPixel( NULL )
		, m_alive( 0 )
	{
	}

	Cell::~Cell()
	{
	}

	void Cell::Set( UbPixel * p_pixel, UbPixel * p_deadPixel, UbPixel * p_alivePixel, UiPixel * p_stepPixel, int p_alive )
	{
		m_nextPixel = p_pixel;
		m_deadPixel = p_deadPixel;
		m_alivePixel = p_alivePixel;
		m_stepPixel = p_stepPixel;
		m_alive = p_alive;

		if ( m_alivePixel && m_deadPixel && m_nextPixel && m_stepPixel )
		{
			SetAlive( p_alive );

			if ( m_alive )
			{
				m_nextPixel->r = m_alivePixel->r;
				m_nextPixel->g = m_alivePixel->g;
				m_nextPixel->b = m_alivePixel->b;
				m_nextPixel->a = m_alivePixel->a;
			}
			else
			{
				m_nextPixel->r = m_deadPixel->r;
				m_nextPixel->g = m_deadPixel->g;
				m_nextPixel->b = m_deadPixel->b;
				m_nextPixel->a = m_deadPixel->a;
			}
		}
	}

	void Cell::Die()
	{
		m_alive = 0;
		*m_nextPixel = Utils::Add( *m_nextPixel, *m_stepPixel, *m_deadPixel );
	}

	void Cell::Live()
	{
		m_alive = 1;
		m_nextPixel->r = m_alivePixel->r;
		m_nextPixel->g = m_alivePixel->g;
		m_nextPixel->b = m_alivePixel->b;
		m_nextPixel->a = m_alivePixel->a;
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
