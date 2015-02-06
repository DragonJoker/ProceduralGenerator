#include "BlgPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( BlgPlugin, BuggedLifeGame )

	BlgPlugin::BlgPlugin()
		:	Plugin< BuggedLifeGame, BlgPlugin >( wxT( "BuggedLifeGame" ) )
	{
	}

	BlgPlugin::~BlgPlugin()
	{
	}

	wxString BlgPlugin::GetName()
	{
		wxString l_strBaseName = _( "Bugged Life Game" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug )" );
#endif
		return l_strBaseName;
	}
}
