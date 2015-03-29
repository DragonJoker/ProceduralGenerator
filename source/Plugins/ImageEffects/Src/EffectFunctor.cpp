#include "EffectFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	EffectFunctor::EffectFunctor( eEFFECT_TYPE p_eEffectType, String const & p_strName, bool p_bAllowsSecondEffect, bool p_bCanBeSecondEffect )
		:	m_eEffectType( p_eEffectType )
		,	m_bAllowsSecondEffect( p_bAllowsSecondEffect )
		,	m_bCanBeSecondEffect( p_bCanBeSecondEffect )
		,	m_size()
		,	m_strName( p_strName )
	{
	}

	EffectFunctor::~EffectFunctor()
	{
	}

	void EffectFunctor::SetImage( PixelBuffer const & p_image )
	{
		m_size = p_image.GetDimensions();
	}
}
