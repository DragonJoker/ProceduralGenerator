#include "IfsPlugin.h"

using namespace ProceduralTextures;

namespace IteratedFunctionSystem
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "IteratedFunctionSystem" ), _( "Iterated Function System" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}
