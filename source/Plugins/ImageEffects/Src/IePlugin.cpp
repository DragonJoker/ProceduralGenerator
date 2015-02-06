#include "IePlugin.h"

#include <cmath>

namespace ProceduralTextures
{
	IMPLEMENT_PLUGIN( IePlugin, ImageEffects )

	IePlugin::IePlugin()
		:	Plugin< ImageEffects, IePlugin >( wxT( "ImageEffects" ) )
	{
	}

	IePlugin::~IePlugin()
	{
	}

	wxString IePlugin::GetName()
	{
		wxString l_strBaseName = _( "Image Effects" );
#if !defined( NDEBUG )
		l_strBaseName += wxT( " (Debug)" );
#endif
		return l_strBaseName;
	}
}
