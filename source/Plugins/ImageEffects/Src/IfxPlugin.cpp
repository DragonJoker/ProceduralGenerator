#include "IfxPlugin.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "ImageEffects" ), _( "Image Effects" ), false )
	{
	}

	Plugin::~Plugin()
	{
	}
}
