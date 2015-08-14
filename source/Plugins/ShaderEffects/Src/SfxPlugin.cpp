#include "SfxPlugin.h"

using namespace ProceduralTextures;

namespace ShaderEffects
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "ShaderEffects" ), _( "Shader Effects" ), false )
	{
	}

	Plugin::~Plugin()
	{
	}
}
