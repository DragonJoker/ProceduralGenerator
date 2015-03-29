#include "PlnCpuStep.h"

using namespace ProceduralTextures;

namespace PerlinNoise
{
	int Permutations[] =
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

	Thread::Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight )
		:	ProceduralTextures::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight )
		,	m_type( eNOISE_TYPE_TURBULENCE )
		,	m_octavesCount( 2 )
		,	m_persistency( 0.05f )
		,	m_step( 1 )
		,	m_paramNoise( &cosf )
		,	m_noise()
		,	m_colour( 255, 255, 255, 255 )
	{
		for ( int i = 0; i < eNOISE_TYPE_COUNT; i++ )
		{
			switch ( i )
			{
			case eNOISE_TYPE_TURBULENCE:
				m_noiseFunctions[i] = std::bind( &Thread::DoTurbulence, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
				break;

			case eNOISE_TYPE_CLOUD:
				m_noiseFunctions[i] = std::bind( &Thread::DoClouds, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
				break;

			case eNOISE_TYPE_MARBLE:
				m_noiseFunctions[i] = std::bind( &Thread::DoMarble, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
				break;

			case eNOISE_TYPE_PARAM:
				m_noiseFunctions[i] = std::bind( &Thread::DoParam, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 );
				break;
			}
		}

		m_noise = m_noiseFunctions[eNOISE_TYPE_TURBULENCE];

		for ( int i = 0; i < 256; i++ )
		{
			m_permutations[256 + i] = m_permutations[i] = Permutations[i];
		}
	}

	Thread::~Thread()
	{
	}

	void Thread::DoStep()
	{
		std::unique_lock< std::mutex > l_lock( m_mutexNoise );
		PixelBuffer & l_pixels = *m_pixels.lock();

		for ( int y = m_iTop; y < m_iBottom; y++ )
		{
			for ( int x = 0; x < m_iWidth; x++ )
			{
				m_noise( x, y, m_step, l_pixels[y][x] );
			}
		}

		m_step++;
	}

	void Thread::SetType( eNOISE_TYPE p_eType )
	{
		std::unique_lock< std::mutex > l_lock( m_mutexNoise );
		m_type = p_eType;
		m_noise = m_noiseFunctions[p_eType];
	}

	void Thread::SetFunction( NoiseFunction p_pfnFunction )
	{
		std::unique_lock< std::mutex > l_lock( m_mutexNoise );
		m_paramNoise = p_pfnFunction;
	}

	void Thread::DoTurbulence( int x, int y, int z, UbPixel & p_rPixel )
	{
		float l_fNoiseCoeff = 0.0f;

		for ( int l_iLevel = 1; l_iLevel < m_octavesCount; l_iLevel++ )
		{
			l_fNoiseCoeff += ( 1.0f / l_iLevel ) * fabsf( float( DoNoise( l_iLevel * m_persistency * x, l_iLevel * m_persistency * y, l_iLevel * m_persistency * z ) ) );
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
		p_rPixel.r = std::min( static_cast< uint8_t >( iColour ), m_colour.r );
		p_rPixel.g = std::min( static_cast< uint8_t >( iColour ), m_colour.g );
		p_rPixel.b = std::min( static_cast< uint8_t >( iColour ), m_colour.b );
	}

	void Thread::DoClouds( int x, int y, int z, UbPixel & p_rPixel )
	{
		float l_fNoiseCoeff = 0.0f;

		for ( int l_iLevel = 1; l_iLevel < m_octavesCount; l_iLevel++ )
		{
			l_fNoiseCoeff += ( 1.0f / l_iLevel ) * fabsf( float( DoNoise( l_iLevel * m_persistency * x, l_iLevel * m_persistency * y, l_iLevel * m_persistency * z ) ) );
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
		p_rPixel.r = std::min( static_cast< uint8_t >( iColour ), m_colour.r );
		p_rPixel.g = std::min( static_cast< uint8_t >( iColour ), m_colour.g );
		p_rPixel.b = std::min( static_cast< uint8_t >( iColour ), m_colour.b );
	}

	void Thread::DoMarble( int x, int y, int z, UbPixel & p_rPixel )
	{
		float l_fNoiseCoeff = 0.0f;

		for ( int l_iLevel = 1; l_iLevel < m_octavesCount; l_iLevel++ )
		{
			l_fNoiseCoeff += ( 1.0f / l_iLevel ) * fabsf( float( DoNoise( l_iLevel * m_persistency * x, l_iLevel * m_persistency * y, l_iLevel * m_persistency * z ) ) );
		}

		l_fNoiseCoeff = 0.5f * sinf( ( x + y ) * m_persistency + l_fNoiseCoeff ) + 0.5f;
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
		p_rPixel.r = std::min( static_cast< uint8_t >( iColour ), m_colour.r );
		p_rPixel.g = std::min( static_cast< uint8_t >( iColour ), m_colour.g );
		p_rPixel.b = std::min( static_cast< uint8_t >( iColour ), m_colour.b );
	}

	void Thread::DoParam( int x, int y, int z, UbPixel & p_rPixel )
	{
		float l_fNoiseCoeff = 0.0f;

		for ( int l_iLevel = 1; l_iLevel < m_octavesCount; l_iLevel++ )
		{
			l_fNoiseCoeff += ( 1.0f / l_iLevel ) * fabsf( float( DoNoise( l_iLevel * m_persistency * x, l_iLevel * m_persistency * y, l_iLevel * m_persistency * z ) ) );
		}

		l_fNoiseCoeff = 0.5f * m_paramNoise( ( x + y ) * m_persistency + l_fNoiseCoeff ) + 0.5f;
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
		p_rPixel.r = std::min( static_cast< uint8_t >( iColour ), m_colour.r );
		p_rPixel.g = std::min( static_cast< uint8_t >( iColour ), m_colour.g );
		p_rPixel.b = std::min( static_cast< uint8_t >( iColour ), m_colour.b );
	}

	double Thread::DoFade( double t )
	{
		return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
	}

	double Thread::DoLerp( double t, double a, double b )
	{
		return a + t * ( b - a );
	}

	double Thread::DoGrad( int hash, double x, double y, double z )
	{
		int h = hash & 15;
		// CONVERT LOW 4 BITS OF HASH CODE
		double u = h < 8 || h == 12 || h == 13 ? x : y,	// INTO 12 GRADIENT DIRECTIONS.
			   v = h < 4 || h == 12 || h == 13 ? y : z;
		return ( ( h & 1 ) == 0 ? u : -u ) + ( ( h & 2 ) == 0 ? v : -v );
	}

	double Thread::DoNoise( double x, double y, double z )
	{
		int X = ( int )floor( x ) & 255;												// FIND UNIT CUBE THAT
		int Y = ( int )floor( y ) & 255;												// CONTAINS POINT.
		int Z = ( int )floor( z ) & 255;
		x -= floor( x );																// FIND RELATIVE X,Y,Z
		y -= floor( y );																// OF POINT IN CUBE.
		z -= floor( z );
		double u = DoFade( x );															// COMPUTE FADE CURVES
		double v = DoFade( y );															// FOR EACH OF X,Y,Z.
		double w = DoFade( z );
		int A = m_permutations[X] + Y;													// HASH COORDINATES OF
		int AA = m_permutations[A] + Z;													// THE 8 CUBE CORNERS,
		int AB = m_permutations[A + 1] + Z;
		int B = m_permutations[X + 1] + Y;
		int BA = m_permutations[B] + Z;
		int BB = m_permutations[B + 1] + Z;
		return
			DoLerp( w, DoLerp( v, DoLerp( u, DoGrad( m_permutations[AA], x, y, z ),				// AND ADD
										  DoGrad( m_permutations[BA], x - 1, y, z ) ),			// BLENDED
							   DoLerp( u, DoGrad( m_permutations[AB], x, y - 1, z ),			// RESULTS
									   DoGrad( m_permutations[BB], x - 1, y - 1, z ) ) ),		// FROM 8
					DoLerp( v, DoLerp( u, DoGrad( m_permutations[AA + 1], x, y, z - 1 ),		// CORNERS
									   DoGrad( m_permutations[BA + 1], x - 1, y, z - 1 ) ),		// OF CUBE
							DoLerp( u, DoGrad( m_permutations[AB + 1], x, y - 1, z - 1 ),
									DoGrad( m_permutations[BB + 1], x - 1, y - 1, z - 1 ) ) ) );
	}

	//*************************************************************************************************

	ProceduralTextures::String CpuStep::ParamNames[eNOISE_PARAM_FUNCTION_COUNT];
	ProceduralTextures::String CpuStep::TypeNames[eNOISE_TYPE_COUNT];

	CpuStep::CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size )
		: ProceduralTextures::CpuStep< Thread >( p_generator, p_size )
	{
		for ( int i = 0; i < eNOISE_PARAM_FUNCTION_COUNT; i++ )
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
	}

	CpuStep::~CpuStep()
	{
	}

	void CpuStep::SetRed( uint8_t val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetRed( val );
		} );
	}

	void CpuStep::SetGreen( uint8_t val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetGreen( val );
		} );
	}

	void CpuStep::SetBlue( uint8_t val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetBlue( val );
		} );
	}

	void CpuStep::SetOctavesCount( int val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetOctavesCount( val );
		} );
	}

	int CpuStep::GetOctavesCount()const
	{
		return DoForOneThread< int >( 0, []( Thread const & l_thread )
		{
			return l_thread.GetOctavesCount();
		} );
	}

	void CpuStep::SetPersistency( float val )
	{
		DoForEachThread( [&val]( Thread & l_thread )
		{
			l_thread.SetPersistency( val );
		} );
	}

	float CpuStep::GetPersistency()const
	{
		return DoForOneThread<float >( 0, []( Thread const & l_thread )
		{
			return l_thread.GetPersistency();
		} );
	}

	void CpuStep::SetType( eNOISE_TYPE p_eType )
	{
		DoForEachThread( [this, &p_eType]( Thread & l_thread )
		{
			l_thread.SetType( p_eType );
		} );
	}

	void CpuStep::SetFunction( eNOISE_PARAM_FUNCTION p_eFunction )
	{
		DoForEachThread( [this, &p_eFunction]( Thread & l_thread )
		{
			l_thread.SetFunction( m_functions[p_eFunction] );
		} );
	}

	void CpuStep::DoInitialiseStep()
	{
	}

	void CpuStep::DoInitialise()
	{
		int l_iOffset = int( m_sizeImage.y() / m_threadsCount );
		int l_iAdd;

		if ( int( l_iOffset * m_threadsCount ) == m_sizeImage.y() )
		{
			l_iAdd = 0;
		}
		else
		{
			l_iAdd = m_sizeImage.y() - int( l_iOffset * m_threadsCount );
		}

		for ( uint32_t i = 0; i < m_sizeImage.y(); i += l_iOffset )
		{
			DoCreateThread( i, i + l_iOffset + l_iAdd, m_sizeImage.y() );
			i += l_iAdd;
			l_iAdd = 0;
		}

		DoForEachThread( [this]( Thread & l_thread )
		{
			l_thread.SetBuffer( m_finalBuffer );
			l_thread.Run();
		} );
	}

	void CpuStep::DoCleanup()
	{
	}

	void CpuStep::DoSwapBuffers()
	{
	}
}
