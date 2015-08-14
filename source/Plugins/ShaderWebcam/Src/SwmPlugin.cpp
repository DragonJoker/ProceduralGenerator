#include "SwmPlugin.h"

using namespace ProceduralTextures;

namespace ShaderWebcam
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "ShaderWebcam" ), _( "Shader Webcam" ), false )
	{
	}

	Plugin::~Plugin()
	{
	}
}
