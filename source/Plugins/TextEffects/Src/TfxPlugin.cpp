#include "TfxPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( TfxPlugin, TextEffects )

	TfxPlugin::TfxPlugin()
		:	Plugin< TextEffects, TfxPlugin >( wxT( "TextEffects" ) )
	{
	}

	TfxPlugin::~TfxPlugin()
	{
	}

	wxString TfxPlugin::GetName()
	{
		wxString l_strBaseName = _( "Text Effects" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
