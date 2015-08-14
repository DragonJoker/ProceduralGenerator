#include "PlnPlugin.h"

using namespace ProceduralTextures;

namespace PerlinNoise
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "PerlinNoise" ), _( "Perlin Noise" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}
