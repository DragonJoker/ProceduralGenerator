#include "OclPlugin.h"

using namespace ProceduralTextures;

namespace OCLProcessing
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "OCLProcessing" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "OpenCL Processing" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
