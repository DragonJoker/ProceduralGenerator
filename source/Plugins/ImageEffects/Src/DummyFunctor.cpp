#include "DummyFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	DummyFunctor::DummyFunctor()
		:	EffectFunctor( eEFFECT_TYPE_NONE, _( "None" ), true, true )
	{
	}

	DummyFunctor::~DummyFunctor()
	{
	}

	void DummyFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		p_bufferOut.Set( p_bufferIn );
	}
}
