#include "ClgCell.h"

#include <GeneratorUtils.h>

namespace ProceduralTextures
{
	ClgCell::ClgCell()
		:	m_nextPixel( NULL )
		,	m_deadPixel( NULL )
		,	m_alivePixel( NULL )
		,	m_stepPixel( NULL )
	{
	}

	ClgCell::ClgCell( const ClgCell & cell )
		:	m_nextPixel( cell.m_nextPixel )
		,	m_deadPixel( cell.m_deadPixel )
		,	m_alivePixel( cell.m_alivePixel )
		,	m_stepPixel( cell.m_stepPixel )
		,	m_alive( cell.m_alive )
	{
	}

	ClgCell::~ClgCell()
	{
	}

	ClgCell  & ClgCell::operator =( const ClgCell & cell )
	{
		m_nextPixel		= cell.m_nextPixel;
		m_deadPixel		= cell.m_deadPixel;
		m_alivePixel	= cell.m_alivePixel;
		m_stepPixel		= cell.m_stepPixel;
		m_alive			= cell.m_alive;
		return * this;
	}

	void ClgCell::Set( UbPixel * p_pixel, UbPixel * p_deadPixel, UbPixel * p_alivePixel, UiPixel * p_stepPixel, bool p_alive )
	{
		m_nextPixel		= p_pixel;
		m_deadPixel		= p_deadPixel;
		m_alivePixel	= p_alivePixel;
		m_stepPixel		= p_stepPixel;
		m_alive			= p_alive;

		if ( m_alivePixel != NULL && m_deadPixel != NULL && m_nextPixel != NULL && m_stepPixel != NULL )
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

	void ClgCell::Die()
	{
		m_alive = false;
		*m_nextPixel = utils::add( *m_nextPixel, *m_stepPixel, *m_deadPixel );
	}

	void ClgCell::Live()
	{
		m_alive = true;
		m_nextPixel->r = m_alivePixel->r;
		m_nextPixel->g = m_alivePixel->g;
		m_nextPixel->b = m_alivePixel->b;
		m_nextPixel->a = m_alivePixel->a;
	}

	void ClgCell::SetAlive( bool p_alive )
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
