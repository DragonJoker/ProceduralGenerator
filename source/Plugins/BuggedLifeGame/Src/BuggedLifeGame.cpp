#include "BuggedLifeGame.h"

#include <Random.h>

namespace ProceduralTextures
{
	BuggedLifeGame::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
		,	m_cells( NULL )
	{
		Random::srand( int( time( NULL ) ) );
	}

	BuggedLifeGame::Thread::~Thread()
	{
	}

	void BuggedLifeGame::Thread::Step()
	{
		int l_nbAlive = 0;

		for ( int i = 0 ; i < m_iWidth ; i++ )
		{
			for ( int j = m_iTop ; j < m_iBottom ; j++ )
			{
				l_nbAlive = _getAliveNeightbours( i, j );

				if ( l_nbAlive == 3 )
				{
					( *m_cells )[j][i].Live();
				}
				else if ( l_nbAlive != 2 )
				{
					( *m_cells )[j][i].Die();
				}
				else
				{
					( *m_cells )[j][i].Stay();
				}
			}
		}
	}

	int BuggedLifeGame::Thread::_getAliveNeightbours( int p_x, int p_y )
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

		return (	( *m_cells )[l_up	][l_left	].m_alive ? 1 : 0 ) + (	( *m_cells )[p_y		][l_left	].m_alive ? 1 : 0 ) + (	( *m_cells )[l_down	][l_left	].m_alive ? 1 : 0 )
			   + (	( *m_cells )[l_up	][p_x		].m_alive ? 1 : 0 ) + (	( *m_cells )[l_down	][p_x		].m_alive ? 1 : 0 )
			   + (	( *m_cells )[l_up	][l_right	].m_alive ? 1 : 0 ) + (	( *m_cells )[p_y		][l_right	].m_alive ? 1 : 0 ) + (	( *m_cells )[l_down	][l_right	].m_alive ? 1 : 0 );
	}

	//*************************************************************************************************

	BuggedLifeGame::BuggedLifeGame( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame )
		,	m_cells( Size( p_width, p_height ) )
		,	m_oldCells( Size( p_width, p_height ) )
		,	m_specStatR( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "R" ) ) )
		,	m_specStatG( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "G" ) ) )
		,	m_specStatB( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "B" ) ) )
		,	m_specSlideR( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.r ) )
		,	m_specSlideG( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.g ) )
		,	m_specSlideB( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.b ) )
	{
		m_bNeedsConfigPanel = true;
		Random::srand( int( time( NULL ) ) );
		m_alivePx = m_pxColour;
		m_deadPx.r = 0;
		m_deadPx.g = 0;
		m_deadPx.b = 0;
		m_deadPx.a = 0;
		bool l_alive;

		for ( int i = 0 ; i < p_height ; i++ )
		{
			for ( int j = 0 ; j < p_width ; j++ )
			{
				l_alive =  Random::rand() % 2 == 0;
				m_cells[i][j].Set(	&m_frontBuffer[i][j], 	&m_alivePx, &m_deadPx, l_alive );
				m_oldCells[i][j].Set(	&m_backBuffer[i][j], 	&m_alivePx, &m_deadPx, l_alive );
			}
		}

		for ( int i = 0 ; i < p_width ; i++ )
		{
			for ( int j = 0 ; j < p_height ; j++ )
			{
				DoInitialiseNeighbours( i, j );
			}
		}

		BuggedLifeGame::Thread * l_pThread;
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
			l_pThread = CreateThread< BuggedLifeGame::Thread >( p_width, i, i + l_iOffset + l_iAdd, p_height, m_pxColour );
			l_pThread->Run();
			i += l_iAdd;
			l_iAdd = 0;
		}

		m_initialised = true;
		DoGeneratePanel();
	}

	BuggedLifeGame::~BuggedLifeGame()
	{
		m_initialised = false;
		DoCleanup();
		m_cells.clear();
		m_oldCells.clear();
	}

	void BuggedLifeGame::DoInitialiseStep()
	{
		for ( int i = 0 ; i < m_sizeImage.x ; i++ )
		{
			for ( int j = 0 ; j < m_sizeImage.y ; j++ )
			{
				m_oldCells[j][i].Update();
			}
		}

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				static_cast< BuggedLifeGame::Thread * >( m_arraySlaveThreads[i] )->SetBuffer( & m_oldCells );
			}
		}
	}

	void BuggedLifeGame::DoInitialiseNeighbours( int p_x, int p_y )
	{
		if ( p_x < 0 || p_x >= m_sizeImage.x || p_y < 0 || p_y >= m_sizeImage.y )
		{
			return;
		}

		int l_left = p_x - 1;
		int l_right = p_x + 1;
		int l_up = p_y - 1;
		int l_down = p_y + 1;

		if ( p_x == 0 )
		{
			l_left = m_sizeImage.x - 1;
		}
		else if ( p_x == m_sizeImage.x - 1 )
		{
			l_right = 0;
		}

		if ( p_y == 0 )
		{
			l_up = m_sizeImage.y - 1;
		}
		else if ( p_y == m_sizeImage.y - 1 )
		{
			l_down = 0;
		}

		m_cells[p_y][p_x].m_neighbours[0] = & m_cells[l_up		][l_left	];
		m_cells[p_y][p_x].m_neighbours[1] = & m_cells[p_y		][l_left	];
		m_cells[p_y][p_x].m_neighbours[2] = & m_cells[l_down	][l_left	];
		m_cells[p_y][p_x].m_neighbours[3] = & m_cells[l_up		][p_x		];
		m_cells[p_y][p_x].m_neighbours[4] = & m_cells[l_down	][p_x		];
		m_cells[p_y][p_x].m_neighbours[5] = & m_cells[l_up		][l_right	];
		m_cells[p_y][p_x].m_neighbours[6] = & m_cells[p_y		][l_right	];
		m_cells[p_y][p_x].m_neighbours[7] = & m_cells[l_down	][l_right	];
		m_oldCells[p_y][p_x].m_neighbours[0] = & m_oldCells[l_up	][l_left	];
		m_oldCells[p_y][p_x].m_neighbours[1] = & m_oldCells[p_y		][l_left	];
		m_oldCells[p_y][p_x].m_neighbours[2] = & m_oldCells[l_down	][l_left	];
		m_oldCells[p_y][p_x].m_neighbours[3] = & m_oldCells[l_up	][p_x		];
		m_oldCells[p_y][p_x].m_neighbours[4] = & m_oldCells[l_down	][p_x		];
		m_oldCells[p_y][p_x].m_neighbours[5] = & m_oldCells[l_up	][l_right	];
		m_oldCells[p_y][p_x].m_neighbours[6] = & m_oldCells[p_y		][l_right	];
		m_oldCells[p_y][p_x].m_neighbours[7] = & m_oldCells[l_down	][l_right	];
	}

	int BuggedLifeGame::_getAliveNeightbours( int p_x, int p_y )
	{
		if ( p_x < 0 || p_x >= m_sizeImage.x || p_y < 0 || p_y >= m_sizeImage.y )
		{
			return 0;
		}

		int l_left = p_x - 1;
		int l_right = p_x + 1;
		int l_up = p_y - 1;
		int l_down = p_y + 1;

		if ( p_x == 0 )
		{
			l_left = m_sizeImage.x - 1;
		}
		else if ( p_x == m_sizeImage.x - 1 )
		{
			l_right = 0;
		}

		if ( p_y == 0 )
		{
			l_up = m_sizeImage.y - 1;
		}
		else if ( p_y == m_sizeImage.y - 1 )
		{
			l_down = 0;
		}

		return (	m_cells[l_up	][l_left	].m_alive ? 1 : 0 ) + (	m_cells[p_y		][l_left	].m_alive ? 1 : 0 ) + (	m_cells[l_down	][l_left	].m_alive ? 1 : 0 )
			   + (	m_cells[l_up	][p_x		].m_alive ? 1 : 0 ) + (	m_cells[l_down	][p_x		].m_alive ? 1 : 0 )
			   + (	m_cells[l_up	][l_right	].m_alive ? 1 : 0 ) + (	m_cells[p_y		][l_right	].m_alive ? 1 : 0 ) + (	m_cells[l_down	][l_right	].m_alive ? 1 : 0 );
	}

	void BuggedLifeGame::DoGeneratePanel()
	{
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 20,  0 ), wxSize( 20, wxDEFAULT_HEIGHT ), &m_specStatR ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eRedValue,		wxPoint( 20, wxDEFAULT_HEIGHT ), wxSize( 20, 60 ), &m_specSlideR,	wxSL_VERTICAL, eEVENT_TYPE_SLIDER_UPDATED, wxCommandEventHandler( BuggedLifeGame::OnSliderRed ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 60,  0 ), wxSize( 60, wxDEFAULT_HEIGHT ), &m_specStatG ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eGreenValue,	wxPoint( 60, wxDEFAULT_HEIGHT ), wxSize( 20, 60 ), &m_specSlideG,	wxSL_VERTICAL, eEVENT_TYPE_SLIDER_UPDATED, wxCommandEventHandler( BuggedLifeGame::OnSliderGreen ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 100,  0 ), wxSize( 20, wxDEFAULT_HEIGHT ), &m_specStatB ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eBlueValue,		wxPoint( 100, wxDEFAULT_HEIGHT ), wxSize( 20, 60 ), &m_specSlideB,	wxSL_VERTICAL, eEVENT_TYPE_SLIDER_UPDATED, wxCommandEventHandler( BuggedLifeGame::OnSliderBlue ) ) );
	}

	void BuggedLifeGame::OnSliderRed( wxCommandEvent & p_event )
	{
		SetRed( uint8_t( p_event.GetInt() ) );
	}

	void BuggedLifeGame::OnSliderGreen( wxCommandEvent & p_event )
	{
		SetGreen( uint8_t( p_event.GetInt() ) );
	}

	void BuggedLifeGame::OnSliderBlue( wxCommandEvent & p_event )
	{
		SetBlue( uint8_t( p_event.GetInt() ) );
	}
}
