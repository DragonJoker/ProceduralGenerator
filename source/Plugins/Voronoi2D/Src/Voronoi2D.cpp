#include "Voronoi2D.h"

#include <Random.h>

namespace ProceduralTextures
{
	Voronoi2D::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
		,	m_pBackBuffer( NULL )
		,	m_pSeeds( NULL )
		,	m_pHeights( NULL )
		,	m_pArrayColours( NULL )
		,	m_pArrayStepColours( NULL )
		,	m_iMaxDepth( 0 )
		,	m_iMaxNeighbours( 0 )
		,	m_iX( 0 )
		,	m_iY( m_iTop )
		,	m_dMkOrder( 4.0 )
		,	m_bComputed( false )
	{
		Random::srand( int( time( NULL ) ) );
		m_pFunctions[eDISTANCE_FUNC_EUCLIDIAN] = & Voronoi2D::Thread::EuclidianLength;
		m_pFunctions[eDISTANCE_FUNC_SQUARED_EUCLIDIAN] = & Voronoi2D::Thread::SquaredEuclidianLength;
		m_pFunctions[eDISTANCE_FUNC_MANHATTAN] = & Voronoi2D::Thread::ManhattanLength;
		m_pFunctions[eDISTANCE_FUNC_MINKOWSKI] = & Voronoi2D::Thread::MinkowskiLength;
		m_pFunctions[eDISTANCE_FUNC_CHEBYCHEV] = & Voronoi2D::Thread::ChebychevLength;
	}

	Voronoi2D::Thread::~Thread()
	{
	}

	void Voronoi2D::Thread::Step()
	{
		if ( ! m_bComputed )
		{
			if ( m_iX < m_iWidth )
			{
				for ( int j = m_iTop ; ! IsStopped() && j < m_iBottom ; j++ )
				{
					ComputeColour( m_iX, j, ( * m_pBackBuffer )[m_iX][j] );
				}

				m_iX++;
			}
			else
			{
				m_bComputed = true;
			}
		}
	}

	void Voronoi2D::Thread::Initialise( PixelBuffer * p_pBackBuffer, IntSetIntMap * p_pSeeds, IntIntMapIntMap * p_pHeights, ColourArray * p_pArrayColours, ColourArray * p_pArrayStepColours, eDISTANCE_FUNC p_eTypeDistance, int p_iMaxDepth, int p_iMaxNeighbours, double p_dMkOrder )
	{
		m_pBackBuffer = p_pBackBuffer;
		m_pSeeds = p_pSeeds;
		m_pHeights = p_pHeights;
		m_pArrayColours = p_pArrayColours;
		m_pArrayStepColours = p_pArrayStepColours;
		m_eTypeDistance = p_eTypeDistance;
		m_iMaxDepth = p_iMaxDepth;
		m_iMaxNeighbours = p_iMaxNeighbours;
		m_dMkOrder = p_dMkOrder;
		m_bComputed = false;
	}

	void Voronoi2D::Thread::ComputeColour( int x, int y, UbPixel & p_px )
	{
		Point2i l_ptCurrent( x, y );
		IntIntMap l_mapHeights;
		IntIntMap::reverse_iterator l_rit;
		int l_iDistance;

		for ( IntSetIntMap::const_iterator iti = m_pSeeds->begin() ; ! IsStopped() && iti != m_pSeeds->end() ; ++iti )
		{
			for ( IntSet::const_iterator itj = iti->second.begin() ; ! IsStopped() && itj != iti->second.end() ; ++itj )
			{
				l_iDistance = ( this->*m_pFunctions[m_eTypeDistance] )( l_ptCurrent, Point2i( iti->first, * itj ) );
				l_mapHeights.insert( IntIntMap::value_type( l_iDistance, ( * m_pHeights )[iti->first][* itj] ) );
			}
		}

		while ( ! IsStopped() && l_mapHeights.size() > ( size_t )m_iMaxNeighbours )
		{
			l_rit = l_mapHeights.rbegin();
			l_rit++;
			l_mapHeights.erase( l_rit.base() );
		}

		int l_iSum = 0;

		for ( IntIntMap::iterator l_it = l_mapHeights.begin() ; ! IsStopped() && l_it != l_mapHeights.end() ; ++l_it )
		{
			l_iSum += l_it->second;
		}

		const Colour & l_clr = ( * m_pArrayColours )[l_iSum / m_iMaxNeighbours];
		p_px.Set( l_clr[0], l_clr[1], l_clr[2], uint8_t( 255 ) );
	}

