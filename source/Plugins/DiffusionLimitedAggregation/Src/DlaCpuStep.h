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
#ifndef ___DLA_CPU_STEP_H___
#define ___DLA_CPU_STEP_H___

#include "DlaCell.h"

#include <random>

#include <Plugin.h>
#include <CpuStep.h>

namespace DiffusionLimitedAggregation
{
	typedef ProceduralTextures::Buffer< Cell > CellBuffer;
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Diffusion limited aggregation worker thread
	*/
	class Thread
		: public ProceduralTextures::Thread
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_parent	The parent generator
		 *\param[in]	p_index		The thread index
		 *\param[in]	p_width		The surface width
		 *\param[in]	p_top		The surface portion's top
		 *\param[in]	p_bottom	The surface portion's bottom
		 *\param[in]	p_height	The surface height
		 */
		Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_parent, size_t p_index, int p_width, int p_top, int p_bottom, int p_totalHeight );
		/**
		 *\brief		Destructor
		 */
		virtual ~Thread();
		/**
		 *\brief		Sets the cells buffers
		 *\param[in]	p_cells		The current cells
		 */
		inline void SetBuffer( std::shared_ptr< CellBuffer > p_cells )
		{
			m_cells = p_cells;
		}

	private:
		/**
		 *\copydoc		ProceduralTexture::CpuStep::Thread::DoStep
		 */
		virtual void DoStep();
		/**
		 *\brief		Generates a point between jump and kill circles
		 *\param[out]	p_position	Receives the generated point
		 *\param[out]	p_origin	The center of both circles
		 *\param[out]	p_radius	The minimal radius
		 */
		void DoGetPointOnCircle( ProceduralTextures::Position & p_position, ProceduralTextures::Position const & p_origin, float p_radius );
		/**
		 *\brief		Increments the value of the cell at given position
		 *\param[out]	p_position	The cell's position
		 */
		void DoSetCellWhite( ProceduralTextures::Position const & p_position );

	public:
		//! The cells
		std::weak_ptr< CellBuffer > m_cells;
		//! A table to know where the walker has already gone
		std::vector < bool > m_passed;
		//! When the walker goes out of this circle, it is moved inside the bounding circle
		int m_killCircle;
		//! When the walker goes out of this circle, it is moved around his position
		int m_jumpCircle;
		//! The 'white' cells bounding circle
		int m_boundingCircle;
		//! The circles' origin
		ProceduralTextures::Position m_origin;
		//! The minimal value a cell must have to be considered 'white'
		int m_minValue;
		//! The actual white cells count
		std::atomic_int * m_currentMass;
		//! The actual white cells count
		std::atomic_int * m_mass;
		//! The distance used to compute a 'white' cell colour
		double m_maxDistance;
		//! The random numbers generator device
		std::random_device m_randDevice;
		//! The random numbers generator engine
		std::default_random_engine m_randEngine;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Diffusion limited aggregation CPU step
	*/
	class CpuStep
		: public ProceduralTextures::CpuStep< Thread >
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_generator	The parent generator
		 *\param[in]	p_size		The dimensions
		 */
		CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size );
		/**
		 *\brief		Destructor
		 */
		virtual ~CpuStep();
		/**
		 *\brief		Retrieves the minimal value a cell must have to be considered 'white'
		 *\return		The value
		 */
		inline int GetMinValue()const
		{
			return m_minValue;
		}
		/**
		 *\brief		Sets the minimal value a cell must have to be considered 'white'
		 *\param[in]	val	The new value
		 */
		inline void SetMinValue( int val )
		{
			m_minValue = val;
		}
		/**
		 *\brief		Retrieves 'white' cells count at which the generation stops
		 *\return		The value
		 */
		inline int GetMass()const
		{
			return m_mass;
		}
		/**
		 *\brief		Sets the 'white' cells count at which the generation stops
		 *\param[in]	val	The new value
		 */
		inline void SetMass( int val )
		{
			m_mass = val;
		}

	private:
		/**
		 *\copydoc		ProceduralTexture::CpuStep::DoInitialiseStep
		 */
		virtual void DoInitialiseStep();
		/**
		 *\copydoc		ProceduralTexture::CpuStep::DoInitialise
		 */
		virtual void DoInitialise();
		/**
		 *\copydoc		ProceduralTexture::CpuStep::DoCleanup
		 */
		virtual void DoCleanup();
		/**
		 *\copydoc		ProceduralTexture::CpuStep::DoSwapBuffers
		 */
		virtual void DoSwapBuffers();

	public:
		//! The cells
		std::shared_ptr< CellBuffer > m_cells;
		//! When the walker goes out of this circle, it is moved inside the bounding circle
		int m_killCircle;
		//! When the walker goes out of this circle, it is moved around his position
		int m_jumpCircle;
		//! The 'white' cells bounding circle
		int m_boundingCircle;
		//! The circles' origin
		ProceduralTextures::Position m_origin;
		//! The minimal value a cell must have to be considered 'white'
		int m_minValue;
		//! The 'white' cells count at which the generation stops
		std::atomic_int m_mass;
		//! The current 'white' cells count
		std::atomic_int m_currentMass;
		//! The distance used to compute a 'white' cell colour
		double m_maxDistance;
	};
}

#endif
