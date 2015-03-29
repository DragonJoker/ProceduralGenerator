#include "RecursiveGaussianBlurFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	RecursiveGaussianBlurFunctor::RecursiveGaussianBlurFunctor()
		:	RecursiveEffectFunctor< GaussianBlurFunctor >( eEFFECT_TYPE_REC_GAUSSIAN_BLUR, _( "Recursive Gaussian blur" ), false, false )
	{
	}

	RecursiveGaussianBlurFunctor::~RecursiveGaussianBlurFunctor()
	{
	}
}
