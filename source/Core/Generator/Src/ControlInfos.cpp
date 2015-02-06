#include "ControlInfos.h"

namespace ProceduralTextures
{
	ControlInfos::ControlInfos( eCONTROL_TYPE p_eType, wxWindowID p_iId, wxPoint const & p_ptPosition, wxSize const & p_szSize, SpecificControlParametersBase * p_pSpecific, int p_iStyle, eEVENT_TYPE p_eEventType, wxObjectEventFunction p_pfnFunction, bool p_bVisible )
		:	m_eType( p_eType )
		,	m_iId( p_iId )
		,	m_ptPosition( p_ptPosition )
		,	m_szSize( p_szSize )
		,	m_pSpecific( p_pSpecific )
		,	m_iStyle( p_iStyle )
		,	m_eEventType( p_eEventType )
		,	m_pfnFunction( p_pfnFunction )
		,	m_bVisible( p_bVisible )
	{
	}

	ControlInfos::ControlInfos( ControlInfos const & p_copy )
		:	m_eType( p_copy.m_eType )
		,	m_iId( p_copy.m_iId )
		,	m_ptPosition( p_copy.m_ptPosition )
		,	m_szSize( p_copy.m_szSize )
		,	m_pSpecific( p_copy.m_pSpecific )
		,	m_iStyle( p_copy.m_iStyle )
		,	m_eEventType( p_copy.m_eEventType )
		,	m_pfnFunction( p_copy.m_pfnFunction )
		,	m_bVisible( p_copy.m_bVisible )
	{
	}

	ControlInfos::ControlInfos( ControlInfos && p_copy )
		:	m_eType()
		,	m_iId()
		,	m_ptPosition()
		,	m_szSize()
		,	m_pSpecific()
		,	m_iStyle()
		,	m_eEventType()
		,	m_pfnFunction()
		,	m_bVisible()
	{
		m_eType = p_copy.m_eType;
		m_iId = p_copy.m_iId;
		m_ptPosition = p_copy.m_ptPosition;
		m_szSize = p_copy.m_szSize;
		m_pSpecific = p_copy.m_pSpecific;
		m_iStyle = p_copy.m_iStyle;
		m_eEventType = p_copy.m_eEventType;
		m_pfnFunction = p_copy.m_pfnFunction;
		m_bVisible = p_copy.m_bVisible;
		p_copy.m_eType = eCONTROL_TYPE_NONE;
		p_copy.m_iId = wxID_ANY;
		p_copy.m_ptPosition = wxPoint();
		p_copy.m_szSize = wxSize();
		p_copy.m_pSpecific = NULL;
		p_copy.m_iStyle = 0;
		p_copy.m_eEventType = eEVENT_TYPE_NONE;
		p_copy.m_pfnFunction = NULL;
		p_copy.m_bVisible = false;
	}

	ControlInfos & ControlInfos::operator =( ControlInfos const & p_copy )
	{
		if ( this != &p_copy )
		{
			ControlInfos l_tmp( p_copy );
			std::swap( *this, l_tmp );
		}

		return *this;
	}

	ControlInfos & ControlInfos::operator =( ControlInfos && p_copy )
	{
		if ( this != &p_copy )
		{
			ControlInfos l_tmp( p_copy );
			std::swap( *this, l_tmp );
		}

		return *this;
	}

	ControlInfos::~ControlInfos()
	{
	}
}
