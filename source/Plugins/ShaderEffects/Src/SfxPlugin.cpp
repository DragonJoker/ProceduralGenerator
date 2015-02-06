#include "SfxPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( SfxPlugin, ShaderEffects )

	SfxPlugin::SfxPlugin()
		:	Plugin< ShaderEffects, SfxPlugin >( wxT( "ShaderEffects" ) )
	{
	}

	SfxPlugin::~SfxPlugin()
	{
	}

	wxString SfxPlugin::GetName()
	{
		wxString l_strBaseName = _( "Shader Effects" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
