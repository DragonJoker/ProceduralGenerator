#include "IfxPlugin.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "ImageEffects" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Image Effects" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
