#include "RecursiveLinearNoiseReductionFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	RecursiveLinearNoiseReductionFunctor::RecursiveLinearNoiseReductionFunctor()
		:	RecursiveEffectFunctor< LinearNoiseReductionFunctor >( eEFFECT_TYPE_REC_LINEAR_NR, _( "Recursive linear noise reduction" ), false, false )
	{
	}

	RecursiveLinearNoiseReductionFunctor::~RecursiveLinearNoiseReductionFunctor()
	{
	}
}
