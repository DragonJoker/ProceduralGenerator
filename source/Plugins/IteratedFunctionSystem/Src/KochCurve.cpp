#include "KochCurve.h"

namespace ProceduralTextures
{
	KochCurve::KochCurve( PixelBuffer * p_pBackBuffer, int p_iWidth, int p_iHeight )
		:	IfsGenerator( p_pBackBuffer, p_iWidth, p_iHeight, 160, 4, 160, Point2d( 0.5, 0.0 ), 4 )
	{
		DoInitialiseVariations();
	}

	void KochCurve::DoInitialiseVariations()
	{
		// x = a[0]*x + a[1]*y + a[2]
		// y = b[0]*x + b[1]*y + b[2]
		m_arrayVariations[0]	= Point3d( 0.333,  0.000, 0.000 );
		m_arrayVariations[1]	= Point3d( 0.000,  0.333,  0.000 );
		m_arrayProbabilities[0] = Point2i( 0, 1 );
		m_arrayVariations[2]	= Point3d( 0.167, -0.289, 0.333 );
		m_arrayVariations[3]	= Point3d( 0.289,  0.167,  0.000 );
		m_arrayProbabilities[1] = Point2i( 15, 100 );
		m_arrayVariations[4]	= Point3d( 0.167,  0.289, 0.500 );
		m_arrayVariations[5]	= Point3d( -0.289,  0.167,  0.289 );
		m_arrayProbabilities[2] = Point2i( 1, 8 );
		m_arrayVariations[6]	= Point3d( 0.333,  0.000, 0.667 );
		m_arrayVariations[7]	= Point3d( 0.000,  0.333,  0.000 );
		m_arrayProbabilities[3] = Point2i( 8, 15 );
	}
}
