#include "RecursiveAnisotropicNoiseReductionFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	RecursiveAnisotropicNoiseReductionFunctor::RecursiveAnisotropicNoiseReductionFunctor()
		:	RecursiveEffectFunctor< AnisotropicNoiseReductionFunctor >( eEFFECT_TYPE_REC_ANISOTROPIC_NR, _( "Recursive anisotropic reduction" ), false, false )
	{
	}

	RecursiveAnisotropicNoiseReductionFunctor::~RecursiveAnisotropicNoiseReductionFunctor()
	{
	}
}
