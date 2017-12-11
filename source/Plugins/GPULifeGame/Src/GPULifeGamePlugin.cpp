#include "GpuLifeGamePlugin.h"

using namespace ProceduralTextures;

namespace GpuLifeGame
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "GpuLifeGame" ), _( "GPU Life Game" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}
