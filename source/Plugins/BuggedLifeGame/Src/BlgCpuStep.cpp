#include "BlgCpuStep.h"

#include <random>

namespace BuggedLifeGame
{
	Thread::Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		: ProceduralTextures::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight )
		, m_pxColour( 255, 255, 255, 255 )
	{
	}

	Thread::~Thread()
	{
	}

	void Thread::DoStep()
	{
		int l_nbAlive = 0;
		CellBuffer & l_cells = *m_cells.lock();

		for ( int y = m_iTop; y < m_iBottom; y++ )
		{
			for ( int x = 0; x < m_iWidth; x++ )
			{
				l_nbAlive = DoGetAliveNeightbours( x, y );

				if ( l_nbAlive == 3 )
				{
					l_cells[y][x].Live();
				}
				else if ( l_nbAlive != 2 )
				{
					l_cells[y][x].Die();
				}
				else
				{
					l_cells[y][x].Stay();
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

		CellBuffer & l_cells = *m_cells.lock();
		return (	l_cells[l_up	][l_left	].m_alive ? 1 : 0 ) + (	l_cells[p_y		][l_left	].m_alive ? 1 : 0 ) + (	l_cells[l_down	][l_left	].m_alive ? 1 : 0 )
			   + (	l_cells[l_up	][p_x		].m_alive ? 1 : 0 ) + (	l_cells[l_down	][p_x		].m_alive ? 1 : 0 )
			   + (	l_cells[l_up	][l_right	].m_alive ? 1 : 0 ) + (	l_cells[p_y		][l_right	].m_alive ? 1 : 0 ) + (	l_cells[l_down	][l_right	].m_alive ? 1 : 0 );
	}

	//*************************************************************************************************

	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::CpuStep< Thread >( p_generator, p_size )
		, m_cells( std::make_shared< CellBuffer >( p_size ) )
		, m_alivePx( 255, 255, 255, 255 )
	{
		m_deadPx.r = 0;
		m_deadPx.g = 0;
		m_deadPx.b = 0;
		m_deadPx.a = 0;
		bool l_alive;
		uint32_t l_width = p_size.x();
		uint32_t l_height = p_size.y();
		CellBuffer & l_cells = *m_cells;
		std::random_device l_randDevice;
		std::default_random_engine l_randEngine( l_randDevice() );
		std::uniform_int_distribution< int > l_randDistribution( 0, 1 );

		for ( uint32_t i = 0; i < l_height; i++ )
		{
			for ( uint32_t j = 0; j < l_width; j++ )
			{
				l_alive = l_randDistribution( l_randEngine ) == 0;
				l_cells[i][j].Set( &( *m_finalBuffer )[i][j], &m_alivePx, &m_deadPx, l_alive );
			}
		}

		for ( uint32_t i = 0; i < l_width; i++ )
		{
			for ( uint32_t j = 0; j < l_height; j++ )
			{
				DoInitialiseNeighbours( i, j );
			}
		}
	}

	CpuStep::~CpuStep()
	{
		m_cells->Clear();
		m_cells.reset();
	}

	void CpuStep::SetRed( int val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetRed( uint8_t( val ) );
		} );

		m_alivePx.r = val;
	}

	void CpuStep::SetGreen( int val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetGreen( uint8_t( val ) );
		} );

		m_alivePx.g = val;
	}

	void CpuStep::SetBlue( int val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetBlue( uint8_t( val ) );
		} );

		m_alivePx.b = val;
	}

	void CpuStep::DoInitialiseStep()
	{
		CellBuffer & l_cells = *m_cells;

		for ( uint32_t i = 0; i < m_sizeImage.x(); i++ )
		{
			for ( uint32_t j = 0; j < m_sizeImage.y(); j++ )
			{
				l_cells[j][i].Update();
			}
		}

		DoForEachThread( [this]( Thread & l_thread )
		{
			l_thread.SetBuffer( m_cells );
		} );
	}

	void CpuStep::DoInitialise()
	{
		uint32_t l_width = m_sizeImage.x();
		uint32_t l_height = m_sizeImage.y();
		int l_iOffset = int( l_height / m_threadsCount );
		int l_iAdd;

		if ( int( l_iOffset * m_threadsCount ) == l_height )
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

		DoForEachThread( []( Thread & l_thread )
		{
			l_thread.Run();
		} );
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
	}

	void CpuStep::DoInitialiseNeighbours( uint32_t p_x, uint32_t p_y )
	{
		if ( p_x < 0 || p_x >= m_sizeImage.x() || p_y < 0 || p_y >= m_sizeImage.y() )
		{
			return;
		}

		int l_left = p_x - 1;
		int l_right = p_x + 1;
		int l_up = p_y - 1;
		int l_down = p_y + 1;

		if ( p_x == 0 )
		{
			l_left = m_sizeImage.x() - 1;
		}
		else if ( p_x == m_sizeImage.x() - 1 )
		{
			l_right = 0;
		}

		if ( p_y == 0 )
		{
			l_up = m_sizeImage.y() - 1;
		}
		else if ( p_y == m_sizeImage.y() - 1 )
		{
			l_down = 0;
		}

		CellBuffer & l_cells = *m_cells;
		l_cells[p_y][p_x].m_neighbours[0] = &l_cells[l_up	][l_left	];
		l_cells[p_y][p_x].m_neighbours[1] = &l_cells[p_y	][l_left	];
		l_cells[p_y][p_x].m_neighbours[2] = &l_cells[l_down	][l_left	];
		l_cells[p_y][p_x].m_neighbours[3] = &l_cells[l_up	][p_x		];
		l_cells[p_y][p_x].m_neighbours[4] = &l_cells[l_down	][p_x		];
		l_cells[p_y][p_x].m_neighbours[5] = &l_cells[l_up	][l_right	];
		l_cells[p_y][p_x].m_neighbours[6] = &l_cells[p_y	][l_right	];
		l_cells[p_y][p_x].m_neighbours[7] = &l_cells[l_down	][l_right	];
	}
}
