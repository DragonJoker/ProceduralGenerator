#include "BlgCell.h"

#include <GeneratorUtils.h>

namespace ProceduralTextures
{
	BlgCell::BlgCell()
		:	m_alivePx( NULL )
		,	m_deadPx( NULL )
		,	m_nextPixel( NULL )
		,	m_alive( false )
	{
	}

	BlgCell::BlgCell( const BlgCell & p_cell )
		:	m_alivePx( p_cell.m_alivePx )
		,	m_deadPx( p_cell.m_deadPx )
		,	m_nextPixel( p_cell.m_nextPixel )
		,	m_pixel( p_cell.m_pixel )
		,	m_step( p_cell.m_step )
		,	m_age( p_cell.m_age )
		,	m_med( p_cell.m_med )
		,	m_alive( p_cell.m_alive )
	{
		for ( std::size_t i = 0 ; i < 8 ; i++ )
		{
			m_neighbours[i] = p_cell.m_neighbours[i];
		}
	}

	BlgCell::~BlgCell()
	{
	}

	BlgCell & BlgCell::operator =( const BlgCell & p_cell )
	{
		m_alivePx	= p_cell.m_alivePx;
		m_deadPx	= p_cell.m_deadPx;
		m_alive		= p_cell.m_alive;
		m_nextPixel	= p_cell.m_nextPixel;
		m_pixel		= p_cell.m_pixel;
		m_step		= p_cell.m_step;
		m_age		= p_cell.m_age;
		m_med		= p_cell.m_med;
		m_alive		= p_cell.m_alive;

		for ( std::size_t i = 0 ; i < 8 ; i++ )
		{
			m_neighbours[i] = p_cell.m_neighbours[i];
		}

		return * this;
	}

	void BlgCell::Set( UbPixel * p_pixel, UbPixel * p_alivePx, UbPixel * p_deadPx, bool p_alive )
	{
		m_alivePx = p_alivePx;
		m_deadPx = p_deadPx;
		m_nextPixel = p_pixel;

		if ( m_alivePx && m_deadPx && m_nextPixel )
		{
			m_step.r = ( ( unsigned int )( m_alivePx->r ) + 1 - m_deadPx->r ) >> 5;
			m_step.g = ( ( unsigned int )( m_alivePx->g ) + 1 - m_deadPx->g ) >> 5;
			m_step.b = ( ( unsigned int )( m_alivePx->b ) + 1 - m_deadPx->b ) >> 5;
			m_step.a = ( ( unsigned int )( m_alivePx->a ) + 1 - m_deadPx->a ) >> 5;
			SetAlive( p_alive );
		}
	}

	void BlgCell::Die()
	{
		m_alive = false;
		*m_nextPixel = utils::substract( m_pixel, m_step, *m_deadPx );
		m_med = *m_nextPixel / m_step;
	}

	void BlgCell::Live()
	{
		if ( ! m_alive )
		{
			m_age.r = 0;
			m_age.g = 0;
			m_age.b = 0;
			m_age.a = 0;
		}

		m_alive = true;
		m_age += m_step;
		*m_nextPixel = utils::add( m_pixel, m_age, *m_alivePx );
		m_med = *m_nextPixel / m_step;
	}

	void BlgCell::Stay()
	{
		if ( m_alive )
		{
			Live();
		}
		else
		{
			Die();
		}
	}

	void BlgCell::SetAlive( bool p_alive )
	{
		m_alive = p_alive;

		if ( m_alive )
		{
			*m_nextPixel = *m_alivePx;
		}
		else
		{
			*m_nextPixel = *m_deadPx;
		}
	}

	void BlgCell::Update()
	{
		m_pixel.r = 0;
		m_pixel.g = 0;
		m_pixel.b = 0;
		m_pixel.a = 0;

		for ( int i = 0 ; i < 8 ; i++ )
		{
			m_pixel += m_neighbours[i]->m_med;
		}
	}
}
