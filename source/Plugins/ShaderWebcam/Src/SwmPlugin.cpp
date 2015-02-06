#include "SwmPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( SwmPlugin, ShaderWebcam )

	SwmPlugin :: SwmPlugin()
		:	Plugin< ShaderWebcam, SwmPlugin >( wxT( "ShaderWebcam" ) )
	{
	}

	SwmPlugin :: ~SwmPlugin()
	{
	}

	wxString SwmPlugin :: GetName()
	{
		wxString l_strBaseName = _( "Shader Webcam" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
