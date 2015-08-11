#include "SfxPlugin.h"

using namespace ProceduralTextures;

namespace ShaderEffects
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "ShaderEffects" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Shader Effects" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug)" );
#endif
		return l_strBaseName;
	}

	bool Plugin::HasCustomisableResolution()
	{
		return false;
	}
}
