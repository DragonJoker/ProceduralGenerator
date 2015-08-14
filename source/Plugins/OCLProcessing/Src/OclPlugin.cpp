#include "OclPlugin.h"

using namespace ProceduralTextures;

namespace OCLProcessing
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "OCLProcessing" ), _( "OpenCL Processing" ), false )
	{
	}

	Plugin::~Plugin()
	{
	}
}
