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
#ifndef ___IFS_CPU_STEP_H___
#define ___IFS_CPU_STEP_H___

#include "IfsCell.h"
#include "IfsGenerator.h"

#include <CpuStep.h>

#include <map>
#include <list>
#include <vector>

namespace IteratedFunctionSystem
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Iterated function system worker thread
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
		/** Initialises the step thread, setting its cells, buffer, and generator
		@param[in] p_mapCells
			The cells map
		@param[in] p_backBuffer
			The buffer
		@param[in] p_generator
			The IFS generator
		 */
		void Initialise( std::shared_ptr< CellMap > p_mapCells, std::weak_ptr< IfsGenerator > * p_generator );
		/** Push the thread into the next generation (yeah!)
		 */
		void NextGeneration();
		/** Tells if the thread waits for a new generation
		@return
				The status
		 */
		inline bool AwaitsNextGeneration()const
		{
			return m_nextGeneration;
		}
		/** Retrieves the cells on whiche the thread is working
		@return
				The status
		 */
		inline std::shared_ptr< CellMap > GetCells()const
		{
			return m_mapCells.lock();
		}
		/** Retrieve the thread's generated cells, empties it
		@return
				The cellss
		 */
		inline CellMap GetGeneratedCells()
		{
			CellMap l_return( m_mapGeneratedCells );
			m_mapGeneratedCells.clear();
			return l_return;
		}

	private:
		/** @copydoc ProceduralTexture::CpuStep::Thread::DoStep
		 */
		virtual void DoStep();

	private:
		//! The cells map
		std::weak_ptr< CellMap > m_mapCells;
		//! The generated cells map
		CellMap m_mapGeneratedCells;
		//! The current iterator on the cells map
		CellMap::iterator m_itMapCells1;
		//! The IFS generator
		std::weak_ptr< IfsGenerator > * m_generator;
		//! Tells if the step has modified at least one cell
		bool m_result;
		//! Tells that the step has ended this generation and waits for a new one
		bool m_nextGeneration;
	};
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Iterated function system CPU step
	*/
	class CpuStep
		: public ProceduralTextures::CpuStep< Thread >
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The dimensions
		 */
		CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size );
		/** Destructor
		 */
		virtual ~CpuStep();
		/** Retrieves the generator size
		@return
				The value
		 */
		int GetGeneratorShift()const
		{
			return m_generator.lock()->GetShift();
		}
		/** Retrieves the generator angle
		@return
				The value
		 */
		int GetGeneratorAngle()const
		{
			return m_generator.lock()->GetAngle();
		}
		/** Retrieves the generator shift
		@return
				The value
		 */
		int GetGeneratorSize()const
		{
			return m_generator.lock()->GetSize();
		}
		/** Sets the generator size
		@param[in] val
			The new value
		 */
		void SetGeneratorSize( int val )
		{
			m_generator.lock()->SetSize( val );
		}
		/** Sets the generator angle
		@param[in] val
			The new value
		 */
		void SetGeneratorAngle( int val )
		{
			m_generator.lock()->SetAngle( val );
		}
		/** Sets the generator shift
		@param[in] val
			The new value
		 */
		void SetGeneratorShift( int val )
		{
			m_generator.lock()->SetShift( val );
		}

		void SetGenerator( eIFS_GENERATOR p_eGenerator );

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

	public:
		//! The generators names
		static ProceduralTextures::String ArrayNames[eIFS_GENERATOR_COUNT];

	private:
		//! The cells map
		std::shared_ptr< CellMap > m_mapCells;
		//! The selected IFS generator
		std::weak_ptr< IfsGenerator > m_generator;
		//! The IFS generators array
		std::shared_ptr< IfsGenerator > m_arrayGenerators[eIFS_GENERATOR_COUNT];
	};
}

#endif
