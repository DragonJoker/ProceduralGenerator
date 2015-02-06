#include "PerlinNoise.h"

namespace ProceduralTextures
{
	static int permutation[] =
	{
		151,	160,	137,	91,		90,		15,		131,	13,
		201,	95,		96,		53,		194,	233,	7,		225,
		140,	36,		103,	30,		69,		142,	8,		99,
		37,		240,	21,		10,		23,		190,	6,		148,
		247,	120,	234,	75,		0,		26,		197,	62,
		94,		252,	219,	203,	117,	35,		11,		32,
		57,		177,	33,		88,		237,	149,	56,		87,
		174,	20,		125,	136,	171,	168,	68,		175,
		74,		165,	71,		134,	139,	48,		27,		166,
		77,		146,	158,	231,	83,		111,	229,	122,
		60,		211,	133,	230,	220,	105,	92,		41,
		55,		46,		245,	40,		244,	102,	143,	54,
		65,		25,		63,		161,	1,		216,	80,		73,
		209,	76,		132,	187,	208,	89,		18,		169,
		200,	196,	135,	130,	116,	188,	159,	86,
		164,	100,	109,	198,	173,	186,	3,		64,
		52,		217,	226,	250,	124,	123,	5,		202,
		38,		147,	118,	126,	255,	82,		85,		212,
		207,	206,	59,		227,	47,		16,		58,		17,
		182,	189,	28,		42,		23,		183,	170,	213,
		119,	248,	152,	2,		44,		154,	163,	70,
		221,	153,	101,	155,	167,	43,		172,	9,
		129,	22,		39,		253,	19,		98,		108,	110,
		79,		113,	224,	232,	178,	185,	112,	104,
		218,	246,	97,		228,	251,	34,		242,	193,
		238,	210,	144,	12,		191,	179,	162,	241,
		81,		51,		145,	235,	249,	14,		239,	107,
		49,		192,	214,	31,		181,	199,	106,	157,
		184,	84,		204,	176,	115,	121,	50,		45,
		127,	4,		150,	254,	138,	236,	205,	93,
		222,	114,	67,		29,		24,		72,		243,	141,
		128,	195,	78,		66,		215,	61,		156,	180
	};

	//*************************************************************************************************

