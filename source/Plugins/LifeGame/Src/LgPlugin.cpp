#include "LgPlugin.h"

using namespace ProceduralTextures;

namespace LifeGame
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "LifeGame" ), _( "Life Game" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}
