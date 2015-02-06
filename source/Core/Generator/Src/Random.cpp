#include "Random.h"

namespace ProceduralTextures
{
	int Random::MT[624];
	int Random::Index = 0;

	void Random::srand( int seed )
	{
		MT[0] = seed;
		unsigned long long l_tmp;

		for ( int i = 1 ; i < 624 ; i++ )
		{
			// loop over each other element
			l_tmp = 0x6c078965 * ( MT[i - 1] ^ ( MT[i - 1] >> 30 ) ) + i;
			MT[i] = int( l_tmp );
		}
	}

	int Random::rand()
	{
		if ( Index == 0 )
		{
			_generateNumbers();
		}

		int y = MT[Index];
		y = y ^ ( y >> 11 );
		y = y ^ ( y << 7 & 0x9d2c5680 );
		y = y ^ ( y << 15 & 0xefc60000 );
		y = y ^ ( y >> 18 );
		Index = ( Index + 1 ) % 624;
		return y;
	}

	void Random::_generateNumbers()
	{
		for ( int i = 0 ; i < 624 ; i++ )
		{
			int y = ( MT[i] & 0x00000001 ) + ( MT[( i + 1 ) % 624] & 0xFFFFFFFE );
			MT[i] = MT[( i + 397 ) % 624] ^ ( y >> 1 );

			if ( ( y % 2 ) != 0 )
			{
				// y is odd
				MT[i] = MT[i] ^ 0x9908b0df;
			}
		}
	}
}
