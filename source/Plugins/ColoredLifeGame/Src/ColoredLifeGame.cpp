#include "ColoredLifeGame.h"

#include <Random.h>

namespace ProceduralTextures
{
	ColoredLifeGame::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
		,	m_cells( NULL )
		,	m_oldCells( NULL )
	{
		Random::srand( int( time( NULL ) ) );
	}

	ColoredLifeGame::Thread::~Thread()
	{
	}

	void ColoredLifeGame::Thread::Step()
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

	int ColoredLifeGame::Thread::_getAliveNeightbours( int p_x, int p_y )
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

	ColoredLifeGame::ColoredLifeGame( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame )
		,	m_oldCells( Point2i( p_width, p_height ) )
		,	m_cells( Point2i( p_width, p_height ) )
		,	m_specStatR( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "R" ) ) )
		,	m_specStatG( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "G" ) ) )
		,	m_specStatB( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "B" ) ) )
		,	m_specSlideR( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.r ) )
		,	m_specSlideG( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.g ) )
		,	m_specSlideB( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.b ) )
	{
		m_bNeedsConfigPanel = true;
		m_pxColour.r = 0;
		m_pxColour.g = 0;
		m_pxColour.b = 0;
		Random::srand( int( time( NULL ) ) );
		m_alivePixel = m_pxColour;
		m_deadPixel.r = 255;
		m_deadPixel.g = 255;
		m_deadPixel.b = 255;
		m_deadPixel.a = 255;
		m_stepPixel.r = abs( int( static_cast< unsigned int >( m_deadPixel.r ) + 1 - static_cast< unsigned int >( m_alivePixel.r ) ) >> 4 );
		m_stepPixel.g = abs( int( static_cast< unsigned int >( m_deadPixel.g ) + 1 - static_cast< unsigned int >( m_alivePixel.g ) ) >> 4 );
		m_stepPixel.b = abs( int( static_cast< unsigned int >( m_deadPixel.b ) + 1 - static_cast< unsigned int >( m_alivePixel.b ) ) >> 4 );
		m_stepPixel.a = abs( int( static_cast< unsigned int >( m_deadPixel.a ) + 1 - static_cast< unsigned int >( m_alivePixel.a ) ) >> 4 );
		bool l_alive;

		for ( int i = 0 ; i < p_height ; i++ )
		{
			for ( int j = 0 ; j < p_width ; j++ )
			{
				l_alive =  Random::rand() % 2 == 0;
				m_cells[i][j].Set( & m_frontBuffer[i][j], & m_deadPixel, & m_alivePixel, & m_stepPixel, l_alive );
				m_oldCells[i][j].Set( & m_backBuffer[i][j], & m_deadPixel, & m_alivePixel, & m_stepPixel, l_alive );
			}
		}

		ColoredLifeGame::Thread * l_pThread;
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
			l_pThread = CreateThread< ColoredLifeGame::Thread >( p_width, i, i + l_iOffset + l_iAdd, p_height, m_pxColour );
			l_pThread->SetCells( & m_cells, & m_oldCells );
			l_pThread->Run();
			i += l_iAdd;
			l_iAdd = 0;
		}

		DoGeneratePanel();
		m_initialised = true;
	}

	ColoredLifeGame::~ColoredLifeGame()
	{
		m_initialised = false;
		DoCleanup();
		m_cells.clear();
		m_oldCells.clear();
	}

	void ColoredLifeGame::SwapBuffers()
	{
		m_oldCells.swap( m_cells );
	}

	void ColoredLifeGame::DoSetRed( uint8_t val )
	{
		m_pxColour.r = val;
		m_alivePixel.r = val;
		m_stepPixel.r = abs( int( static_cast< unsigned int >( m_deadPixel.r ) + 1 - static_cast< unsigned int >( m_alivePixel.r ) ) >> 4 );
	}
	void ColoredLifeGame::DoSetGreen( uint8_t val )
	{
		m_pxColour.g = val;
		m_alivePixel.g = val;
		m_stepPixel.g = abs( int( static_cast< unsigned int >( m_deadPixel.g ) + 1 - static_cast< unsigned int >( m_alivePixel.g ) ) >> 4 );
	}

	void ColoredLifeGame::DoSetBlue( uint8_t val )
	{
		m_pxColour.b = val;
		m_alivePixel.b = val;
		m_stepPixel.b = abs( int( static_cast< unsigned int >( m_deadPixel.b ) + 1 - static_cast< unsigned int >( m_alivePixel.b ) ) >> 4 );
	}

	void ColoredLifeGame::DoGeneratePanel()
	{
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 20,  0 ), wxSize( 20, wxDEFAULT_HEIGHT ), &m_specStatR ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eRedValue,		wxPoint( 20, wxDEFAULT_HEIGHT ), wxSize( 20, 60 ), &m_specSlideR,	wxSL_VERTICAL, eEVENT_TYPE_SLIDER_UPDATED, wxCommandEventHandler(	ColoredLifeGame::OnSliderRed ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 60,  0 ), wxSize( 60, wxDEFAULT_HEIGHT ), &m_specStatG ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eGreenValue,	wxPoint( 60, wxDEFAULT_HEIGHT ), wxSize( 20, 60 ), &m_specSlideG,	wxSL_VERTICAL, eEVENT_TYPE_SLIDER_UPDATED, wxCommandEventHandler(	ColoredLifeGame::OnSliderGreen ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 100,  0 ), wxSize( 20, wxDEFAULT_HEIGHT ), &m_specStatB ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eBlueValue,		wxPoint( 100, wxDEFAULT_HEIGHT ), wxSize( 20, 60 ), &m_specSlideB,	wxSL_VERTICAL, eEVENT_TYPE_SLIDER_UPDATED, wxCommandEventHandler(	ColoredLifeGame::OnSliderBlue ) ) );
	}

	void ColoredLifeGame::OnSliderRed( wxCommandEvent & p_event )
	{
		SetRed( uint8_t( 255 - p_event.GetInt() ) );
	}

	void ColoredLifeGame::OnSliderGreen( wxCommandEvent & p_event )
	{
		SetGreen( uint8_t( 255 - p_event.GetInt() ) );
	}

	void ColoredLifeGame::OnSliderBlue( wxCommandEvent & p_event )
	{
		SetBlue( uint8_t( 255 - p_event.GetInt() ) );
	}
}

//*************************************************************************************************
