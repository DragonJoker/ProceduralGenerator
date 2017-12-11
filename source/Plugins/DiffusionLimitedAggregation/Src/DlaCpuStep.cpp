#include "DlaCpuStep.h"

#include <random>

using namespace ProceduralTextures;

namespace DiffusionLimitedAggregation
{
	const double pi = 3.14159265358979323846;

	Thread::Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		: ProceduralTextures::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight )
		, m_passed( m_iWidth * m_iHeight )
	{
		srand( int( time( NULL ) ) );
	}

	Thread::~Thread()
	{
	}

	void Thread::DoStep()
	{
		if ( *m_mass > *m_currentMass )
		{
			Position l_ptPoint;
			Position l_ptTmp;
			std::set < int > l_setTries;
			DoGetPointOnCircle( l_ptPoint, m_origin, float( m_boundingCircle + 5 ) );
			m_passed.assign( m_iWidth * m_iHeight, false );
			bool l_alreadyPassedWhite = false;
			int l_iDistance = int( GetSquaredDistance( l_ptPoint, m_origin ) );
			int l_iSquaredKillCircle = m_killCircle * m_killCircle;
			int l_iSquaredJumpCircle =  m_jumpCircle * m_jumpCircle;
			CellBuffer & l_cells = *m_cells.lock();

			for ( int i = 1000; i != 0 && !l_alreadyPassedWhite && !l_cells[l_ptPoint.y()][l_ptPoint.x()].m_white; i-- )
			{
				while ( l_iDistance > l_iSquaredJumpCircle )
				{
					if ( l_iDistance > l_iSquaredKillCircle )
					{
						DoGetPointOnCircle( l_ptPoint, m_origin, float( m_boundingCircle + 5 ) );
						l_iDistance = int( GetSquaredDistance( l_ptPoint, m_origin ) );
					}
					else
					{
						DoGetPointOnCircle( l_ptPoint, l_ptPoint, sqrt( float( l_iDistance ) ) - m_boundingCircle );
						l_iDistance = int( GetSquaredDistance( l_ptPoint, m_origin ) );
					}
				}

				Cell & l_cell = l_cells[l_ptPoint.y()][l_ptPoint.x()];

				if ( l_cell.m_whiteNeighbour )
				{
					if ( l_cell.IncValue( m_minValue ) )
					{
						l_alreadyPassedWhite = true;
						m_boundingCircle = std::max( m_boundingCircle, int( sqrt( ( float )l_iDistance ) ) );

						if ( m_boundingCircle == 1 )
						{
							m_boundingCircle = 2;
						}

						DoSetCellWhite( l_ptPoint );
						( *m_currentMass )++;
					}
				}
				else
				{
					l_setTries.clear();

					do
					{
						l_ptTmp = l_ptPoint;
						int l_direction = rand() % 4;

						if ( l_direction == 0 )
						{
							if ( l_ptTmp.x() > 0 )
							{
								l_ptTmp.x()--;
							}
						}
						else if ( l_direction == 1 )
						{
							if ( l_ptTmp.x() < m_iWidth - 1 )
							{
								l_ptTmp.x()++;
							}
						}
						else if ( l_direction == 2 )
						{
							if ( l_ptTmp.y() > 0 )
							{
								l_ptTmp.y()--;
							}
						}
						else if ( l_direction == 3 )
						{
							if ( l_ptTmp.y() < m_iHeight - 1 )
							{
								l_ptTmp.y()++;
							}
						}

						l_setTries.insert( l_direction );
					}
					while ( m_passed[l_ptTmp.x() * m_iHeight + l_ptTmp.y()] && l_setTries.size() < 4 );

					m_passed[l_ptTmp.x() * m_iHeight + l_ptTmp.y()] = true;

					if ( l_setTries.size() == 4 )
					{
						l_alreadyPassedWhite = true;
					}
					else
					{
						l_ptPoint = l_ptTmp;
						l_iDistance = int( GetSquaredDistance( l_ptPoint, m_origin ) );
					}
				}
			}
		}
	}

	void Thread::DoGetPointOnCircle( Position & p_position, Position const & p_origin, float p_fRadius )
	{
		int l_iAngle;
		float l_fRadius;
		int l_iCount = 1000;
		CellBuffer & l_cells = *m_cells.lock();

		do
		{
			l_iAngle = rand() % 360;
			l_fRadius = ( 3 * p_fRadius / 4 ) + rand() % std::max< int >( 1, int( p_fRadius / 4 ) );
			l_fRadius += 2;
			p_position.x() = p_origin.x() + int( sin( l_iAngle * pi / 180.0 ) * l_fRadius );
			p_position.y() = p_origin.y() + int( cos( l_iAngle * pi / 180.0 ) * l_fRadius );
			l_iCount--;
		}
		while ( l_iCount != 0 && ( p_position.x() >= m_iWidth || p_position.y() >= m_iHeight || p_position.x() < 0 || p_position.y() < 0 || l_cells[p_position.y()][p_position.x()].m_white ) );
	}

	void Thread::DoSetCellWhite( Position const & p_ptPoint )
	{
		CellBuffer & l_cells = *m_cells.lock();
		int x = p_ptPoint.x();
		int y = p_ptPoint.y();
		l_cells[y][x].SetWhite( GetDistance( p_ptPoint, m_origin ) / m_maxDistance );

		if ( x > 0 )
		{
			l_cells[y][( x - 1 )].m_whiteNeighbour = true;
		}

		if ( y > 0 )
		{
			l_cells[( y - 1 )][x].m_whiteNeighbour = true;
		}

		if ( x < m_iWidth - 1 )
		{
			l_cells[y][( x + 1 )].m_whiteNeighbour = true;
		}

		if ( y < m_iHeight - 1 )
		{
			l_cells[( y + 1 )][x].m_whiteNeighbour = true;
		}
	}

	//*************************************************************************************************

	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, Size const & p_size )
		: ProceduralTextures::CpuStep< Thread >( p_generator, p_size )
		, m_boundingCircle( 2 )
		, m_minValue( 1 )
		, m_origin( p_size.x() / 2, p_size.y() / 2 )
		, m_cells( std::make_shared< CellBuffer >( p_size ) )
	{
		m_mass = 10000;
		m_currentMass = 0;
		m_maxDistance = GetDistance( m_origin );
		m_jumpCircle = m_boundingCircle << 1;
		m_killCircle = m_boundingCircle << 2;
		uint32_t l_width = p_size.x();
		uint32_t l_height = p_size.y();
		CellBuffer & l_cells = *m_cells;

		for ( uint32_t y = 0; y < l_height; y++ )
		{
			for ( uint32_t x = 0; x < l_width; x++ )
			{
				l_cells[y][x].Set( &( *m_finalBuffer )[y][x], false );
			}
		}

		uint32_t x = m_origin.x();
		uint32_t y = m_origin.y();
		l_cells[y][x].SetWhite( 0.0 );
		l_cells[y][( x - 1 )].m_whiteNeighbour = true;
		l_cells[( y - 1 )][x].m_whiteNeighbour = true;
		l_cells[y][( x + 1 )].m_whiteNeighbour = true;
		l_cells[( y + 1 )][x].m_whiteNeighbour = true;
	}

	CpuStep::~CpuStep()
	{
	}

	void CpuStep::DoInitialiseStep()
	{
	}

	void CpuStep::DoInitialise()
	{
		uint32_t l_width = m_sizeImage.x();
		uint32_t l_height = m_sizeImage.y();
		int l_iOffset = int( l_height / m_threadsCount );

		for ( uint32_t i = 0; i < l_height; i += l_iOffset )
		{
			DoCreateThread( 0, l_height, l_height );
		}

		DoForEachThread( [this]( Thread & l_thread )
		{
			l_thread.SetBuffer( m_cells );
			l_thread.m_minValue = m_minValue;
			l_thread.m_maxDistance = m_maxDistance;
			l_thread.m_origin = m_origin;
			l_thread.m_boundingCircle = m_boundingCircle;
			l_thread.m_killCircle = m_killCircle;
			l_thread.m_jumpCircle = m_jumpCircle;
			l_thread.m_currentMass = &m_currentMass;
			l_thread.m_mass = &m_mass;
			l_thread.Run();
		} );
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
		DoForEachThread( [this]( Thread & l_thread )
		{
			m_boundingCircle = std::max( m_boundingCircle, l_thread.m_boundingCircle );
		} );

		m_jumpCircle = m_boundingCircle << 1;
		m_killCircle = m_boundingCircle << 2;

		DoForEachThread( [this]( Thread & l_thread )
		{
			l_thread.m_boundingCircle = m_boundingCircle;
			l_thread.m_killCircle = m_killCircle;
			l_thread.m_jumpCircle = m_jumpCircle;
			l_thread.m_minValue = m_minValue;
		} );
	}
}
