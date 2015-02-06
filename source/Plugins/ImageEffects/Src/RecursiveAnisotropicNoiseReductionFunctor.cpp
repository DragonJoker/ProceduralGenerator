#include "RecursiveAnisotropicNoiseReductionFunctor.h"

namespace ProceduralTextures
{
	RecursiveAnisotropicNoiseReductionFunctor::RecursiveAnisotropicNoiseReductionFunctor()
		:	RecursiveEffectFunctor< AnisotropicNoiseReductionFunctor >( eEFFECT_TYPE_REC_ANISOTROPIC_NR, _( "Recursive anisotropic reduction" ), false, false )
	{
	}

	RecursiveAnisotropicNoiseReductionFunctor::~RecursiveAnisotropicNoiseReductionFunctor()
	{
	}
}

//*************************************************************************************************
