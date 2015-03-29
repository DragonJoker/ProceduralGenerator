#include "WatershedFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	WatershedFunctor::WatershedFunctor()
		:	EffectFunctor( eEFFECT_TYPE_WATERSHED, _( "Watershed" ), true, true )
		,	m_iNbSteps( 1 )
	{
	}

	WatershedFunctor::~WatershedFunctor()
	{
	}

	void WatershedFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		int l_iSteps = ( 256 / m_iNbSteps );

		for ( uint32_t i = 0; i < m_size.y(); i++ )
		{
			for ( uint32_t j = 0; j < m_size.x(); j++ )
			{
				p_bufferOut[i][j].Set( ( p_bufferIn[i][j] / l_iSteps ) * l_iSteps );
			}
		}

		m_iNbSteps++;

		if ( m_iNbSteps >= 50 )
		{
			m_iNbSteps = 1;
		}
	}
}

//*************************************************************************************************