	PerlinNoise::Thread::Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour )
		:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour )
		,	m_eType( eNOISE_TYPE_TURBULENCE )
		,	m_iNumberOfOctaves( 1 )
		,	m_fPersistency( 1.0 )
		,	m_iStep( 1 )
		,	m_pixels( NULL )
		,	m_pfnNoise( & cosf )
		,	m_pfnType( NULL )
	{
		for ( int i = 0; i < eNOISE_TYPE_COUNT ; i++ )
		{
			switch ( i )
			{
			case eNOISE_TYPE_TURBULENCE:
				m_pfnArrayTypes[i] = & PerlinNoise::Thread::_turbulence;
				break;

			case eNOISE_TYPE_CLOUD:
				m_pfnArrayTypes[i] = & PerlinNoise::Thread::_clouds;
				break;

			case eNOISE_TYPE_MARBLE:
				m_pfnArrayTypes[i] = & PerlinNoise::Thread::_marble;
				break;

			case eNOISE_TYPE_PARAM:
				m_pfnArrayTypes[i] = & PerlinNoise::Thread::_param;
				break;
			}
		}

		m_pfnType = m_pfnArrayTypes[eNOISE_TYPE_TURBULENCE];

		for ( int i = 0; i < 256 ; i++ )
		{
			m_iPermutations[256 + i] = m_iPermutations[i] = permutation[i];
		}
	}

	PerlinNoise::Thread::~Thread()
	{
	}

	void PerlinNoise::Thread::Step()
	{
		for ( int y = m_iTop ; y < m_iBottom ; y++ )
		{
			for ( int x = 0 ; x < m_iWidth ; x++ )
			{
				( this->*m_pfnType )( x, y, m_iStep, ( * m_pixels )[y][x] );
			}
		}

		m_iStep++;
	}

	void PerlinNoise::Thread::SetType( eNOISE_TYPE p_eType )
	{
		m_eType = p_eType;
		m_pfnType = m_pfnArrayTypes[p_eType];
	}

	void PerlinNoise::Thread::SetFunction( NoiseFunction p_pfnFunction )
	{
		m_pfnNoise = p_pfnFunction;
	}

	void PerlinNoise::Thread::_turbulence( int x, int y, int z, UbPixel & p_rPixel )
	{
		float l_fNoiseCoeff = 0.0f;

		for ( int l_iLevel = 1; l_iLevel < m_iNumberOfOctaves ; l_iLevel++ )
		{
			l_fNoiseCoeff += ( 1.0f / l_iLevel ) * fabsf( float( _noise( l_iLevel * m_fPersistency * x, l_iLevel * m_fPersistency * y, l_iLevel * m_fPersistency * z ) ) );
		}

		int iColour = ( int( l_fNoiseCoeff * 255.0f ) << 1 ) | 0x80;

		if ( iColour > 255 )
		{
			iColour = 255;
		}
		else if ( iColour < 0 )
		{
			iColour = 0;
		}

		p_rPixel.a = 255;
		p_rPixel.r = std::min( static_cast< uint8_t >( iColour ), m_pxColour.r );
		p_rPixel.g = std::min( static_cast< uint8_t >( iColour ), m_pxColour.g );
		p_rPixel.b = std::min( static_cast< uint8_t >( iColour ), m_pxColour.b );
	}

	void PerlinNoise::Thread::_clouds( int x, int y, int z, UbPixel & p_rPixel )
	{
		float l_fNoiseCoeff = 0.0f;

		for ( int l_iLevel = 1; l_iLevel < m_iNumberOfOctaves ; l_iLevel++ )
		{
			l_fNoiseCoeff += ( 1.0f / l_iLevel ) * fabsf( float( _noise( l_iLevel * m_fPersistency * x, l_iLevel * m_fPersistency * y, l_iLevel * m_fPersistency * z ) ) );
		}

		int iColour = int( l_fNoiseCoeff * 255.0f );

		if ( iColour > 255 )
		{
			iColour = 255;
		}
		else if ( iColour < 0 )
		{
			iColour = 0;
		}

		p_rPixel.a = 255;
		p_rPixel.r = std::min( static_cast< uint8_t >( iColour ), m_pxColour.r );
		p_rPixel.g = std::min( static_cast< uint8_t >( iColour ), m_pxColour.g );
		p_rPixel.b = std::min( static_cast< uint8_t >( iColour ), m_pxColour.b );
	}

	void PerlinNoise::Thread::_marble( int x, int y, int z, UbPixel & p_rPixel )
	{
		float l_fNoiseCoeff = 0.0f;

		for ( int l_iLevel = 1; l_iLevel < m_iNumberOfOctaves ; l_iLevel++ )
		{
			l_fNoiseCoeff += ( 1.0f / l_iLevel ) * fabsf( float( _noise( l_iLevel * m_fPersistency * x, l_iLevel * m_fPersistency * y, l_iLevel * m_fPersistency * z ) ) );
		}

		l_fNoiseCoeff = 0.5f * sinf( ( x + y ) * m_fPersistency + l_fNoiseCoeff ) + 0.5f;
		int iColour = int( l_fNoiseCoeff * 255.0f );

		if ( iColour > 255 )
		{
			iColour = 255;
		}
		else if ( iColour < 0 )
		{
			iColour = 0;
		}

		p_rPixel.a = 255;
		p_rPixel.r = std::min( static_cast< uint8_t >( iColour ), m_pxColour.r );
		p_rPixel.g = std::min( static_cast< uint8_t >( iColour ), m_pxColour.g );
		p_rPixel.b = std::min( static_cast< uint8_t >( iColour ), m_pxColour.b );
	}

	void PerlinNoise::Thread::_param( int x, int y, int z, UbPixel & p_rPixel )
	{
		float l_fNoiseCoeff = 0.0f;

		for ( int l_iLevel = 1; l_iLevel < m_iNumberOfOctaves ; l_iLevel++ )
		{
			l_fNoiseCoeff += ( 1.0f / l_iLevel ) * fabsf( float( _noise( l_iLevel * m_fPersistency * x, l_iLevel * m_fPersistency * y, l_iLevel * m_fPersistency * z ) ) );
		}

		l_fNoiseCoeff = 0.5f * ( * m_pfnNoise )( ( x + y ) * m_fPersistency + l_fNoiseCoeff ) + 0.5f;
		int iColour = int( l_fNoiseCoeff * 255.0f );

		if ( iColour > 255 )
		{
			iColour = 255;
		}
		else if ( iColour < 0 )
		{
			iColour = 0;
		}

		p_rPixel.a = 255;
		p_rPixel.r = std::min( static_cast< uint8_t >( iColour ), m_pxColour.r );
		p_rPixel.g = std::min( static_cast< uint8_t >( iColour ), m_pxColour.g );
		p_rPixel.b = std::min( static_cast< uint8_t >( iColour ), m_pxColour.b );
	}

	double PerlinNoise::Thread::_fade( double t )
	{
		return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
	}

	double PerlinNoise::Thread::_lerp( double t, double a, double b )
	{
		return a + t * ( b - a );
	}

	double PerlinNoise::Thread::_grad( int hash, double x, double y, double z )
	{
		int h = hash & 15;
		// CONVERT LOW 4 BITS OF HASH CODE
		double u = h < 8 || h == 12 || h == 13 ? x : y,	// INTO 12 GRADIENT DIRECTIONS.
			   v = h < 4 || h == 12 || h == 13 ? y : z;
		return ( ( h & 1 ) == 0 ? u : -u ) + ( ( h & 2 ) == 0 ? v : -v );
	}

	double PerlinNoise::Thread::_noise( double x, double y, double z )
	{
		int X = ( int )floor( x ) & 255;												// FIND UNIT CUBE THAT
		int Y = ( int )floor( y ) & 255;												// CONTAINS POINT.
		int Z = ( int )floor( z ) & 255;
		x -= floor( x );																// FIND RELATIVE X,Y,Z
		y -= floor( y );																// OF POINT IN CUBE.
		z -= floor( z );
		double u = _fade( x );														// COMPUTE FADE CURVES
		double v = _fade( y );														// FOR EACH OF X,Y,Z.
		double w = _fade( z );
		int A = m_iPermutations[X] + Y;												// HASH COORDINATES OF
		int AA = m_iPermutations[A] + Z;												// THE 8 CUBE CORNERS,
		int AB = m_iPermutations[A + 1] + Z;
		int B = m_iPermutations[X + 1] + Y;
		int BA = m_iPermutations[B] + Z;
		int BB = m_iPermutations[B + 1] + Z;
		return
			_lerp( w, _lerp( v, _lerp( u, _grad( m_iPermutations[AA], x, y, z ),		// AND ADD
									   _grad( m_iPermutations[BA], x - 1, y, z ) ),								// BLENDED
							 _lerp( u, _grad( m_iPermutations[AB], x, y - 1, z ),						// RESULTS
									_grad( m_iPermutations[BB], x - 1, y - 1, z ) ) ),								// FROM 8
				   _lerp( v, _lerp( u, _grad( m_iPermutations[AA + 1], x, y, z - 1 ),			// CORNERS
									_grad( m_iPermutations[BA + 1], x - 1, y, z - 1 ) ),							// OF CUBE
						  _lerp( u, _grad( m_iPermutations[AB + 1], x, y - 1, z - 1 ),
								 _grad( m_iPermutations[BB + 1], x - 1, y - 1, z - 1 ) ) ) );
	}

	//*************************************************************************************************

	PerlinNoise::PerlinNoise( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame, int p_iNoo, float p_fP, eNOISE_TYPE p_eType )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame )
		,	m_iNumberOfOctaves( p_iNoo )
		,	m_fPersistency( p_fP )
		,	m_iStep( 1 )
		,	m_eType( p_eType )
		,	m_specStatR( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "R" ) ) )
		,	m_specSlideR( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.r ) )
		,	m_specStatG( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "G" ) ) )
		,	m_specSlideG( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.g ) )
		,	m_specStatB( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "B" ) ) )
		,	m_specSlideB( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 0, 255, m_pxColour.b ) )
		,	m_specStatOctavesTitle( SpecificControlParameters< eCONTROL_TYPE_STATIC	 >( _( "Octaves : " ) ) )
		,	m_specStatOctaves( SpecificControlParameters< eCONTROL_TYPE_STATIC >( wxT( "2" ) ) )
		,	m_specSlideOctaves( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 2, 100, p_iNoo ) )
		,	m_specStatFrequencyTitle( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Frequency : " ) ) )
		,	m_specStatFrequency( SpecificControlParameters< eCONTROL_TYPE_STATIC >( wxT( "50" ) ) )
		,	m_specSlideFrequency( SpecificControlParameters< eCONTROL_TYPE_SLIDER >( 2, 1000, 50 ) )
		,	m_specStatType( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Type : " ) ) )
		,	m_specComboType( SpecificControlParameters< eCONTROL_TYPE_COMBO >() )
		,	m_specComboFunction( SpecificControlParameters< eCONTROL_TYPE_COMBO  >() )
	{
		wxString l_arrayFuncNames[eNOISE_PARAM_FUNCTION_COUNT] =
		{
			wxT( "cos" ),	wxT( "sin" ),	wxT( "tan" ),
			wxT( "cosh" ),	wxT( "sinh" ),	wxT( "tanh" ),
			wxT( "acos" ),	wxT( "asin" ),	wxT( "atan" ),
			wxT( "sqrt" ),
			wxT( "ln" ),	wxT( "exp" ),	wxT( "log" ),
		};
		wxString l_arrayTypeNames[eNOISE_TYPE_COUNT] =
		{
			_( "Turbulence" ),
			_( "Cloud" ),
			_( "Marble" ),
			_( "Parametric" ),
		};
		m_specComboType.Initialise( l_arrayTypeNames[0], l_arrayTypeNames );
		m_specComboFunction.Initialise( l_arrayFuncNames[0], l_arrayFuncNames );
		m_bNeedsConfigPanel = true;
		PerlinNoise::Thread * l_pThread;
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
			l_pThread = CreateThread< PerlinNoise::Thread >( p_width, i, i + l_iOffset + l_iAdd, p_height, m_pxColour );
			l_pThread->SetBuffer( & m_backBuffer );
			l_pThread->m_iNumberOfOctaves = m_iNumberOfOctaves;
			l_pThread->m_fPersistency = m_fPersistency;
			l_pThread->m_eType = m_eType;
			l_pThread->Run();
			i += l_iAdd;
			l_iAdd = 0;
		}

		for ( int i = 0 ; i < eNOISE_PARAM_FUNCTION_COUNT ; i++ )
		{
			switch ( i )
			{
			case eNOISE_PARAM_FUNCTION_COS:
				m_functions[i] = &cosf;
				break;

			case eNOISE_PARAM_FUNCTION_SIN:
				m_functions[i] = &sinf;
				break;

			case eNOISE_PARAM_FUNCTION_TAN:
				m_functions[i] = &tanf;
				break;

			case eNOISE_PARAM_FUNCTION_COSH:
				m_functions[i] = &coshf;
				break;

			case eNOISE_PARAM_FUNCTION_SINH:
				m_functions[i] = &sinhf;
				break;

			case eNOISE_PARAM_FUNCTION_TANH:
				m_functions[i] = &tanhf;
				break;

			case eNOISE_PARAM_FUNCTION_ACOS:
				m_functions[i] = &acosf;
				break;

			case eNOISE_PARAM_FUNCTION_ASIN:
				m_functions[i] = &asinf;
				break;

			case eNOISE_PARAM_FUNCTION_ATAN:
				m_functions[i] = &atanf;
				break;

			case eNOISE_PARAM_FUNCTION_SQRT:
				m_functions[i] = &sqrtf;
				break;

			case eNOISE_PARAM_FUNCTION_LN:
				m_functions[i] = &logf;
				break;

			case eNOISE_PARAM_FUNCTION_EXP:
				m_functions[i] = &expf;
				break;

			case eNOISE_PARAM_FUNCTION_LOG10:
				m_functions[i] = &log10f;
				break;
			}
		}

		DoGeneratePanel();
		m_initialised = true;
	}

	PerlinNoise::~PerlinNoise()
	{
		DoCleanup();
	}

	void PerlinNoise::SetNumberOfOctaves( int val )
	{
		m_iNumberOfOctaves = val;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				( ( PerlinNoise::Thread * )m_arraySlaveThreads[i] )->m_iNumberOfOctaves = val;
			}
		}
	}

	void PerlinNoise::SetPersistency( float val )
	{
		m_fPersistency = val;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				( ( PerlinNoise::Thread * )m_arraySlaveThreads[i] )->m_fPersistency = val;
			}
		}
	}

	void PerlinNoise::SetType( eNOISE_TYPE p_eType )
	{
		m_eType = p_eType;

		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				( ( PerlinNoise::Thread * )m_arraySlaveThreads[i] )->SetType( p_eType );
			}
		}
	}

	void PerlinNoise::SetFunction( eNOISE_PARAM_FUNCTION p_eFunction )
	{
		for ( size_t i = 0 ; i < m_arraySlaveThreads.size() ; i++ )
		{
			if ( m_arraySlaveThreads[i] != NULL )
			{
				( ( PerlinNoise::Thread * )m_arraySlaveThreads[i] )->SetFunction( m_functions[p_eFunction] );
			}
		}
	}

	void PerlinNoise::DoGeneratePanel()
	{
		int iWidth = 150;
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 0 ), wxSize( 20, wxDEFAULT_HEIGHT ), &m_specStatR ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_REDVALUE,	wxPoint( 30, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth -  40, wxDEFAULT_HEIGHT ), &m_specSlideR,				wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	PerlinNoise::OnSliderRed ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 1 ), wxSize( 20, wxDEFAULT_HEIGHT ), &m_specStatG ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_GREENVALUE,	wxPoint( 30, wxDEFAULT_HEIGHT * 1 ), wxSize( iWidth -  40, wxDEFAULT_HEIGHT ), &m_specSlideG,				wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	PerlinNoise::OnSliderGreen ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( 20, wxDEFAULT_HEIGHT ), &m_specStatB ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_BLUEVALUE,	wxPoint( 30, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  40, wxDEFAULT_HEIGHT ), &m_specSlideB,				wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	PerlinNoise::OnSliderBlue ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStatOctavesTitle ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 90, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth - 100, wxDEFAULT_HEIGHT ), &m_specStatOctaves ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_OCTAVES,	wxPoint( 10, wxDEFAULT_HEIGHT * 4 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specSlideOctaves,			wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	PerlinNoise::OnOctaves ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 5 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStatFrequencyTitle ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 90, wxDEFAULT_HEIGHT * 5 ), wxSize( iWidth - 100, wxDEFAULT_HEIGHT ), &m_specStatFrequency ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_SLIDER,	eID_FREQUENCY,	wxPoint( 10, wxDEFAULT_HEIGHT * 6 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specSlideFrequency,		wxSL_HORIZONTAL,	eEVENT_TYPE_SLIDER_UPDATED,		wxCommandEventHandler(	PerlinNoise::OnFrequency ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,	wxID_ANY,		wxPoint( 10, wxDEFAULT_HEIGHT * 7 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStatType ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,	eID_TYPE,		wxPoint( 10, wxDEFAULT_HEIGHT * 8 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboType,			wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED, wxCommandEventHandler(	PerlinNoise::OnType ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,	eID_FUNCTION,	wxPoint( 10, wxDEFAULT_HEIGHT * 9 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboFunction,		wxCB_READONLY,		eEVENT_TYPE_COMBOBOX_SELECTED, wxCommandEventHandler(	PerlinNoise::OnFunction ), false ) );
	}

	void PerlinNoise::OnSliderRed( wxCommandEvent & p_event )
	{
		SetRed( uint8_t( p_event.GetInt() ) );
	}

	void PerlinNoise::OnSliderGreen( wxCommandEvent & p_event )
	{
		SetGreen( uint8_t( p_event.GetInt() ) );
	}

	void PerlinNoise::OnSliderBlue( wxCommandEvent & p_event )
	{
		SetBlue( uint8_t( p_event.GetInt() ) );
	}

	void PerlinNoise::OnOctaves( wxCommandEvent & p_event )
	{
		int l_iValue = p_event.GetInt();
		SetNumberOfOctaves( l_iValue );
		wxString l_strValue;
		l_strValue << l_iValue;
		m_specStatOctaves.m_pControl->SetLabel( l_strValue );
	}

	void PerlinNoise::OnFrequency( wxCommandEvent & p_event )
	{
		int l_iValue = p_event.GetInt();
		SetPersistency( l_iValue / 1000.0f );
		wxString l_strValue;
		l_strValue << int( GetPersistency() * 1000.0 );
		m_specStatFrequency.m_pControl->SetLabel( l_strValue );
	}

	void PerlinNoise::OnType( wxCommandEvent & p_event )
	{
		wxString l_strChoice = p_event.GetString();
		SetType( PerlinNoise::eNOISE_TYPE( p_event.GetInt() ) );

		if ( p_event.GetInt() == PerlinNoise::eNOISE_TYPE_PARAM )
		{
			m_specComboFunction.m_pControl->Show();
		}
		else
		{
			m_specComboFunction.m_pControl->Hide();
		}
	}

	void PerlinNoise::OnFunction( wxCommandEvent & p_event )
	{
		SetFunction( PerlinNoise::eNOISE_PARAM_FUNCTION( p_event.GetInt() ) );
	}
}
