#include "DiffusionLimitedAggregation.h"

#include <Random.h>

namespace ProceduralTextures
{
	DiffusionLimitedAggregation::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
	{
		Random::srand( int( time( NULL ) ) );
		m_passed.resize( m_iWidth * m_iHeight );
	}

	DiffusionLimitedAggregation::Thread::~Thread()
	{
	}

	void DiffusionLimitedAggregation::Thread::_getPointOnCircle( Point2i & p_ptPoint, const Point2i & p_ptOrigin, float p_fRadius )
	{
		int l_iAngle;
		float l_fRadius;
		int l_iCount = 1000;

		do
		{
			l_iAngle = Random::rand() % 360;
			l_fRadius = ( 3 * p_fRadius / 4 ) + Random::rand() % std::max< int >( 1, int( p_fRadius / 4 ) );
			l_fRadius += 2;
			//		l_fRadius = p_fRadius;
			p_ptPoint[0] = p_ptOrigin[0] + int( sin( l_iAngle * M_PI / 180.0 ) * l_fRadius );
			p_ptPoint[1] = p_ptOrigin[1] + int( cos( l_iAngle * M_PI / 180.0 ) * l_fRadius );
			l_iCount--;
		}
		while ( l_iCount != 0 && ( p_ptPoint[0] >= m_iWidth || p_ptPoint[1] >= m_iHeight || p_ptPoint[0] < 0 || p_ptPoint[1] < 0 || ( * m_cells )[p_ptPoint[0]][p_ptPoint[1]].m_white ) );
	}

	void DiffusionLimitedAggregation::Thread::Step()
	{
		Point2i l_ptPoint;
		Point2i l_ptTmp;
		std::set < int > l_setTries;
		_getPointOnCircle( l_ptPoint, m_ptOrigin, float( m_iBoundingCircle + 5 ) );
		m_passed.assign( m_iWidth * m_iHeight, false );
		bool l_alreadyPassedWhite = false;
		int l_iDistance = int( ( l_ptPoint - m_ptOrigin ).GetSquaredLength() );
		int l_iSquaredKillCircle = m_iKillCircle * m_iKillCircle;
		int l_iSquaredJumpCircle = m_iJumpCircle * m_iJumpCircle;

		for ( int i = 1000 ; i != 0 && ! l_alreadyPassedWhite && !( * m_cells )[l_ptPoint[0]][l_ptPoint[1]].m_white ; i-- )
		{
			while ( l_iDistance > l_iSquaredJumpCircle )
			{
				if ( l_iDistance > l_iSquaredKillCircle )
				{
					_getPointOnCircle( l_ptPoint, m_ptOrigin, float( m_iBoundingCircle + 5 ) );
					l_iDistance = int( ( l_ptPoint - m_ptOrigin ).GetSquaredLength() );
				}
				else
				{
					_getPointOnCircle( l_ptPoint, l_ptPoint, sqrt( float( l_iDistance ) ) - m_iBoundingCircle );
					l_iDistance = int( ( l_ptPoint - m_ptOrigin ).GetSquaredLength() );
				}
			}

			DlaCell & l_cell = ( * m_cells )[l_ptPoint[0]][l_ptPoint[1]];

			if ( l_cell.m_whiteNeighbour )
			{
				if ( l_cell.IncValue( m_iMinValue ) )
				{
					l_alreadyPassedWhite = true;
					m_iBoundingCircle = std::max( m_iBoundingCircle, int( sqrt( ( float )l_iDistance ) ) );

					if ( m_iBoundingCircle == 1 )
					{
						m_iBoundingCircle = 2;
					}

					_setCellWhite( l_ptPoint );
					( * m_iCurrentMass )++;
				}
			}
			else
			{
				l_setTries.clear();

				do
				{
					l_ptTmp = l_ptPoint;
					int l_direction = Random::rand() % 4;

					if ( l_direction == 0 )
					{
						if ( l_ptTmp[0] > 0 )
						{
							l_ptTmp[0]--;
						}
					}
					else if ( l_direction == 1 )
					{
						if ( l_ptTmp[0] < m_iWidth - 1 )
						{
							l_ptTmp[0]++;
						}
					}
					else if ( l_direction == 2 )
					{
						if ( l_ptTmp[1] > 0 )
						{
							l_ptTmp[1]--;
						}
					}
					else if ( l_direction == 3 )
					{
						if ( l_ptTmp[1] < m_iHeight - 1 )
						{
							l_ptTmp[1]++;
						}
					}

					l_setTries.insert( l_direction );
				}
				while ( m_passed[l_ptTmp[0] * m_iHeight + l_ptTmp[1]] && l_setTries.size() < 4 );

				m_passed[l_ptTmp[0] * m_iHeight + l_ptTmp[1]] = true;

				if ( l_setTries.size() == 4 )
				{
					l_alreadyPassedWhite = true;
				}
				else
				{
					l_ptPoint = l_ptTmp;
					l_iDistance = int( ( l_ptPoint - m_ptOrigin ).GetSquaredLength() );
				}
			}
		}
	}

