#include "IfsPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( IfsPlugin, IteratedFunctionSystem )

	IfsPlugin::IfsPlugin()
		:	Plugin< IteratedFunctionSystem, IfsPlugin >( wxT( "IteratedFunctionSystem" ) )
	{
	}

	IfsPlugin::~IfsPlugin()
	{
	}

	wxString IfsPlugin::GetName()
	{
		wxString l_strBaseName = _( "Iterated Function System" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
