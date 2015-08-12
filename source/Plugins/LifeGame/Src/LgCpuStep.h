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
#ifndef ___LG_CPU_STEP_H___
#define ___LG_CPU_STEP_H___

#include "LgCell.h"

#include <CpuStep.h>

namespace LifeGame
{
	typedef ProceduralTextures::Buffer< Cell > CellBuffer;
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Life game worker thread
	*/
	class Thread
		: public ProceduralTextures::Thread
	{
	public:
		/** Constructor
		@param[in] p_parent
			The parent generator
		@param[in] p_index
			The thread index
		@param[in] p_width
			The surface width
		@param[in] p_top
			The surface portion's top
		@param[in] p_bottom
			The surface portion's bottom
		@param[in] p_height
			The surface height
		 */
		Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_parent, size_t p_index, int p_width, int p_top, int p_bottom, int p_totalHeight );
		/** Destructor
		 */
		virtual ~Thread();
		/** Initialises the cells buffers
		@param[in] p_cells
			The current cells
		@param[in] p_oldCells
			The previous cells
		 */
		inline void SetCells( std::shared_ptr< CellBuffer > p_cells, std::shared_ptr< CellBuffer > p_oldCells )
		{
			m_cells = p_cells;
			m_oldCells = p_oldCells;
		}

	private:
		/** @copydoc ProceduralTexture::CpuStep::Thread::DoStep
		 */
		virtual void DoStep();

	private:
		//! The current cells
		std::weak_ptr< CellBuffer > m_cells;
		//! The previous cells
		std::weak_ptr< CellBuffer > m_oldCells;
	};
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Life game CPU step
	*/
	class CpuStep
		: public ProceduralTextures::CpuStep< Thread >
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
				The computing surface dimensions
		 */
		CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size );
		/** Destructor
		 */
		virtual ~CpuStep();

	private:
		/** @copydoc ProceduralTexture::CpuStep::DoInitialiseStep
		 */
		virtual void DoInitialiseStep();
		/** @copydoc ProceduralTexture::CpuStep::DoInitialise
		 */
		virtual void DoInitialise();
		/** @copydoc ProceduralTexture::CpuStep::DoCleanup
		 */
		virtual void DoCleanup();
		/** @copydoc ProceduralTexture::CpuStep::DoSwapBuffers
		 */
		virtual void DoSwapBuffers();

		/** Initialises the neighbours for given cell
		@param[in] p_x,
			p_y	The cell coordinates
		 */
		void DoInitialiseNeighbours( uint32_t p_x, uint32_t p_y );

	private:
		//! The previous back buffer
		std::shared_ptr< ProceduralTextures::PixelBuffer > m_previousBuffer;
		//! The current cells
		std::shared_ptr< CellBuffer > m_cells;
		//! The previous cells
		std::shared_ptr< CellBuffer > m_oldCells;
	};
}

#endif
