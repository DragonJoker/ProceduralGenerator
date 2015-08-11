#include "GpuPlugin.h"

using namespace ProceduralTextures;

namespace GpuProgramming
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		:	ProceduralTextures::Plugin< Generator, Plugin >( _T( "GpuProgramming" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "GPU Programming" );
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
