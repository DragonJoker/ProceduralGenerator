#include "V2dPlugin.h"

using namespace ProceduralTextures;

namespace Voronoi2D
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "Voronoi2D" ), _( "Voronoi 2D" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}
