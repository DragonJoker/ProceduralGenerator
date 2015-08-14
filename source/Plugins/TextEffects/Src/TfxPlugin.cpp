#include "TfxPlugin.h"

using namespace ProceduralTextures;

namespace TextEffects
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "TextEffects" ), _( "Text Effects" ), false )
	{
	}

	Plugin::~Plugin()
	{
	}
}
