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
#ifndef ___LG_PLUGIN_H___
#define ___LG_PLUGIN_H___

#include "LifeGame.h"

#include <Plugin.h>

namespace ProceduralTextures
{
	class LgPlugin
		:	public Plugin< LifeGame, LgPlugin >
	{
	public:
		LgPlugin();
		virtual ~LgPlugin();

		virtual wxString GetName();

		DECLARE_PLUGIN()
	};
}

#endif
