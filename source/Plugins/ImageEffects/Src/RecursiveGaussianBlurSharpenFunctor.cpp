#include "RecursiveGaussianBlurSharpenFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	RecursiveGaussianBlurSharpenFunctor::RecursiveGaussianBlurSharpenFunctor()
		:	RecursiveEffectFunctor< GaussianBlurSharpenFunctor >( eEFFECT_TYPE_REC_GAUSSIAN_BLUR_SHARPEN, _( "Recursive Gaussian blur + sharpen" ), false, false )
	{
	}

	RecursiveGaussianBlurSharpenFunctor::~RecursiveGaussianBlurSharpenFunctor()
	{
	}
}
