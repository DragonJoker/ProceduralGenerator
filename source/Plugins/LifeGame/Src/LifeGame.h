/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___LG_LIFE_GAME_H___
#define ___LG_LIFE_GAME_H___

#include <Generator.h>

#include "LgCpuStep.h"
#include "LgGpuStep.h"

namespace LifeGame
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Life game generator
	*/
	class Generator
		: public ProceduralTextures::Generator< CpuStep, GpuStep >
	{
	public:
		/**
		 *\brief		Constructor
		 */
		Generator();
		/**
		 *\brief		Destructor
		 */
		virtual ~Generator();

	private:
		/**
		 *\copydoc		ProceduralTexture::Generator::DoCreate
		 */
		virtual void DoCreate( ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize );
		/**
		 *\copydoc		ProceduralTexture::Generator::DoDestroy
		 */
		virtual void DoDestroy();
		/**
		 *\copydoc		ProceduralTexture::Generator::DoGeneratePanel
		 */
		virtual void DoGeneratePanel();
	};
}

#endif
