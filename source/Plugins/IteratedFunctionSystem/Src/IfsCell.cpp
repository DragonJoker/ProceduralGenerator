#include "IfsCell.h"

namespace ProceduralTextures
{
	IfsCell::IfsCell()
		:	m_pixel( NULL )
	{
	}

	IfsCell::IfsCell( const IfsCell & cell )
		:	m_pixel( cell.m_pixel )
		,	m_ptPosition( cell.m_ptPosition )
	{
	}

	IfsCell::~IfsCell()
	{
	}

	IfsCell & IfsCell::operator =( const IfsCell & cell )
	{
		m_pixel = cell.m_pixel;
		m_ptPosition = cell.m_ptPosition;
		return * this;
	}

	void IfsCell::Set( UbPixel * p_pixel, const Point2d & p_ptPosition )
	{
		m_pixel = p_pixel;
		m_ptPosition = p_ptPosition;
	}
}
