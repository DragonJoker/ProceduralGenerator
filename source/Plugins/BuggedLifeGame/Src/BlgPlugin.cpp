#include "BlgPlugin.h"

using namespace ProceduralTextures;

namespace BuggedLifeGame
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "BuggedLifeGame" ), _( "Bugged Life Game" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}
