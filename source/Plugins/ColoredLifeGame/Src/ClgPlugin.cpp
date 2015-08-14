#include "ClgPlugin.h"

using namespace ProceduralTextures;

namespace ColoredLifeGame
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "ColoredLifeGame" ), _( "Coloured Life Game" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}
