#include "IfsPlugin.h"

using namespace ProceduralTextures;

namespace IteratedFunctionSystem
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "IteratedFunctionSystem" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Iterated Function System" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
