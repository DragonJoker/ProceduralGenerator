#include "TfxPlugin.h"

using namespace ProceduralTextures;

namespace TextEffects
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "TextEffects" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Text Effects" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
