#include "LifeGame.h"

#include <Random.h>
#include <Value.h>

namespace ProceduralTextures
{
	LifeGame::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
		,	m_cells( NULL )
		,	m_oldCells( NULL )
	{
		Random::srand( int( time( NULL ) ) );
	}

	LifeGame::Thread::~Thread()
	{
	}

	void LifeGame::Thread::Step()
	{
		int l_iAlive;

		for ( int i = 0 ; i < m_iWidth ; i++ )
		{
			for ( int j = m_iTop ; j < m_iBottom ; j++ )
			{
				l_iAlive = _getAliveNeightbours( i, j );

				if ( l_iAlive == 3 )
				{
					( *m_cells )[j][i].Live();
				}
				else if ( l_iAlive != 2 )
				{
					( *m_cells )[j][i].Die();
				}
				else
				{
					( *m_cells )[j][i].SetAlive( ( *m_oldCells )[j][i].m_alive );
				}
			}
		}
	}

	int LifeGame::Thread::_getAliveNeightbours( int p_x, int p_y )
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

		if ( ( *m_oldCells )[l_up][l_left].m_alive )
		{
			l_iResult++;
		}

		if ( ( *m_oldCells )[p_y][l_left].m_alive )
		{
			l_iResult++;
		}

		if ( ( *m_oldCells )[l_down][l_left].m_alive )
		{
			l_iResult++;
		}

		if ( ( *m_oldCells )[l_up][p_x].m_alive )
		{
			l_iResult++;
		}

		if ( ( *m_oldCells )[l_down][p_x].m_alive )
		{
			l_iResult++;
		}

		if ( ( *m_oldCells )[l_up][l_right].m_alive )
		{
			l_iResult++;
		}

		if ( ( *m_oldCells )[p_y][l_right].m_alive )
		{
			l_iResult++;
		}

		if ( ( *m_oldCells )[l_down][l_right].m_alive )
		{
			l_iResult++;
		}

		return l_iResult;
	}

	//*************************************************************************************************

	LifeGame::LifeGame( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame, false )
		,	m_cells( Size( p_width, p_height ) )
		,	m_oldCells( Size( p_width, p_height ) )
	{
		Random::srand( int( time( NULL ) ) );
		bool l_alive;

		for ( int i = 0 ; i < p_height ; i++ )
		{
			for ( int j = 0 ; j < p_width ; j++ )
			{
				l_alive =  Random::rand() % 2 == 0;
				m_cells[i][j].Set( & m_frontBuffer[i][j], l_alive );
				m_oldCells[i][j].Set( & m_backBuffer[i][j], l_alive );
			}
		}

		LifeGame::Thread * l_pThread;
		int l_iOffset = int( p_height / m_uiThreadCount );
		int l_iAdd;

		if ( int( l_iOffset * m_uiThreadCount ) == p_height )
		{
			l_iAdd = 0;
		}
		else
		{
			l_iAdd = p_height - int( l_iOffset * m_uiThreadCount );
		}

		for ( int i = 0 ; i < p_height ; i += l_iOffset )
		{
			l_pThread = CreateThread< LifeGame::Thread >( p_width, i, i + l_iOffset + l_iAdd, p_height, m_pxColour );
			l_pThread->SetCells( & m_cells, & m_oldCells );
			l_pThread->Run();
			i += l_iAdd;
			l_iAdd = 0;
		}

		m_initialised = true;
	}

	LifeGame::~LifeGame()
	{
		m_initialised = false;
		DoCleanup();
		m_cells.clear();
		m_oldCells.clear();
	}

	void LifeGame::SwapBuffers()
	{
		m_oldCells.swap( m_cells );
	}

	void LifeGame::SetStartConfiguration( bool * p_cells )
	{
		for ( int i = 0 ; i < m_sizeImage.x ; i++ )
		{
			for ( int j = 0 ; j < m_sizeImage.y ; j++ )
			{
				m_cells[j][i].SetAlive( p_cells[i * m_sizeImage.y + j] );
			}
		}

		m_initialised = true;
	}
}
