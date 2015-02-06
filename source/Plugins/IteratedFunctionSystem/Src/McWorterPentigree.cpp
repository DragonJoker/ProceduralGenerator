#include "McWorterPentigree.h"

namespace ProceduralTextures
{
	McWorterPentigree::McWorterPentigree( PixelBuffer * p_pBackBuffer, int p_iWidth, int p_iHeight )
		:	IfsGenerator( p_pBackBuffer, p_iWidth, p_iHeight, 160, 4, 160, Point2d( 0.5, 0.0 ), 6 )
	{
		DoInitialiseVariations();
	}

	void McWorterPentigree::DoInitialiseVariations()
	{
		// x = a[0]*x + a[1]*y + a[2]
		// y = b[0]*x + b[1]*y + b[2]
		m_arrayVariations[0]	= Point3d( 0.309, -0.205,  0.000 );
		m_arrayVariations[1]	= Point3d( 0.225,  0.309,  0.000 );
		m_arrayProbabilities[0] = Point2i( 0, 100 );
		m_arrayVariations[2]	= Point3d( -0.118, -0.363,  0.309 );
		m_arrayVariations[3]	= Point3d( 0.363, -0.118,  0.225 );
		m_arrayProbabilities[1] = Point2i( 0, 100 );
		m_arrayVariations[4]	= Point3d( 0.309,  0.225,  0.191 );
		m_arrayVariations[5]	= Point3d( -0.225,  0.309,  0.588 );
		m_arrayProbabilities[2] = Point2i( 0, 100 );
		m_arrayVariations[6]	= Point3d( -0.118,  0.363,  0.500 );
		m_arrayVariations[7]	= Point3d( -0.363, -0.118,  0.363 );
		m_arrayProbabilities[3] = Point2i( 0, 100 );
		m_arrayVariations[8]	= Point3d( 0.309,  0.225,  0.382 );
		m_arrayVariations[9]	= Point3d( -0.225,  0.309,  0.000 );
		m_arrayProbabilities[4] = Point2i( 0, 100 );
		m_arrayVariations[10]	= Point3d( 0.309, -0.225,  0.691 );
		m_arrayVariations[11]	= Point3d( 0.225,  0.309, -0.225 );
		m_arrayProbabilities[5] = Point2i( 0, 100 );
	}
}
