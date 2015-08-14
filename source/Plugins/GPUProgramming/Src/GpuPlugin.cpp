#include "GpuPlugin.h"

using namespace ProceduralTextures;

namespace GpuProgramming
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "GpuProgramming" ), _( "GPU Programming" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}
