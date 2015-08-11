#include "V2dPlugin.h"

using namespace ProceduralTextures;

namespace Voronoi2D
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "Voronoi2D" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Voronoi 2D" );
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
