#include "PlnPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( PlnPlugin, PerlinNoise )

	PlnPlugin::PlnPlugin()
		:	Plugin< PerlinNoise, PlnPlugin >( wxT( "PerlinNoise" ) )
	{
	}

	PlnPlugin::~PlnPlugin()
	{
	}

	wxString PlnPlugin::GetName()
	{
		wxString l_strBaseName = _( "Perlin Noise" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
