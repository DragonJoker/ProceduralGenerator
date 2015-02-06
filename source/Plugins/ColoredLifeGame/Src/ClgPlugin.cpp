#include "ClgPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( ClgPlugin, ColoredLifeGame )

	ClgPlugin::ClgPlugin()
		:	Plugin< ColoredLifeGame, ClgPlugin >( wxT( "ColoredLifeGame" ) )
	{
	}

	ClgPlugin::~ClgPlugin()
	{
	}

	wxString ClgPlugin::GetName()
	{
		wxString l_strBaseName = _( "Coloured Life Game" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug )" );
#endif
		return l_strBaseName;
	}
}
