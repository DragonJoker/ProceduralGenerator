#include "OclPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( OclPlugin, OCLProcessing )

	OclPlugin::OclPlugin()
		:	Plugin< OCLProcessing, OclPlugin >( wxT( "OCLProcessing" ) )
	{
	}

	OclPlugin::~OclPlugin()
	{
	}

	wxString OclPlugin::GetName()
	{
		wxString l_strBaseName = _( "OpenCL Processing" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
