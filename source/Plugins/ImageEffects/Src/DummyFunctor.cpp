#include "DummyFunctor.h"

#include <cmath>

namespace ProceduralTextures
{
	DummyFunctor::DummyFunctor()
		:	EffectFunctor( eEFFECT_TYPE_NONE, _( "None" ), true, true )
	{
	}

	DummyFunctor::~DummyFunctor()
	{
	}

	void DummyFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		p_bufferOut = p_bufferIn;
	}
}
