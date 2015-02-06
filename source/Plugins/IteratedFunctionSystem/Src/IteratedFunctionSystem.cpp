#include "IteratedFunctionSystem.h"

#include "FractalLeaf.h"
#include "HeighwayDragon.h"
#include "KochCurve.h"
#include "LevyDragon.h"
#include "McWorterPentigree.h"
#include "Pentadentrite.h"
#include "SierpinskyCarpet.h"
#include "SierpinskyGasket.h"

#include <Random.h>

namespace ProceduralTextures
{
	IteratedFunctionSystem::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
	{
		Random::srand( int( time( NULL ) ) );
	}

	IteratedFunctionSystem::Thread::~Thread()
	{
		m_pMapCells = NULL;
		m_backBuffer = NULL;
	}

	void IteratedFunctionSystem::Thread::Step()
	{
		//	StepLst();
		//	StepVct();
		StepMap();
	}

	void IteratedFunctionSystem::Thread::Initialise( IfsCellMap * p_pMapCells, IfsCellLst * p_pLstCells, IfsCellVct * p_pVctCells, PixelBuffer * p_pBackBuffer, IfsGenerator ** p_pGenerator )
	{
		m_pMapCells = p_pMapCells;
		m_pLstCells = p_pLstCells;
		m_pVctCells = p_pVctCells;
		m_backBuffer = p_pBackBuffer;
		m_pGenerator = p_pGenerator;
		m_uiCells1 = 0;
		m_uiCellsSize = m_pVctCells->size();
		m_itLstCells1 = m_pLstCells->begin();
		m_itMapCells1 = m_pMapCells->begin();
		m_bResult = true;
	}

