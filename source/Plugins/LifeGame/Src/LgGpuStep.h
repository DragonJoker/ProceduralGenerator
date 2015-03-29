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
#ifndef ___LG_GPU_STEP_H___
#define ___LG_GPU_STEP_H___

#include <GpuStep.h>

namespace LifeGame
{
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Life game GPU step (dummy)
	*/
	class GpuStep
		: public ProceduralTextures::GpuStep
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_generator		The parent generator
		 *\param[in]	p_size			The displayed surface dimensions
		 *\param[in]	p_bordersSize	The windows' borders size
		 */
		GpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize );
		/**
		 *\brief		Destructor
		 */
		virtual ~GpuStep();

	private:
		/**
		 *\copydoc		ProceduralTexture::GpuStep::DoInitialise
		 */
		virtual void DoInitialise();
		/**
		 *\copydoc		ProceduralTexture::GpuStep::DoCleanup
		 */
		virtual void DoCleanup();
		/**
		 *\copydoc		ProceduralTexture::GpuStep::DoResize
		 */
		virtual void DoResize();
		/**
		 *\copydoc		ProceduralTexture::GpuStep::DoPreRender
		 */
		virtual void DoPreRender();
		/**
		 *\copydoc		ProceduralTexture::GpuStep::DoRender
		 */
		virtual void DoRender( bool & p_bChanged );
		/**
		 *\copydoc		ProceduralTexture::GpuStep::DoPostRender
		 */
		virtual void DoPostRender();
	};
}

#endif
