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
#ifndef ___BLG_CELL_H___
#define ___BLG_CELL_H___

#include <GeneratorPrerequisites.h>
#include <Pixel.h>

namespace BuggedLifeGame
{
	class Cell
	{
	private:
		ProceduralTextures::UbPixel * m_alivePx;
		ProceduralTextures::UbPixel * m_deadPx;

	public:
		Cell * m_neighbours[8];
		bool m_alive;
		ProceduralTextures::UbPixel * m_nextPixel;
		ProceduralTextures::UiPixel m_pixel;
		ProceduralTextures::UiPixel m_step;
		ProceduralTextures::UiPixel m_age;
		ProceduralTextures::UiPixel m_med;

	public:
		Cell();
		~Cell();

		void Set( ProceduralTextures::UbPixel * p_pixel, ProceduralTextures::UbPixel * p_alivePx, ProceduralTextures::UbPixel * p_deadPx, bool p_alive );
		void Die();
		void Live();
		void Stay();

		void SetAlive( bool p_alive );

		void Update();

		Cell & Copy( Cell const & p_cell );

	private:
		void _addToPixelRed( uint8_t p_red );
		void _addToPixelGreen( uint8_t p_green );
		void _addToPixelBlue( uint8_t p_blue );
	};

	typedef ProceduralTextures::Buffer< Cell > CellBuffer;
}

#endif
