#include "DlaPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( DlaPlugin, DiffusionLimitedAggregation )

	DlaPlugin::DlaPlugin()
		:	Plugin< DiffusionLimitedAggregation, DlaPlugin >( wxT( "DiffusionLimitedAggregation" ) )
	{
	}

	DlaPlugin::~DlaPlugin()
	{
	}

	wxString DlaPlugin::GetName()
	{
		wxString l_strBaseName = _( "Diffusion Limited Aggregation" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug )" );
#endif
		return l_strBaseName;
	}
}