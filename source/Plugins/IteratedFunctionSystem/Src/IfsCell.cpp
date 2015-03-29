#include "IfsCell.h"

using namespace ProceduralTextures;

namespace IteratedFunctionSystem
{
	Cell::Cell()
		:	m_pixel( NULL )
	{
	}

	Cell::~Cell()
	{
	}

	void Cell::Set( UbPixel * p_pixel, Point2d const & p_ptPosition )
	{
		m_pixel = p_pixel;
		m_ptPosition = p_ptPosition;
	}
}
