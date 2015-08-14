#include "Plugin.h"

namespace ProceduralTextures
{
	PluginBase::PluginBase( String const & p_internal, String const & p_displayable, bool p_customResolution )
		: m_internal( p_internal )
		, m_displayable( p_displayable )
		, m_customResolution( p_customResolution )
	{
		Translator::Initialise( System::GetExecutableDirectory(), _T( "ProceduralGenerator" ), p_internal );
	}

	PluginBase::~PluginBase()
	{
		Translator::Cleanup( m_internal );
	}

	String PluginBase::GetName()
	{
		String l_name = m_displayable;
#if !defined( NDEBUG )
		l_name += _T( " (Debug )" );
#endif
		return l_name;
	}
}
