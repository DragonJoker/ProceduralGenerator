#include "RecursiveSharpenFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	RecursiveSharpenFunctor::RecursiveSharpenFunctor()
		:	RecursiveEffectFunctor< SharpenFunctor >( eEFFECT_TYPE_REC_SHARPEN, _( "Recursive sharpen" ), false, false )
	{
	}

	RecursiveSharpenFunctor::~RecursiveSharpenFunctor()
	{
	}
}
