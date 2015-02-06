/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___IFS_CELL_H___
#define ___IFS_CELL_H___

#include <Pixel.h>
#include <Point.h>

namespace ProceduralTextures
{
	struct IfsCell
	{
		UbPixel * m_pixel;
		Point2d m_ptPosition;

		IfsCell();
		IfsCell( const IfsCell & cell );
		virtual ~IfsCell();

		IfsCell & operator =( const IfsCell & cell );

		void Set( UbPixel * p_pixel, const Point2d & p_ptPosition );
	};

	typedef std::map < int, IfsCell > IfsCellIntMap;
	typedef std::map < int, IfsCellIntMap > IfsCellMap;
	typedef std::list < IfsCell > IfsCellLst;
	typedef std::vector < IfsCell > IfsCellVct;
}

#endif
