#include "EffectFunctor.h"

namespace ProceduralTextures
{
	EffectFunctor::EffectFunctor( eEFFECT_TYPE p_eEffectType, const wxString & p_strName, bool p_bAllowsSecondEffect, bool p_bCanBeSecondEffect )
		:	m_eEffectType( p_eEffectType )
		,	m_bAllowsSecondEffect( p_bAllowsSecondEffect )
		,	m_bCanBeSecondEffect( p_bCanBeSecondEffect )
		,	m_iImgWidth( 0 )
		,	m_iImgHeight( 0 )
		,	m_strName( p_strName )
	{
	}

	EffectFunctor::~EffectFunctor()
	{
	}

	void EffectFunctor::SetImage( const wxImage & p_image )
	{
		m_iImgWidth = p_image.GetWidth();
		m_iImgHeight = p_image.GetHeight();
	}
}
