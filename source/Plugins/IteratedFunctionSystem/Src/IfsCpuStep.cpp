#include "IfsCpuStep.h"

#include "FractalLeaf.h"
#include "HeighwayDragon.h"
#include "KochCurve.h"
#include "LevyDragon.h"
#include "McWorterPentigree.h"
#include "Pentadentrite.h"
#include "SierpinskyCarpet.h"
#include "SierpinskyGasket.h"

#include <random>

namespace IteratedFunctionSystem
{
	Thread::Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		: ProceduralTextures::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight )
		, m_nextGeneration( false )
	{
	}

	Thread::~Thread()
	{
	}

	void Thread::Initialise( std::shared_ptr< CellMap > p_pMapCells, std::weak_ptr< IfsGenerator > * p_pGenerator )
	{
		m_mapCells = p_pMapCells;
		m_generator = p_pGenerator;
		m_result = true;
		NextGeneration();
	}

	void Thread::NextGeneration()
	{
		m_itMapCells1 = m_mapCells.lock()->begin();
		m_nextGeneration = false;
	}

	void Thread::DoStep()
	{
		if ( !m_nextGeneration )
		{
			CellMap & l_cells = *m_mapCells.lock();
			IfsGenerator & l_generator = *m_generator->lock();

			if ( m_itMapCells1 != l_cells.end() )
			{
				for ( auto l_itMapCells2 = m_itMapCells1->second.begin() ; ! IsStopped() && l_itMapCells2 != m_itMapCells1->second.end() ; ++l_itMapCells2 )
				{
					m_result |= l_generator.Generate( l_itMapCells2->second, m_mapGeneratedCells );
				}

				m_itMapCells1++;

				if ( m_itMapCells1 == l_cells.end() )
				{
					if ( m_result )
					{
						m_nextGeneration = true;
						m_result = true;
					}
					else
					{
						Stop();
					}
				}
			}
			else
			{
				Stop();
			}
		}
	}

	//*************************************************************************************************

	ProceduralTextures::String CpuStep::ArrayNames[eIFS_GENERATOR_COUNT];

	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::CpuStep< Thread >( p_generator, p_size )
		, m_mapCells( std::make_shared< CellMap >() )
	{
		uint32_t l_width = p_size.x();
		uint32_t l_height = p_size.y();

		for ( int i = 0; i < eIFS_GENERATOR_COUNT; i++ )
		{
			switch ( i )
			{
			case eIFS_GENERATOR_SIERPINSKI_CARPET:
				ArrayNames[i] = _( "Sierpinski Carpet" );
				m_arrayGenerators[i] = std::make_shared< SierpinskiCarpet >( m_finalBuffer, l_width, l_height );
				break;

			case eIFS_GENERATOR_SIERPINSKI_GASKET:
				ArrayNames[i] = _( "Sierpinski Gasket" );
				m_arrayGenerators[i] = std::make_shared< SierpinskiGasket >( m_finalBuffer, l_width, l_height );
				break;

			case eIFS_GENERATOR_FRACTAL_LEAF:
				ArrayNames[i] = _( "Fractal Leaf" );
				m_arrayGenerators[i] = std::make_shared< FractalLeaf >( m_finalBuffer, l_width, l_height );
				break;

			case eIFS_GENERATOR_HEIGHWAY_DRAGON:
				ArrayNames[i] = _( "Heighway Dragon" );
				m_arrayGenerators[i] = std::make_shared< HeighwayDragon >( m_finalBuffer, l_width, l_height );
				break;

			case eIFS_GENERATOR_MCWORTHER_PENTIGREE:
				ArrayNames[i] = _( "Mc Worther Pentigree" );
				m_arrayGenerators[i] = std::make_shared< McWorterPentigree >( m_finalBuffer, l_width, l_height );
				break;

			case eIFS_GENERATOR_PENTADENTRITE:
				ArrayNames[i] = _( "Pentadentrite" );
				m_arrayGenerators[i] = std::make_shared< Pentadentrite >( m_finalBuffer, l_width, l_height );
				break;

			case eIFS_GENERATOR_KOCH_CURVE:
				ArrayNames[i] = _( "Koch Curve" );
				m_arrayGenerators[i] = std::make_shared< KochCurve >( m_finalBuffer, l_width, l_height );
				break;

			case eIFS_GENERATOR_LEVY_DRAGON:
				ArrayNames[i] = _( "Levy Dragon" );
				m_arrayGenerators[i] = std::make_shared< LevyDragon >( m_finalBuffer, l_width, l_height );
				break;
			}
		}

		m_generator = m_arrayGenerators[eIFS_GENERATOR_SIERPINSKI_CARPET];
	}

	CpuStep::~CpuStep()
	{
		m_mapCells->clear();
		m_mapCells.reset();
		m_generator.reset();

		for ( int i = 0; i < eIFS_GENERATOR_COUNT; i++ )
		{
			m_arrayGenerators[i].reset();
		}
	}

	void CpuStep::SetGenerator( eIFS_GENERATOR p_eGenerator )
	{
		Cleanup();
		m_generator = m_arrayGenerators[p_eGenerator];
		Initialise();
	}

	void CpuStep::DoInitialiseStep()
	{
	}

	void CpuStep::DoInitialise()
	{
		m_mapCells->clear();
		std::shared_ptr< IfsGenerator > l_generator = m_generator.lock();
		l_generator->Initialise( m_mapCells );
		m_finalBuffer->Initialise();

		for ( size_t i = 0; i < l_generator->GetInitCellsMapSize(); i++ )
		{
			DoCreateThread( 0, m_sizeImage.y(), m_sizeImage.y() );
		}

		size_t i = 0;

		DoForEachThread( [&i, &l_generator, this]( Thread & l_thread )
		{
			l_thread.Initialise( l_generator->GetInitCellsMap( i ), &m_generator );
			l_thread.Run();
			i++;
		} );

		m_initialised = true;
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
		bool l_nextGeneration = true;

		DoForEachThread( [&l_nextGeneration]( Thread const & l_thread )
		{
			l_nextGeneration &= l_thread.AwaitsNextGeneration();
		} );

		if ( l_nextGeneration )
		{
			std::shared_ptr< IfsGenerator > l_generator = m_generator.lock();

			DoForEachThread( [this, &l_generator]( Thread & l_thread )
			{
				CellMap & l_cells = *l_thread.GetCells();
				CellMap::iterator l_itCells1;
				CellIntMap::iterator l_itCells2;

				for ( auto && l_pair1 : l_thread.GetGeneratedCells() )
				{
					l_itCells1 = l_cells.find( l_pair1.first );

					if ( l_itCells1 == l_cells.end() )
					{
						l_cells.insert( std::make_pair( l_pair1.first, l_pair1.second ) );
					}
					else
					{
						for ( auto & l_pair2 : l_pair1.second )
						{
							l_itCells2 = l_itCells1->second.find( l_pair2.first );

							if ( l_itCells2 == l_itCells1->second.end() )
							{
								l_itCells1->second.insert( std::make_pair( l_pair2.first, l_pair2.second ) );
							}
						}
					}
				}
			} );

			DoForEachThread( [this, &l_generator]( Thread & l_thread )
			{
				l_thread.NextGeneration();
			} );

			l_generator->NextGeneration();
		}
	}
}
