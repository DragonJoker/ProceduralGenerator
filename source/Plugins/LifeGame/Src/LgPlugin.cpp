#include "LgPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( LgPlugin, LifeGame )

	LgPlugin::LgPlugin()
		:	Plugin< LifeGame, LgPlugin >( wxT( "LifeGame" ) )
	{
	}

	LgPlugin::~LgPlugin()
	{
	}

	wxString LgPlugin::GetName()
	{
		wxString l_strBaseName = _( "Life Game" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
