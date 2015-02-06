#include "RecursiveGaussianBlurFunctor.h"

namespace ProceduralTextures
{
	RecursiveGaussianBlurFunctor::RecursiveGaussianBlurFunctor()
		:	RecursiveEffectFunctor< GaussianBlurFunctor >( eEFFECT_TYPE_REC_GAUSSIAN_BLUR, _( "Recursive Gaussian blur" ), false, false )
	{
	}

	RecursiveGaussianBlurFunctor::~RecursiveGaussianBlurFunctor()
	{
	}
}
