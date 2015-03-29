#include "SwmPlugin.h"

using namespace ProceduralTextures;

namespace ShaderWebcam
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "ShaderWebcam" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Shader Webcam" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
