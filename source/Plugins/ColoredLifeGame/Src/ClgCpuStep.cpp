#include "ClgCpuStep.h"

#include <random>

using namespace ProceduralTextures;

namespace ColoredLifeGame
{
	Thread::Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		:	ProceduralTextures::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight )
		,	m_pxColour( 255, 255, 255, 255 )
	{
	}

	Thread::~Thread()
	{
	}

	void Thread::DoStep()
	{
		int l_iAlive;
		CellBuffer & l_cells = *m_cells.lock();
		CellBuffer & l_oldCells = *m_oldCells.lock();

		for ( int i = 0; i < m_iWidth; i++ )
		{
			for ( int j = m_iTop; j < m_iBottom; j++ )
			{
				l_iAlive = DoGetAliveNeightbours( i, j );

				if ( l_iAlive == 3 )
				{
					l_cells[j][i].Live();
				}
				else if ( l_iAlive != 2 )
				{
					l_cells[j][i].Die();
				}
				else
				{
					l_cells[j][i].SetAlive( l_oldCells[j][i].m_alive );
				}
			}
		}
	}

	int Thread::DoGetAliveNeightbours( int p_x, int p_y )
	{
		if ( p_x < 0 || p_x >= m_iWidth || p_y < 0 || p_y >= m_iHeight )
		{
			return 0;
		}

		int l_left = p_x - 1;
		int l_right = p_x + 1;
		int l_up = p_y - 1;
		int l_down = p_y + 1;

		if ( p_x == 0 )
		{
			l_left = m_iWidth - 1;
		}
		else if ( p_x == m_iWidth - 1 )
		{
			l_right = 0;
		}

		if ( p_y == 0 )
		{
			l_up = m_iHeight - 1;
		}
		else if ( p_y == m_iHeight - 1 )
		{
			l_down = 0;
		}

		int l_iResult = 0;
		CellBuffer & l_oldCells = *m_oldCells.lock();

		if ( l_oldCells[l_up][l_left].m_alive )
		{
			l_iResult++;
		}

		if ( l_oldCells[p_y][l_left].m_alive )
		{
			l_iResult++;
		}

		if ( l_oldCells[l_down][l_left].m_alive )
		{
			l_iResult++;
		}

		if ( l_oldCells[l_up][p_x].m_alive )
		{
			l_iResult++;
		}

		if ( l_oldCells[l_down][p_x].m_alive )
		{
			l_iResult++;
		}

		if ( l_oldCells[l_up][l_right].m_alive )
		{
			l_iResult++;
		}

		if ( l_oldCells[p_y][l_right].m_alive )
		{
			l_iResult++;
		}

		if ( l_oldCells[l_down][l_right].m_alive )
		{
			l_iResult++;
		}

		return l_iResult;
	}

	//*************************************************************************************************

	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, Size const & p_size )
		: ProceduralTextures::CpuStep< Thread >( p_generator, p_size )
		, m_oldCells( std::make_shared< CellBuffer >( p_size ) )
		, m_cells( std::make_shared< CellBuffer >( p_size ) )
		, m_alivePixel( 0, 0, 0, 255 )
		, m_deadPixel( 255, 255, 255, 255 )
		, m_previousBuffer( std::make_shared< PixelBuffer >( p_size ) )
	{
		m_stepPixel.r = abs( int( static_cast< unsigned int >( m_deadPixel.r ) + 1 - static_cast< unsigned int >( m_alivePixel.r ) ) >> 4 );
		m_stepPixel.g = abs( int( static_cast< unsigned int >( m_deadPixel.g ) + 1 - static_cast< unsigned int >( m_alivePixel.g ) ) >> 4 );
		m_stepPixel.b = abs( int( static_cast< unsigned int >( m_deadPixel.b ) + 1 - static_cast< unsigned int >( m_alivePixel.b ) ) >> 4 );
		m_stepPixel.a = abs( int( static_cast< unsigned int >( m_deadPixel.a ) + 1 - static_cast< unsigned int >( m_alivePixel.a ) ) >> 4 );
		bool l_alive;
		uint32_t l_width = p_size.x();
		uint32_t l_height = p_size.y();
		CellBuffer & l_cells = *m_cells;
		CellBuffer & l_oldCells = *m_oldCells;
		std::random_device l_randDevice;
		std::default_random_engine l_randEngine( l_randDevice() );
		std::uniform_int_distribution< int > l_randDistribution( 0, 1 );

		for ( uint32_t i = 0; i < l_height; i++ )
		{
			for ( uint32_t j = 0; j < l_width; j++ )
			{
				l_alive =  l_randDistribution( l_randEngine ) == 0;
				l_cells[i][j].Set( &( *m_finalBuffer )[i][j], &m_deadPixel, &m_alivePixel, &m_stepPixel, l_alive );
				l_oldCells[i][j].Set( &( *m_previousBuffer )[i][j], &m_deadPixel, &m_alivePixel, &m_stepPixel, l_alive );
			}
		}
	}

	CpuStep::~CpuStep()
	{
		m_cells->Clear();
		m_oldCells->Clear();
		m_cells.reset();
		m_oldCells.reset();
	}

	void CpuStep::SetRed( int val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetRed( uint8_t( val ) );
		} );

		m_alivePixel.r = val;
		m_stepPixel.r = abs( int( static_cast< unsigned int >( m_deadPixel.r ) + 1 - static_cast< unsigned int >( m_alivePixel.r ) ) >> 4 );
	}

	void CpuStep::SetGreen( int val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetGreen( uint8_t( val ) );
		} );

		m_alivePixel.g = val;
		m_stepPixel.r = abs( int( static_cast< unsigned int >( m_deadPixel.r ) + 1 - static_cast< unsigned int >( m_alivePixel.r ) ) >> 4 );
	}

	void CpuStep::SetBlue( int val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetBlue( uint8_t( val ) );
		} );

		m_alivePixel.b = val;
		m_stepPixel.r = abs( int( static_cast< unsigned int >( m_deadPixel.r ) + 1 - static_cast< unsigned int >( m_alivePixel.r ) ) >> 4 );
	}

	void CpuStep::DoInitialiseStep()
	{
	}

	void CpuStep::DoInitialise()
	{
		uint32_t l_width = m_sizeImage.x();
		uint32_t l_height = m_sizeImage.y();
		int l_iOffset = int( l_height / m_threadsCount );
		int l_iAdd;

		if ( uint32_t( l_iOffset * m_threadsCount ) == l_height )
		{
			l_iAdd = 0;
		}
		else
		{
			l_iAdd = l_height - int( l_iOffset * m_threadsCount );
		}

		for ( uint32_t i = 0; i < l_height; i += l_iOffset )
		{
			DoCreateThread( i, i + l_iOffset + l_iAdd, l_height );
			i += l_iAdd;
			l_iAdd = 0;
		}

		DoForEachThread( [this]( Thread & l_thread )
		{
			l_thread.SetCells( m_cells, m_oldCells );
			l_thread.Run();
		} );
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
		m_oldCells->swap( *m_cells );
	}
}

//*************************************************************************************************