	void IteratedFunctionSystem::Thread::StepMap()
	{
		if ( m_itMapCells1 != m_pMapCells->end() )
		{
			for ( IfsCellIntMap::iterator l_itMapCells2 = m_itMapCells1->second.begin() ; ! IsStopped() && l_itMapCells2 != m_itMapCells1->second.end() ; ++l_itMapCells2 )
			{
				m_bResult |= ( * m_pGenerator )->Generate( l_itMapCells2->second, m_mapGeneratedCells );
			}

			m_itMapCells1++;

			if ( m_itMapCells1 == m_pMapCells->end() )
			{
				if ( m_bResult )
				{
					IfsCellMap::iterator l_itCells1;
					IfsCellIntMap::iterator l_itCells2;

					for ( IfsCellMap::iterator l_it1 = m_mapGeneratedCells.begin() ; l_it1 != m_mapGeneratedCells.end() ; ++l_it1 )
					{
						l_itCells1 = m_pMapCells->find( l_it1->first );

						if ( l_itCells1 == m_pMapCells->end() )
						{
							m_pMapCells->insert( IfsCellMap::value_type( l_it1->first, l_it1->second ) );
						}
						else
						{
							for ( IfsCellIntMap::iterator l_it2 = l_it1->second.begin() ; l_it2 != l_it1->second.end() ; ++l_it2 )
							{
								l_itCells2 = l_itCells1->second.find( l_it2->first );

								if ( l_itCells2 == l_itCells1->second.end() )
								{
									l_itCells1->second.insert( IfsCellIntMap::value_type( l_it2->first, l_it2->second ) );
								}
							}
						}
					}

					m_itMapCells1 = m_pMapCells->begin();
					m_mapGeneratedCells.clear();
					m_bResult = true;
					( * m_pGenerator )->NextGeneration();
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

	void IteratedFunctionSystem::Thread::StepLst()
	{
		if ( m_itLstCells1 != m_pLstCells->end() )
		{
			m_bResult |= ( * m_pGenerator )->Generate( * m_itLstCells1, m_lstGeneratedCells );
			IfsCellLst::iterator l_itTmp = m_itLstCells1;
			m_itLstCells1++;

			if ( m_itLstCells1 == m_pLstCells->end() )
			{
				m_pLstCells->insert( l_itTmp, m_lstGeneratedCells.begin(), m_lstGeneratedCells.end() );
				m_itLstCells1 = m_pLstCells->begin();
				( * m_pGenerator )->NextGeneration();
			}
		}
		else
		{
			Stop();
		}
	}

	void IteratedFunctionSystem::Thread::StepVct()
	{
		if ( m_uiCells1 < m_pVctCells->size() )
		{
			m_bResult |= ( * m_pGenerator )->Generate( m_pVctCells->at( m_uiCells1 ), ( * m_pVctCells ) );
			m_uiCells1++;

			if ( m_uiCells1 == m_uiCellsSize )
			{
				m_uiCells1 = 0;
				m_uiCellsSize = m_pVctCells->size();
				m_pVctCells->reserve( m_pVctCells->size() * ( * m_pGenerator )->GetVariationCount() );
				( * m_pGenerator )->NextGeneration();
			}
		}
		else
		{
			Stop();
		}
	}

	//*************************************************************************************************

	wxString IteratedFunctionSystem::ArrayNames[eIFS_GENERATOR_COUNT];

	IteratedFunctionSystem::IteratedFunctionSystem( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame )
		,	m_pGenerator( NULL )
		,	m_specStaticAngle( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Angle :" ) ) )
		,	m_specEditAngle( SpecificControlParameters< eCONTROL_TYPE_EDIT >( wxT( "0" ) ) )
		,	m_specStaticShift( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Shift :" ) ) )
		,	m_specEditShift( SpecificControlParameters< eCONTROL_TYPE_EDIT >( wxT( "0" ) ) )
		,	m_specStaticSize( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Size :" ) ) )
		,	m_specEditSize( SpecificControlParameters< eCONTROL_TYPE_EDIT >( wxT( "0" ) ) )
		,	m_specStaticGenerator( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Generator :" ) ) )
		,	m_specComboGenerator( SpecificControlParameters< eCONTROL_TYPE_COMBO >() )
	{
		for ( int i = 0 ; i < eIFS_GENERATOR_COUNT ; i++ )
		{
			switch ( i )
			{
			case eIFS_GENERATOR_SIERPINSKI_CARPET:
				ArrayNames[i] = _( "Sierpinski Carpet" );
				m_arrayGenerators[i] = new SierpinskiCarpet( & m_backBuffer, p_width, p_height );
				break;

			case eIFS_GENERATOR_SIERPINSKI_GASKET:
				ArrayNames[i] = _( "Sierpinski Gasket" );
				m_arrayGenerators[i] = new SierpinskiGasket( & m_backBuffer, p_width, p_height );
				break;

			case eIFS_GENERATOR_FRACTAL_LEAF:
				ArrayNames[i] = _( "Fractal Leaf" );
				m_arrayGenerators[i] = new FractalLeaf( & m_backBuffer, p_width, p_height );
				break;

			case eIFS_GENERATOR_HEIGHWAY_DRAGON:
				ArrayNames[i] = _( "Heighway Dragon" );
				m_arrayGenerators[i] = new HeighwayDragon( & m_backBuffer, p_width, p_height );
				break;

			case eIFS_GENERATOR_MCWORTHER_PENTIGREE:
				ArrayNames[i] = _( "Mc Worther Pentigree" );
				m_arrayGenerators[i] = new McWorterPentigree( & m_backBuffer, p_width, p_height );
				break;

			case eIFS_GENERATOR_PENTADENTRITE:
				ArrayNames[i] = _( "Pentadentrite" );
				m_arrayGenerators[i] = new Pentadentrite( & m_backBuffer, p_width, p_height );
				break;

			case eIFS_GENERATOR_KOCH_CURVE:
				ArrayNames[i] = _( "Koch Curve" );
				m_arrayGenerators[i] = new KochCurve( & m_backBuffer, p_width, p_height );
				break;

			case eIFS_GENERATOR_LEVY_DRAGON:
				ArrayNames[i] = _( "Levy Dragon" );
				m_arrayGenerators[i] = new LevyDragon( & m_backBuffer, p_width, p_height );
				break;
			}
		}

		m_specComboGenerator.m_strValue = ArrayNames[eIFS_GENERATOR_SIERPINSKI_CARPET];
		m_specComboGenerator.m_arrayValues.insert( m_specComboGenerator.m_arrayValues.end(), &ArrayNames[0], &ArrayNames[eIFS_GENERATOR_COUNT] );
		m_bNeedsConfigPanel = true;
		Random::srand( int( time( NULL ) ) );
		m_pGenerator = m_arrayGenerators[eIFS_GENERATOR_SIERPINSKI_CARPET];
		_initialise();
		DoGeneratePanel();
	}

	IteratedFunctionSystem::~IteratedFunctionSystem()
	{
		DoCleanup();
		m_mapCells.clear();
		m_pGenerator = NULL;

		for ( int i = 0 ; i < eIFS_GENERATOR_COUNT ; i++ )
		{
			delete m_arrayGenerators[i];
		}
	}

	void IteratedFunctionSystem::SetGenerator( eIFS_GENERATOR p_eGenerator )
	{
		m_initialised = true;
		ClearAllThreads();
		m_pGenerator = m_arrayGenerators[p_eGenerator];
		_initialise();
	}

	void IteratedFunctionSystem::DoGeneratePanel()
	{
		int iWidth = 150;
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 0 ), wxSize( 40, wxDEFAULT_HEIGHT ), &m_specStaticAngle ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_EDIT,	eID_ANGLE,		wxPoint( 50, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth -  60, wxDEFAULT_HEIGHT ), &m_specEditAngle,			wxTE_PROCESS_ENTER,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	IteratedFunctionSystem::OnAngle ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 1 ), wxSize( 40, wxDEFAULT_HEIGHT ), &m_specStaticShift ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_EDIT,	eID_SHIFT,		wxPoint( 50, wxDEFAULT_HEIGHT * 1 ), wxSize( iWidth -  60, wxDEFAULT_HEIGHT ), &m_specEditShift,			wxTE_PROCESS_ENTER,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	IteratedFunctionSystem::OnShift ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( 40, wxDEFAULT_HEIGHT ), &m_specStaticSize ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_EDIT,	eID_SIZE,		wxPoint( 50, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  60, wxDEFAULT_HEIGHT ), &m_specEditSize,				wxTE_PROCESS_ENTER,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	IteratedFunctionSystem::OnSizeIfs ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticGenerator ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,	eID_GENERATOR,	wxPoint( 10, wxDEFAULT_HEIGHT * 4 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboGenerator,		wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED, wxCommandEventHandler(	IteratedFunctionSystem::OnGenerator ) ) );
	}

	void IteratedFunctionSystem::_initialise()
	{
		m_mapCells.clear();
		m_lstCells.clear();
		m_pGenerator->Initialise( m_mapCells, m_lstCells, m_VctCells );
		m_backBuffer.init();
		IteratedFunctionSystem::Thread * l_pThread;

		for ( size_t i = 0 ; i < m_pGenerator->GetInitCellsMapSize() ; i++ )
		{
			l_pThread = CreateThread< IteratedFunctionSystem::Thread >( m_sizeImage.x, 0, m_sizeImage.y, m_sizeImage.y, UbPixel() );
			l_pThread->Initialise( & ( m_pGenerator->GetInitCellsMap( i ) ), & ( m_pGenerator->GetInitCellsLst( i ) ), & ( m_pGenerator->GetInitCellsVct( i ) ), & m_backBuffer, & m_pGenerator );
			l_pThread->Run();
		}

		m_initialised = true;
	}

	void IteratedFunctionSystem::OnAngle( wxCommandEvent & p_event )
	{
		long l_lValue;
		p_event.GetString().ToLong( & l_lValue );
		SetGeneratorAngle( l_lValue );
	}

	void IteratedFunctionSystem::OnShift( wxCommandEvent & p_event )
	{
		long l_lValue;
		p_event.GetString().ToLong( & l_lValue );
		SetGeneratorShift( l_lValue );
	}

	void IteratedFunctionSystem::OnSizeIfs( wxCommandEvent & p_event )
	{
		long l_lValue;
		p_event.GetString().ToLong( & l_lValue );
		SetGeneratorSize( l_lValue );
	}

	void IteratedFunctionSystem::OnGenerator( wxCommandEvent & p_event )
	{
		SetGenerator( eIFS_GENERATOR( p_event.GetInt() ) );
	}
}
