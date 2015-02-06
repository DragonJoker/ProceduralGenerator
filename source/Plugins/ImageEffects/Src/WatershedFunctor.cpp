#include "WatershedFunctor.h"

namespace ProceduralTextures
{
	WatershedFunctor::WatershedFunctor()
		:	EffectFunctor( eEFFECT_TYPE_WATERSHED, _( "Watershed" ), true, true )
		,	m_iNbSteps( 1 )
	{
	}

	WatershedFunctor::~WatershedFunctor()
	{
	}

	void WatershedFunctor::operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		int l_iSteps = ( 256 / m_iNbSteps );

		for ( int i = 0 ; i < m_iImgHeight ; i++ )
		{
			for ( int j = 0 ; j < m_iImgWidth ; j++ )
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
