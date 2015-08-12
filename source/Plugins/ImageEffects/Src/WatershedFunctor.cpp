#include "WatershedFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	WatershedFunctor::WatershedFunctor()
		: EffectFunctor( eEFFECT_TYPE_WATERSHED, _( "Watershed" ), true, true )
		, m_iNbSteps( 1 )
		, m_tempo( 0 )
	{
	}

	WatershedFunctor::~WatershedFunctor()
	{
	}

	void WatershedFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		DynPoint< UbPixel > const * l_pixelsIn = &p_bufferIn[0];
		DynPoint< UbPixel > * l_pixelsOut = &p_bufferOut[0];
		int l_iSteps = ( 256 / m_iNbSteps );

		for ( uint32_t i = 0; i < m_size.y(); i++ )
		{
			UbPixel const * l_pixelIn = &( *l_pixelsIn )[1];
			UbPixel * l_pixelOut = &( *l_pixelsOut )[1];

			for ( uint32_t j = 0; j < m_size.x(); j++ )
			{
				l_pixelOut->Set( ( ( *l_pixelIn ) / l_iSteps ) * l_iSteps );
				++l_pixelIn;
				++l_pixelOut;
			}

			++l_pixelsIn;
			++l_pixelsOut;
		}

		m_tempo++;

		if ( m_tempo >= 10 )
		{
			m_tempo = 0;
			m_iNbSteps++;

			if ( m_iNbSteps >= 50 )
			{
				m_iNbSteps = 1;
			}
		}
	}
}

//*************************************************************************************************
