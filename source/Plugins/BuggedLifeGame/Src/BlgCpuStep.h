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
#ifndef ___BLG_CPU_STEP_H___
#define ___BLG_CPU_STEP_H___

#include "BlgCell.h"

#include <Buffer.h>
#include <CpuStep.h>

namespace BuggedLifeGame
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Bugged life game worker thread
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

		/** Sets the cells buffer
		@param[in] p_buffer
			The new value
		 */
		inline void SetBuffer( std::shared_ptr< CellBuffer > p_buffer )
		{
			m_cells = p_buffer;
		}

		/** Sets the red value of the pixel
		@param[in] val
			The new value
		 */
		inline void SetRed( uint8_t val )
		{
			m_pxColour.r = val;
		}

		/** Sets the green value of the pixel
		@param[in] val
			The new value
		 */
		inline void SetGreen( uint8_t val )
		{
			m_pxColour.g = val;
		}

		/** Sets the blue value of the pixel
		@param[in] val
			The new value
		 */
		inline void SetBlue( uint8_t val )
		{
			m_pxColour.b = val;
		}

	private:
		/** @copydoc ProceduralTexture::CpuStep::Thread::DoStep
		 */
		virtual void DoStep();

	private:
		//! The cells
		std::weak_ptr< CellBuffer > m_cells;
		//! The pixel colour (image is in computed in white and m_pxColour )
		ProceduralTextures::UbPixel m_pxColour;
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Bugged life game CPU step
	*/
	class CpuStep
		: public ProceduralTextures::CpuStep< Thread >
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The computing image size
		 */
		CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size );

		/** Destructor
		 */
		virtual ~CpuStep();

		/** Sets the red value of the pixel
		@param[in] val
			The new value
		 */
		void SetRed( int val );

		/** Sets the green value of the pixel
		@param[in] val
			The new value
		 */
		void SetGreen( int val );

		/** Sets the blue value of the pixel
		@param[in] val
			The new value
		 */
		void SetBlue( int val );

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
		//! The alive pixel colour
		ProceduralTextures::UbPixel m_alivePx;
		//! The dead pixel colour
		ProceduralTextures::UbPixel m_deadPx;
		//! The cells buffer
		std::shared_ptr< CellBuffer > m_cells;
	};
}

#endif
