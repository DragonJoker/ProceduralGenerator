/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___DLA_CELL_H___
#define ___DLA_CELL_H___

#include <Pixel.h>

namespace DiffusionLimitedAggregation
{
	struct Cell
	{
		ProceduralTextures::UbPixel * m_pixel;
		bool m_whiteNeighbour;
		int m_iValue;
		bool m_white;

		Cell();
		~Cell();

		void Set( ProceduralTextures::UbPixel * p_pixel, bool p_white );
		void SetWhite( double p_dDistance );
		bool IncValue( int p_iMinValue );
	};
}

#endif
