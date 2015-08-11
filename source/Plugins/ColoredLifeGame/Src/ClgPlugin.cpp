#include "ClgPlugin.h"

using namespace ProceduralTextures;

namespace ColoredLifeGame
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "ColoredLifeGame" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Coloured Life Game" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug )" );
#endif
		return l_strBaseName;
	}

	bool Plugin::HasCustomisableResolution()
	{
		return true;
	}
}
