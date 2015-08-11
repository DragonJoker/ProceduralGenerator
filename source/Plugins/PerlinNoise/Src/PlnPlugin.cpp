#include "PlnPlugin.h"

using namespace ProceduralTextures;

namespace PerlinNoise
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "PerlinNoise" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Perlin Noise" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug)" );
#endif
		return l_strBaseName;
	}

	bool Plugin::HasCustomisableResolution()
	{
		return true;
	}
}
