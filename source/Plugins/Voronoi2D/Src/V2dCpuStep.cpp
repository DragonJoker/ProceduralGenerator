#include "V2dCpuStep.h"

#include <random>
#include <numeric>

namespace Voronoi2D
{
	Thread::Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		:	ProceduralTextures::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight )
		,	m_maxDepth( 0 )
		,	m_maxNeighbours( 0 )
		,	m_iX( 0 )
		,	m_iY( m_iTop )
		,	m_minkowskiOrder( 4.0 )
		,	m_computed( false )
	{
		m_pFunctions[eDISTANCE_FUNC_EUCLIDIAN] = std::bind( &Thread::EuclidianLength, this, std::placeholders::_1, std::placeholders::_2 );
		m_pFunctions[eDISTANCE_FUNC_SQUARED_EUCLIDIAN] = std::bind( &Thread::SquaredEuclidianLength, this, std::placeholders::_1, std::placeholders::_2 );
		m_pFunctions[eDISTANCE_FUNC_MANHATTAN] = std::bind( &Thread::ManhattanLength, this, std::placeholders::_1, std::placeholders::_2 );
		m_pFunctions[eDISTANCE_FUNC_MINKOWSKI] = std::bind( &Thread::MinkowskiLength, this, std::placeholders::_1, std::placeholders::_2 );
		m_pFunctions[eDISTANCE_FUNC_CHEBYCHEV] = std::bind( &Thread::ChebychevLength, this, std::placeholders::_1, std::placeholders::_2 );
	}

	Thread::~Thread()
	{
	}

	void Thread::DoStep()
	{
		if ( !m_computed )
		{
			if ( m_iX < m_iWidth )
			{
				ProceduralTextures::PixelBuffer & l_buffer = *m_backBuffer.lock();

				for ( int y = m_iTop; !IsStopped() && y < m_iBottom; y++ )
				{
					ComputeColour( m_iX, y, l_buffer[y][m_iX] );
				}

				m_iX++;
			}
			else
			{
				m_computed = true;
			}
		}
	}

	void Thread::Initialise( std::shared_ptr< ProceduralTextures::PixelBuffer > p_pBackBuffer, std::shared_ptr< IntSetIntMap > p_pSeeds, std::shared_ptr< IntIntMapIntMap > p_pHeights, std::shared_ptr< ColourArray > p_pArrayColours, std::shared_ptr< ColourArray > p_pArrayStepColours, eDISTANCE_FUNC p_eTypeDistance, int p_iMaxDepth, int p_iMaxNeighbours, double p_dMkOrder )
	{
		m_backBuffer = p_pBackBuffer;
		m_seeds = p_pSeeds;
		m_heights = p_pHeights;
		m_arrayColours = p_pArrayColours;
		m_arrayStepColours = p_pArrayStepColours;
		m_distanceType = p_eTypeDistance;
		m_maxDepth = p_iMaxDepth;
		m_maxNeighbours = p_iMaxNeighbours;
		m_minkowskiOrder = p_dMkOrder;
		m_computed = false;
	}

	void Thread::ComputeColour( int x, int y, ProceduralTextures::UbPixel & p_px )
	{
		ProceduralTextures::Position l_ptCurrent( x, y );
		IntIntMap::reverse_iterator l_rit;
		std::shared_ptr< IntSetIntMap > l_seeds = m_seeds.lock();
		IntIntMapIntMap & l_heights = *m_heights.lock();

		IntIntMap l_mapHeights = DoComputeDistance( l_ptCurrent, l_seeds, l_heights );
		auto l_it = l_mapHeights.cbegin();
		int l_index = 0;

		while ( l_index++ < m_maxNeighbours )
		{
			++l_it;
		}

		int l_iSum = std::accumulate( l_mapHeights.cbegin(), l_it, int( 0 ), []( int p_init, IntIntMap::value_type const & p_pair )
		{
			return p_init + p_pair.second;
		} );

		if ( !IsStopped() )
		{
			Colour const & l_clr = ( *m_arrayColours.lock() )[l_iSum / m_maxNeighbours];
			p_px.Set( l_clr[0], l_clr[1], l_clr[2], uint8_t( 255 ) );
		}
	}

	int Thread::EuclidianLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB )
	{
		return int( GetDistance( p_ptB, p_ptA ) );
	}

	int Thread::SquaredEuclidianLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB )
	{
		return int( GetSquaredDistance( p_ptB, p_ptA ) );
	}

	int Thread::ManhattanLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB )
	{
		return int( GetManhattanDistance( p_ptB, p_ptA ) );
	}

	int Thread::MinkowskiLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB )
	{
		return int( GetMinkowskiDistance( m_minkowskiOrder, p_ptB, p_ptA ) );
	}

	int Thread::ChebychevLength( ProceduralTextures::Position const & p_ptA, ProceduralTextures::Position const & p_ptB )
	{
		return int( GetChebychevDistance( p_ptB, p_ptA ) );
	}

	IntIntMap Thread::DoComputeDistance( ProceduralTextures::Position const p_position, std::shared_ptr< IntSetIntMap > p_seeds, IntIntMapIntMap & p_heights )
	{
		IntIntMap l_heights;

		for ( auto iti : *p_seeds )
		{
			for ( auto itj : iti.second )
			{
				int l_distance = m_pFunctions[m_distanceType]( p_position, ProceduralTextures::Position( iti.first, itj ) );
				l_heights.insert( std::make_pair( l_distance, p_heights[iti.first][itj] ) );
				++itj;
			}
		}

		return l_heights;
	}

	//*************************************************************************************************

	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::CpuStep< Thread >( p_generator, p_size )
		, m_maxNeighbours( 32 )
		, m_distanceType( eDISTANCE_FUNC_EUCLIDIAN )
		, m_minkowskiOrder( 4.0 )
		, m_seeds( std::make_shared< IntSetIntMap >() )
		, m_heights( std::make_shared< IntIntMapIntMap >() )
		, m_arrayColours( std::make_shared< ColourArray >() )
		, m_arrayStepColours( std::make_shared< ColourArray >() )
	{
		m_arrayStepColours->push_back( Colour( 000, 000, 000 ) );
		m_arrayStepColours->push_back( Colour( 255, 000, 000 ) );
		m_arrayStepColours->push_back( Colour( 255, 255, 000 ) );
		m_arrayStepColours->push_back( Colour( 000, 255, 000 ) );
		m_arrayStepColours->push_back( Colour( 000, 255, 255 ) );
		m_arrayStepColours->push_back( Colour( 000, 000, 255 ) );
		m_arrayStepColours->push_back( Colour( 255, 000, 255 ) );
		m_arrayStepColours->push_back( Colour( 255, 255, 255 ) );
	}

	CpuStep::~CpuStep()
	{
	}

	void CpuStep::SetMaxDepth( int p_iMaxDepth )
	{
		Cleanup();
		m_maxDepth = p_iMaxDepth;
		m_arrayColours->clear();
		int l_iNbColours = int( m_maxDepth * ( m_arrayStepColours->size() - 1 ) );
		m_arrayColours->reserve( l_iNbColours );
		int l_iSteps = m_maxDepth - 1;
		int l_iCurrent = 0, l_iPrevious;
		Colour l_clrColour;
		ProceduralTextures::Point3d l_ptStep;
		ProceduralTextures::Point3d l_ptColour;

		for ( int i = 0; i < l_iNbColours; )
		{
			l_iPrevious = l_iCurrent++;
			l_clrColour = ( *m_arrayStepColours )[l_iPrevious];
			l_ptColour = ( *m_arrayStepColours )[l_iPrevious];
			l_ptStep = ( ProceduralTextures::Point3d( ( *m_arrayStepColours )[l_iCurrent] ) - l_ptColour ) / l_iSteps;

			for ( int j = 0; j <= l_iSteps; j++, i++ )
			{
				m_arrayColours->push_back( l_ptColour );
				l_ptColour += l_ptStep;
			}
		}

		Generate();
	}

	void CpuStep::SetMaxNeighbours( int p_iMaxNeighbours )
	{
		Cleanup();
		m_maxNeighbours = p_iMaxNeighbours;
		Generate();
	}

	void CpuStep::SetTypeDistance( eDISTANCE_FUNC p_eType )
	{
		Cleanup();
		m_distanceType = p_eType;
		Generate();
	}

	void CpuStep::SetMinkowskiOrder( double p_dMkOrder )
	{
		Cleanup();
		m_minkowskiOrder = p_dMkOrder;
		Generate();
	}

	void CpuStep::Generate()
	{
		m_finalBuffer->Initialise();
		int l_iOffset = int( m_sizeImage.y() / m_threadsCount );
		int l_iAdd;

		if ( int( l_iOffset * m_threadsCount ) == m_sizeImage.y() )
		{
			l_iAdd = 0;
		}
		else
		{
			l_iAdd = m_sizeImage.y() - int( l_iOffset * m_threadsCount );
		}

		for ( uint32_t i = 0; i < m_sizeImage.y(); i += l_iOffset )
		{
			DoCreateThread( i, i + l_iOffset + l_iAdd, m_sizeImage.y() );
			i += l_iAdd;
			l_iAdd = 0;
		}

		DoForEachThread( [this]( Thread & l_thread )
		{
			l_thread.Initialise( m_finalBuffer, m_seeds, m_heights, m_arrayColours, m_arrayStepColours, m_distanceType, m_maxDepth, m_maxNeighbours, m_minkowskiOrder );
			l_thread.Run();
		} );

		m_initialised = true;
	}

	void CpuStep::DoInitialiseStep()
	{
	}

	void CpuStep::DoInitialise()
	{
		SetMaxDepth( 5 );
		m_seeds->clear();
		m_heights->clear();
		int l_iNbSeedsX = int( sqrt( float( m_sizeImage.x() ) ) );
		int l_iNbSeedsY = int( sqrt( float( m_sizeImage.y() ) ) );
		int x, y;
		int l_iIndex;
		IntSetIntMap::iterator l_itSeeds1;
		IntSet::iterator l_itSeeds2;
		IntIntMapIntMap::iterator l_itHeights1;
		std::random_device l_randDevice;
		std::default_random_engine l_randEngine( l_randDevice() );
		std::uniform_int_distribution< int > l_randDistributionX( 0, m_sizeImage.x() - 1 );
		std::uniform_int_distribution< int > l_randDistributionY( 0, m_sizeImage.y() - 1 );
		std::uniform_int_distribution< int > l_randDistributionC( 0, int( m_arrayColours->size() - 1 ) );

		for ( int i = 0; i < l_iNbSeedsX; i++ )
		{
			for ( int j = 0; j < l_iNbSeedsY; j++ )
			{
				x = l_randDistributionX( l_randEngine );
				l_itSeeds1 = m_seeds->find( x );
				l_itHeights1 = m_heights->find( x );

				if ( m_seeds->size() < m_sizeImage.x() )
				{
					while ( l_itSeeds1 != m_seeds->end() )
					{
						x = l_randDistributionX( l_randEngine );
						l_itSeeds1 = m_seeds->find( x );
					}

					m_seeds->insert( IntSetIntMap::value_type( x, IntSet() ) );
					m_heights->insert( IntIntMapIntMap::value_type( x, IntIntMap() ) );
				}

				l_itSeeds1 = m_seeds->find( x );
				l_itHeights1 = m_heights->find( x );

				y = l_randDistributionY( l_randEngine );
				l_itSeeds2 = l_itSeeds1->second.find( y );

				if ( l_itSeeds1->second.size() < m_sizeImage.y() )
				{
					while ( l_itSeeds2 != l_itSeeds1->second.end() )
					{
						y = l_randDistributionY( l_randEngine );
						l_itSeeds2 = l_itSeeds1->second.find( y );
					}

					l_itSeeds1->second.insert( y );
				}

				l_iIndex = l_randDistributionC( l_randEngine );
				l_itHeights1->second.insert( IntIntMap::value_type( y, l_iIndex ) );
			}
		}
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
	}
}
