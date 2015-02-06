#include "RecursiveLinearNoiseReductionFunctor.h"

namespace ProceduralTextures
{
	RecursiveLinearNoiseReductionFunctor::RecursiveLinearNoiseReductionFunctor()
		:	RecursiveEffectFunctor< LinearNoiseReductionFunctor >( eEFFECT_TYPE_REC_LINEAR_NR, _( "Recursive linear noise reduction" ), false, false )
	{
	}

	RecursiveLinearNoiseReductionFunctor::~RecursiveLinearNoiseReductionFunctor()
	{
	}
}

//*************************************************************************************************
