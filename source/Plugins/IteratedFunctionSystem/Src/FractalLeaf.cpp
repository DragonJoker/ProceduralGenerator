#include "FractalLeaf.h"

using namespace ProceduralTextures;

namespace IteratedFunctionSystem
{
	FractalLeaf::FractalLeaf( std::shared_ptr< PixelBuffer > p_pBackBuffer, int p_iWidth, int p_iHeight )
		:	IfsGenerator( p_pBackBuffer, p_iWidth, p_iHeight, 160, 4, 160, Point2d( 0.5, 0.0 ), 4 )
	{
		DoInitialiseVariations();
	}

	void FractalLeaf::DoInitialiseVariations()
	{
		// x = a[0]*x + a[1]*y + a[2]
		// y = b[0]*x + b[1]*y + b[2]
		m_arrayVariations[0]	= Point3d( 0.000,  0.000, 0.000 );
		m_arrayVariations[1]	= Point3d( 0.000,  0.160,  0.000 );
		m_arrayProbabilities[0] = Point2i( 0, 1 );
		m_arrayVariations[2]	= Point3d( 0.850,  0.040, 0.000 );
		m_arrayVariations[3]	= Point3d( -0.040,  0.850,  1.600 );
		m_arrayProbabilities[1] = Point2i( 15, 100 );
		m_arrayVariations[4]	= Point3d( 0.200, -0.260, 0.000 );
		m_arrayVariations[5]	= Point3d( 0.230,  0.220,  1.600 );
		m_arrayProbabilities[2] = Point2i( 1, 8 );
		m_arrayVariations[6]	= Point3d( -0.150,  0.280, 0.000 );
		m_arrayVariations[7]	= Point3d( 0.260,  0.240,  0.440 );
		m_arrayProbabilities[3] = Point2i( 8, 15 );
	}
}