	int Voronoi2D::Thread::EuclidianLength( const Point2i & p_ptA, const Point2i & p_ptB )
	{
		return int( ( p_ptB - p_ptA ).GetLength() );
	}

	int Voronoi2D::Thread::SquaredEuclidianLength( const Point2i & p_ptA, const Point2i & p_ptB )
	{
		return int( ( p_ptB - p_ptA ).GetSquaredLength() );
	}

	int Voronoi2D::Thread::ManhattanLength( const Point2i & p_ptA, const Point2i & p_ptB )
	{
		return int( ( p_ptB - p_ptA ).GetManhattanLength() );
	}

	int Voronoi2D::Thread::MinkowskiLength( const Point2i & p_ptA, const Point2i & p_ptB )
	{
		return int( ( p_ptB - p_ptA ).GetMinkowskiLength( m_dMkOrder ) );
	}

	int Voronoi2D::Thread::ChebychevLength( const Point2i & p_ptA, const Point2i & p_ptB )
	{
		return int( ( p_ptB - p_ptA ).GetChebychevLength() );
	}

	//*************************************************************************************************

	Voronoi2D::Voronoi2D( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame, true, 1 )
		,	m_iMaxNeighbours( 32 )
		,	m_eTypeDistance( eDISTANCE_FUNC_EUCLIDIAN )
		,	m_dMkOrder( 4.0 )
		,	m_specStatDepthTitle( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Max Depth :" ) ) )
		,	m_specStatDepth( SpecificControlParameters< eCONTROL_TYPE_STATIC >( wxT( "5" ) ) )
		,	m_specSlideDepth( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 1, 100, 5 ) )
		,	m_specStatNeighboursTitle( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Neighbours :" ) ) )
		,	m_specStatNeighbours( SpecificControlParameters< eCONTROL_TYPE_STATIC >( wxT( "5" ) ) )
		,	m_specSlideNeighbours( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 1, 256, 32 ) )
		,	m_specStatDistance( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Distance Function :" ) ) )
		,	m_specComboDistance( SpecificControlParameters< eCONTROL_TYPE_COMBO >() )
		,	m_specStatMinkowski( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Minkowski Order : " ) + wxString( wxT( "4.0" ) ) ) )
		,	m_specSlideMinkowski( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 1, 256, 40 ) )
	{
		wxString l_arrayChoices[] =
		{
			_( "Euclidian" ),
			_( "Squared Euclidian" ),
			_( "Manhattan" ),
			_( "Minkowski" ),
			_( "Chebychev" ),
		};
		m_specComboDistance.Initialise( l_arrayChoices[0], l_arrayChoices );
		m_bNeedsConfigPanel = true;
		Random::srand( int( time( NULL ) ) );
		m_arrayStepColours.push_back( Voronoi2D::Colour( 000, 000, 000 ) );
		m_arrayStepColours.push_back( Voronoi2D::Colour( 255, 000, 000 ) );
		m_arrayStepColours.push_back( Voronoi2D::Colour( 255, 255, 000 ) );
		m_arrayStepColours.push_back( Voronoi2D::Colour( 000, 255, 000 ) );
		m_arrayStepColours.push_back( Voronoi2D::Colour( 000, 255, 255 ) );
		m_arrayStepColours.push_back( Voronoi2D::Colour( 000, 000, 255 ) );
		m_arrayStepColours.push_back( Voronoi2D::Colour( 255, 000, 255 ) );
		m_arrayStepColours.push_back( Voronoi2D::Colour( 255, 255, 255 ) );
		SetMaxDepth( 5 );
		DoGeneratePanel();
	}

	Voronoi2D::~Voronoi2D()
	{
		m_initialised = false;
		DoCleanup();
	}

	void Voronoi2D::SetMaxDepth( int p_iMaxDepth )
	{
		ClearAllThreads();
		m_iMaxDepth = p_iMaxDepth;
		m_arrayColours.clear();
		int l_iNbColours = int( m_iMaxDepth * ( m_arrayStepColours.size() - 1 ) );
		m_arrayColours.reserve( l_iNbColours );
		int l_iSteps = m_iMaxDepth - 1;
		int l_iCurrent = 0, l_iPrevious;
		Colour l_clrColour;
		Point3d l_ptStep;
		Point3d l_ptColour;

		for ( int i = 0 ; i < l_iNbColours ; )
		{
			l_iPrevious = l_iCurrent++;
			l_clrColour = m_arrayStepColours[l_iPrevious];
			l_ptColour = m_arrayStepColours[l_iPrevious];
			l_ptStep = ( Point3d( m_arrayStepColours[l_iCurrent] ) - l_ptColour ) / l_iSteps;

			for ( int j = 0 ; j <= l_iSteps ; j++, i++ )
			{
				m_arrayColours.push_back( l_ptColour );
				l_ptColour += l_ptStep;
			}
		}

		Initialise();
		Generate();
	}

	void Voronoi2D::SetMaxNeighbours( int p_iMaxNeighbours )
	{
		ClearAllThreads();
		m_iMaxNeighbours = p_iMaxNeighbours;
		Generate();
	}

	void Voronoi2D::SetTypeDistance( eDISTANCE_FUNC p_eType )
	{
		ClearAllThreads();
		m_eTypeDistance = p_eType;
		Generate();
	}

	void Voronoi2D::SetMinkowskiOrder( double p_dMkOrder )
	{
		ClearAllThreads();
		m_dMkOrder = p_dMkOrder;
		Generate();
	}

	void Voronoi2D::Initialise()
	{
		m_seeds.clear();
		m_heights.clear();
		int l_iNbSeedsX = int( sqrt( float( m_sizeImage.x ) ) );
		int l_iNbSeedsY = int( sqrt( float( m_sizeImage.y ) ) );
		int x, y;
		int l_iIndex;
		IntSetIntMap::iterator l_itSeeds1;
		IntSet::iterator l_itSeeds2;
		IntIntMapIntMap::iterator l_itHeights1;

		for ( int i = 0 ; i < l_iNbSeedsX ; i++ )
		{
			for ( int j = 0 ; j < l_iNbSeedsY ; j++ )
			{
				x = Random::rand() % m_sizeImage.x;
				y = Random::rand() % m_sizeImage.y;
				l_itSeeds1 = m_seeds.find( x );
				l_itHeights1 = m_heights.find( x );

				while ( l_itSeeds1 != m_seeds.end() )
				{
					x = Random::rand() % m_sizeImage.x;
					l_itSeeds1 = m_seeds.find( x );
				}

				m_setX.insert( x );
				m_seeds.insert( IntSetIntMap::value_type( x, IntSet() ) );
				m_heights.insert( IntIntMapIntMap::value_type( x, IntIntMap() ) );
				l_itSeeds1 = m_seeds.find( x );
				l_itHeights1 = m_heights.find( x );
				l_itSeeds2 = l_itSeeds1->second.find( y );

				while ( l_itSeeds2 != l_itSeeds1->second.end() )
				{
					y = Random::rand() % m_sizeImage.y;
					l_itSeeds2 = l_itSeeds1->second.find( y );
				}

				m_setY.insert( y );
				l_itSeeds1->second.insert( y );
				l_iIndex = Random::rand() % m_arrayColours.size();
				l_itHeights1->second.insert( IntIntMap::value_type( y, l_iIndex ) );
			}
		}
	}

	void Voronoi2D::Generate()
	{
		Voronoi2D::Thread * l_pThread;
		int l_iOffset = int( m_sizeImage.y / m_uiThreadCount );
		int l_iAdd;

		if ( int( l_iOffset * m_uiThreadCount ) == m_sizeImage.y )
		{
			l_iAdd = 0;
		}
		else
		{
			l_iAdd = m_sizeImage.y - int( l_iOffset * m_uiThreadCount );
		}

		for ( int i = 0 ; i < m_sizeImage.y ; i += l_iOffset )
		{
			l_pThread = CreateThread< Voronoi2D::Thread >( m_sizeImage.x, i, i + l_iOffset + l_iAdd, m_sizeImage.y, m_pxColour );
			l_pThread->Initialise( & m_backBuffer, & m_seeds, & m_heights, & m_arrayColours, & m_arrayStepColours, m_eTypeDistance, m_iMaxDepth, m_iMaxNeighbours, m_dMkOrder );
			l_pThread->Run();
			i += l_iAdd;
			l_iAdd = 0;
		}

		m_initialised = true;
	}

	void Voronoi2D::DoGeneratePanel()
	{
		int iWidth = 150;
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,				wxPoint( 10, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specStatDepthTitle ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,				wxPoint( 90, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth - 100,  wxDEFAULT_HEIGHT ), &m_specStatDepth ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_DEPTH,				wxPoint( 10, wxDEFAULT_HEIGHT * 1 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specSlideDepth,			wxSL_HORIZONTAL,	eEVENT_TYPE_SCROLL_THUMBRELEASE,	wxScrollEventHandler(	Voronoi2D::OnMaxDepths ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,				wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specStatNeighboursTitle ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,				wxPoint( 90, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth - 100,  wxDEFAULT_HEIGHT ), &m_specStatNeighbours ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_NEIGHBOURS,			wxPoint( 10, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specSlideNeighbours,		wxSL_HORIZONTAL,	eEVENT_TYPE_SCROLL_THUMBRELEASE,	wxScrollEventHandler(	Voronoi2D::OnMaxNeighbours ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,				wxPoint( 10, wxDEFAULT_HEIGHT * 4 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specStatDistance ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,	eID_DISTANCE_FUNC,		wxPoint( 10, wxDEFAULT_HEIGHT * 5 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specComboDistance,		wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED,		wxCommandEventHandler(	Voronoi2D::OnDistanceType ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,				wxPoint( 10, wxDEFAULT_HEIGHT * 7 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specStatMinkowski,		0,					eEVENT_TYPE_NONE,					NULL,													false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_MINKOWSKI_ORDER,	wxPoint( 10, wxDEFAULT_HEIGHT * 8 ), wxSize( iWidth -  20,  wxDEFAULT_HEIGHT ), &m_specSlideMinkowski,		wxSL_HORIZONTAL,	eEVENT_TYPE_SCROLL_THUMBRELEASE,	wxScrollEventHandler(	Voronoi2D::OnMkOrder ),	false ) );
	}

	void Voronoi2D::OnMaxDepths( wxScrollEvent & p_event )
	{
		int l_iValue = p_event.GetInt();

		if ( IsInitialised() )
		{
			SetMaxDepth( l_iValue );
			wxString l_strValue;
			l_strValue << l_iValue;
			m_specStatDepth.m_pControl->SetLabel( l_strValue );
		}
	}

	void Voronoi2D::OnMaxNeighbours( wxScrollEvent & p_event )
	{
		int l_iValue = p_event.GetInt();

		if ( IsInitialised() )
		{
			SetMaxNeighbours( l_iValue );
			wxString l_strValue;
			l_strValue << l_iValue;
			m_specStatNeighbours.m_pControl->SetLabel( l_strValue );
		}
	}

	void Voronoi2D::OnMkOrder( wxScrollEvent & p_event )
	{
		double l_dValue = double( p_event.GetInt() ) / 10.0;

		if ( IsInitialised() )
		{
			SetMinkowskiOrder( l_dValue );
			wxString l_strValue;
			l_strValue << _( "Minkowski Order : " ) << l_dValue;
			m_specStatMinkowski.m_pControl->SetLabel( l_strValue );
		}
	}

	void Voronoi2D::OnDistanceType( wxCommandEvent & p_event )
	{
		Voronoi2D::eDISTANCE_FUNC l_eValue = Voronoi2D::eDISTANCE_FUNC( p_event.GetInt() );

		if ( IsInitialised() )
		{
			SetTypeDistance( l_eValue );

			if ( l_eValue == Voronoi2D::eDISTANCE_FUNC_MINKOWSKI )
			{
				m_specStatMinkowski.m_pControl->Show();
				m_specSlideMinkowski.m_pControl->Show();
			}
			else
			{
				m_specStatMinkowski.m_pControl->Hide();
				m_specSlideMinkowski.m_pControl->Hide();
			}
		}
	}

	void Voronoi2D::OnMaxDepthsTrack( wxScrollEvent & p_event )
	{
		int l_iValue = p_event.GetInt();
		wxString l_strValue;
		l_strValue << l_iValue;
		m_specStatDepth.m_pControl->SetLabel( l_strValue );
	}

	void Voronoi2D::OnMaxNeighboursTrack( wxScrollEvent & p_event )
	{
		int l_iValue = p_event.GetInt();
		wxString l_strValue;
		l_strValue << l_iValue;
		m_specStatNeighbours.m_pControl->SetLabel( l_strValue );
	}

	void Voronoi2D::OnMkOrderTrack( wxScrollEvent & p_event )
	{
		double l_dValue = double( p_event.GetInt() ) / 10.0;
		wxString l_strValue;
		l_strValue << _( "Minkowski Order : " ) << l_dValue;
		m_specStatMinkowski.m_pControl->SetLabel( l_strValue );
	}
}
