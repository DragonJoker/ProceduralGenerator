#include "DlaPlugin.h"

using namespace ProceduralTextures;

namespace DiffusionLimitedAggregation
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "DiffusionLimitedAggregation" ) )
	{
	}

	Plugin::~Plugin()
	{
	}

	String Plugin::GetName()
	{
		String l_strBaseName = _( "Diffusion Limited Aggregation" );
#if !defined( NDEBUG )
		l_strBaseName += _T( " (Debug )" );
#endif
		return l_strBaseName;
	}
}