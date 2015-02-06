#include "GpuPlugin.h"

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( GpuPlugin, GpuProgramming )

	GpuPlugin::GpuPlugin()
		:	Plugin< GpuProgramming, GpuPlugin >( wxT( "GpuProgramming" ) )
	{
	}

	GpuPlugin::~GpuPlugin()
	{
	}

	wxString GpuPlugin::GetName()
	{
		wxString l_strBaseName = _( "GPU Programming" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug )" );
#endif
		return l_strBaseName;
	}
}
