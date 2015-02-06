#include "V2dPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( V2dPlugin, Voronoi2D )

	V2dPlugin::V2dPlugin()
		:	Plugin< Voronoi2D, V2dPlugin >( wxT( "Voronoi2D" ) )
	{
	}

	V2dPlugin::~V2dPlugin()
	{
	}

	wxString V2dPlugin::GetName()
	{
		wxString l_strBaseName = _( "Voronoi 2D" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