	void DiffusionLimitedAggregation::Thread::_setCellWhite( const Point2i & p_ptPoint )
	{
		int x = p_ptPoint[0];
		int y = p_ptPoint[1];
		( * m_cells )[x][y].SetWhite( ( p_ptPoint - m_ptOrigin ).GetLength() / m_dMaxDistance );

		if ( x > 0 )
		{
			( * m_cells )[( x - 1 )][y].m_whiteNeighbour = true;
		}

		if ( y > 0 )
		{
			( * m_cells )[x][( y - 1 )].m_whiteNeighbour = true;
		}

		if ( x < m_iWidth - 1 )
		{
			( * m_cells )[( x + 1 )][y].m_whiteNeighbour = true;
		}

		if ( y < m_iHeight - 1 )
		{
			( * m_cells )[x][( y + 1 )].m_whiteNeighbour = true;
		}
	}

	//*************************************************************************************************

	DiffusionLimitedAggregation::DiffusionLimitedAggregation( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame )
		,	m_iBoundingCircle( 2 )
		,	m_iMinValue( 1 )
		,	m_ptOrigin( p_width / 2, p_height / 2 )
		,	m_iCurrentMass( 0 )
		,	m_iMass( 10000 )
		,	m_cells( Size( p_width, p_height ) )
		,	m_specStatMassTitle( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Max Mass :" ) ) )
		,	m_specStatMass( SpecificControlParameters< eCONTROL_TYPE_STATIC >( wxT( "10000" ) ) )
		,	m_specSlideMass( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 10000, 1000000, 10000 ) )
		,	m_specStatMinValueTitle( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "MinValue :" ) ) )
		,	m_specStatMinValue( SpecificControlParameters< eCONTROL_TYPE_STATIC >( wxT( "1" ) ) )
		,	m_specSlideMinValue( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 1, 10, 1 ) )
	{
		m_dMaxDistance = m_ptOrigin.GetLength();
		m_bNeedsConfigPanel = true;
		m_iJumpCircle = m_iBoundingCircle << 1;
		m_iKillCircle = m_iBoundingCircle << 2;
		Random::srand( int( time( NULL ) ) );

		for ( int i = 0 ; i < p_height ; i++ )
		{
			for ( int j = 0 ; j < p_width ; j++ )
			{
				m_cells[i][j].Set( & m_backBuffer[i][j], false );
			}
		}

		_setCellWhite( m_ptOrigin );
		int l_iOffset = int( p_height / m_uiThreadCount );
		DiffusionLimitedAggregation::Thread * l_pThread;

		for ( int i = 0 ; i < p_height ; i += l_iOffset )
		{
			l_pThread = CreateThread< DiffusionLimitedAggregation::Thread >( p_width, 0, p_height, p_height, m_pxColour );
			l_pThread->SetBuffer( & m_cells );
			l_pThread->m_iMinValue = m_iMinValue;
			l_pThread->m_dMaxDistance = m_dMaxDistance;
			l_pThread->m_ptOrigin = m_ptOrigin;
			l_pThread->m_iBoundingCircle = m_iBoundingCircle;
			l_pThread->m_iKillCircle = m_iKillCircle;
			l_pThread->m_iJumpCircle = m_iJumpCircle;
			l_pThread->m_iCurrentMass = & m_iCurrentMass;
			l_pThread->Run();
		}

		DoGeneratePanel();
		m_initialised = true;
	}

	DiffusionLimitedAggregation::~DiffusionLimitedAggregation()
	{
		DoCleanup();
	}

	void DiffusionLimitedAggregation::SwapBuffers()
	{
		DiffusionLimitedAggregation::Thread * l_pThread;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				l_pThread = static_cast< DiffusionLimitedAggregation::Thread * >( m_arraySlaveThreads[i] );
				m_iBoundingCircle = std::max( m_iBoundingCircle, l_pThread->m_iBoundingCircle );
			}
		}

		m_iJumpCircle = m_iBoundingCircle << 1;
		m_iKillCircle = m_iBoundingCircle << 2;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				l_pThread = static_cast< DiffusionLimitedAggregation::Thread * >( m_arraySlaveThreads[i] );
				l_pThread->m_iBoundingCircle = m_iBoundingCircle;
				l_pThread->m_iKillCircle = m_iKillCircle;
				l_pThread->m_iJumpCircle = m_iJumpCircle;
				l_pThread->m_iMinValue = m_iMinValue;
			}
		}

		ProceduralGenerator::SwapBuffers();
	}

	void DiffusionLimitedAggregation::DoGeneratePanel()
	{
		int iWidth = 150;
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specStatMassTitle ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 90, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth - 100,  wxDEFAULT_HEIGHT ), &m_specStatMass ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_MASS,		wxPoint( 10, wxDEFAULT_HEIGHT * 1 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specSlideMass,			wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,	wxCommandEventHandler(	DiffusionLimitedAggregation::OnMass ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specStatMinValueTitle ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 110, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth - 110,  wxDEFAULT_HEIGHT ), &m_specStatMinValue ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_MINVALUE,	wxPoint( 10, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specSlideMinValue,		wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,	wxCommandEventHandler(	DiffusionLimitedAggregation::OnMinValue ) ) );
	}

	void DiffusionLimitedAggregation::_setCellWhite( const Point2i & p_ptPoint )
	{
		int x = p_ptPoint[0];
		int y = p_ptPoint[1];
		m_cells[x][y].SetWhite( ( p_ptPoint - m_ptOrigin ).GetLength() / m_dMaxDistance );

		if ( x > 0 )
		{
			m_cells[( x - 1 )][y].m_whiteNeighbour = true;
		}
		else
		{
			m_cells[( m_sizeImage.x - 1 )][y].m_whiteNeighbour = true;
		}

		if ( y > 0 )
		{
			m_cells[x][( y - 1 )].m_whiteNeighbour = true;
		}
		else
		{
			m_cells[x][( m_sizeImage.y - 1 )].m_whiteNeighbour = true;
		}

		if ( x < m_sizeImage.x - 1 )
		{
			m_cells[( x + 1 )][y].m_whiteNeighbour = true;
		}
		else
		{
			m_cells[0][y].m_whiteNeighbour = true;
		}

		if ( y < m_sizeImage.y - 1 )
		{
			m_cells[x][( y + 1 )].m_whiteNeighbour = true;
		}
		else
		{
			m_cells[x][0].m_whiteNeighbour = true;
		}
	}

	void DiffusionLimitedAggregation::OnMass( wxCommandEvent & p_event )
	{
		int l_iValue = p_event.GetInt();
		SetMass( l_iValue );
		wxString l_strValue;
		l_strValue << l_iValue;
		m_specStatMass.m_pControl->SetLabel( l_strValue );
	}

	void DiffusionLimitedAggregation::OnMinValue( wxCommandEvent & p_event )
	{
		int l_iValue = p_event.GetInt();
		SetMinValue( l_iValue );
		wxString l_strValue;
		l_strValue << l_iValue;
		m_specStatMinValue.m_pControl->SetLabel( l_strValue );
	}
}
