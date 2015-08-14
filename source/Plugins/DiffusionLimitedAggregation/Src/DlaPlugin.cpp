#include "DlaPlugin.h"

using namespace ProceduralTextures;

namespace DiffusionLimitedAggregation
{
	IMPLEMENT_PLUGIN( Plugin, Generator )

	Plugin::Plugin()
		: ProceduralTextures::Plugin< Generator, Plugin >( _T( "DiffusionLimitedAggregation" ), _( "Diffusion Limited Aggregation" ) )
	{
	}

	Plugin::~Plugin()
	{
	}
}