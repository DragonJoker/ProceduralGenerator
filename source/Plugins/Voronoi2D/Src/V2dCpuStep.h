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
#ifndef ___V2D_VORONOI_2D_H___
#define ___V2D_VORONOI_2D_H___

#include <CpuStep.h>
#include <Position.h>

namespace Voronoi2D
{
	typedef ProceduralTextures::Point< uint8_t, 3 > Colour;
	typedef std::vector< Colour > ColourArray;
	typedef std::set< int >	IntSet;
	typedef std::map< int, int > IntIntMap;
	typedef std::map< int, IntSet > IntSetIntMap;
	typedef std::map< int, IntIntMap > IntIntMapIntMap;
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		The distance function
	*/
	typedef enum
	{
		eDISTANCE_FUNC_EUCLIDIAN,
		eDISTANCE_FUNC_SQUARED_EUCLIDIAN,
		eDISTANCE_FUNC_MANHATTAN,
		eDISTANCE_FUNC_MINKOWSKI,
		eDISTANCE_FUNC_CHEBYCHEV,
		eDISTANCE_FUNC_COUNT,
	}	eDISTANCE_FUNC;
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Voronoi 2D worker thread
	*/
	class Thread
		: public ProceduralTextures::Thread
	{
	private:
		typedef std::function< int ( ProceduralTextures::Position const &, ProceduralTextures::Position const & ) > DistanceFunction;

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
		 *\brief		Sets the buffers, seeds, heights and colours
		 *\param[in]	p_pBackBuffer		The buffer
		 *\param[in]	p_pSeeds			The seeds
		 *\param[in]	p_pHeights			The heights
		 *\param[in]	p_pArrayColours		The The colours
		 *\param[in]	p_eTypeDistance		The distance function
		 *\param[in]	p_iMaxDepth			The maximum depth
		 *\param[in]	p_iMaxNeighbours	The maximum neighbours
		 *\param[in]	p_dMkOrder			The minkowski distance function order
		 */
		void Initialise( std::shared_ptr< ProceduralTextures::PixelBuffer > p_pBackBuffer, std::shared_ptr< IntSetIntMap > p_pSeeds, std::shared_ptr< IntIntMapIntMap > p_pHeights, std::shared_ptr< ColourArray > p_pArrayColours, std::shared_ptr< ColourArray > p_pArrayStepColours, eDISTANCE_FUNC p_eTypeDistance, int p_iMaxDepth, int p_iMaxNeighbours, double p_dMkOrder );
		/**
		 *\brief		Computes a colour for given position
		 *\param[in]	x, y	The position
		 *\param[out]	p_px	Receives the position
		 */
		void ComputeColour( int x, int y, ProceduralTextures::UbPixel & p_px );
		/**
		 *\brief		Retrieves the Euclidian length between two points
		 *\param[in]	p_ptA, p_ptB	The points
		 *\return		The length
		 */
		int EuclidianLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB );
		/**
		 *\brief		Retrieves the squared Euclidian length between two points
		 *\param[in]	p_ptA, p_ptB	The points
		 *\return		The length
		 */
		int SquaredEuclidianLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB );
		/**
		 *\brief		Retrieves the Manhattan length between two points
		 *\param[in]	p_ptA, p_ptB	The points
		 *\return		The length
		 */
		int ManhattanLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB );
		/**
		 *\brief		Retrieves the Minkowski length between two points
		 *\param[in]	p_ptA, p_ptB	The points
		 *\return		The length
		 */
		int MinkowskiLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB );
		/**
		 *\brief		Retrieves the Chebychev length between two points
		 *\param[in]	p_ptA, p_ptB	The points
		 *\return		The length
		 */
		int ChebychevLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB );

	private:
		/**
		 *\copydoc		ProceduralTexture::CpuStep::Thread::DoStep
		 */
		virtual void DoStep();
		/**
		 *\brief		Retrieves the distance between each seed and the given position
		 *\param[in]	p_position	The position
		 *\param[in]	p_seeds		The seeds
		 *\param[in]	p_heights	The heights
		 *\return		The lengths
		 */
		IntIntMap DoComputeDistance( ProceduralTextures::Position const p_position, std::shared_ptr< IntSetIntMap > p_seeds, IntIntMapIntMap & p_heights );

	private:
		//! The heights
		IntIntMap m_mapHeights;
		//! The back buffer
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_backBuffer;
		//! The seeds
		std::weak_ptr< IntSetIntMap > m_seeds;
		//! The heights
		std::weak_ptr< IntIntMapIntMap > m_heights;
		//! The colours
		std::weak_ptr< ColourArray > m_arrayColours;
		//! The colour steps
		std::weak_ptr< ColourArray > m_arrayStepColours;
		//! The distance function
		eDISTANCE_FUNC m_distanceType;
		//! The maximum depth
		int m_maxDepth;
		//! The maximum neighbours involved in colour computing
		int m_maxNeighbours;
		//! The current X
		int m_iX;
		//! The current Y
		int m_iY;
		//! The Minkowski distance function order
		double m_minkowskiOrder;
		//! The distance functions
		DistanceFunction m_pFunctions[eDISTANCE_FUNC_COUNT];
		//! Tells the workers job is ended
		bool m_computed;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Voronoi 2D CPU step
	*/
	class CpuStep
		:	public ProceduralTextures::CpuStep< Thread >
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
		 *\brief		Sets the maximum depth
		 *\param[in]	p_value	The new value
		 */
		void SetMaxDepth( int p_value );
		/**
		 *\brief		Sets the maximum neighbours
		 *\param[in]	p_value	The new value
		 */
		void SetMaxNeighbours( int p_value );
		/**
		 *\brief		Sets the distance function
		 *\param[in]	p_value	The new value
		 */
		void SetTypeDistance( eDISTANCE_FUNC p_value );
		/**
		 *\brief		Sets the Minkowski function order
		 *\param[in]	p_value	The new value
		 */
		void SetMinkowskiOrder( double p_value );
		/**
		 *\brief		Generates the threads
		 */
		void Generate();
		/**
		 *\brief		Retrieves the maximum depth
		 *\return		The value
		 */
		inline int GetMaxDepth()const
		{
			return m_maxDepth;
		}
		/**
		 *\brief		Retrieves the maximum depth
		 *\return		The value
		 */
		inline int GetMaxNeighbours()const
		{
			return m_maxNeighbours;
		}
		/**
		 *\brief		Retrieves the distance function
		 *\return		The value
		 */
		inline eDISTANCE_FUNC GetTypeDistance()const
		{
			return m_distanceType;
		}
		/**
		 *\brief		Retrieves the Minkowski function order
		 *\return		The value
		 */
		inline double GetMinkowskiOrder()const
		{
			return m_minkowskiOrder;
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

	private:
		//! The seeds
		std::shared_ptr< IntSetIntMap > m_seeds;
		//! The seeds
		std::shared_ptr< IntIntMapIntMap > m_heights;
		//! The maximum depth
		int m_maxDepth;
		//! The maximum neighbours involved in clour computing
		int m_maxNeighbours;
		//! The colours
		std::shared_ptr< ColourArray > m_arrayColours;
		//! The colours steps
		std::shared_ptr< ColourArray > m_arrayStepColours;
		//! The distance function
		eDISTANCE_FUNC m_distanceType;
		//! The Minkowski distance function order
		double m_minkowskiOrder;
	};
}

#endif
