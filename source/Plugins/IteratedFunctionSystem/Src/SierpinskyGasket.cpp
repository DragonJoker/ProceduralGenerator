#include "SierpinskyGasket.h"

using namespace ProceduralTextures;

namespace IteratedFunctionSystem
{
	SierpinskiGasket::SierpinskiGasket( std::shared_ptr< PixelBuffer > p_pBackBuffer, int p_iWidth, int p_iHeight )
		:	IfsGenerator( p_pBackBuffer, p_iWidth, p_iHeight, 50, 50, 50, Point2d( 0, 0 ), 3 )
	{
		DoInitialiseVariations();
	}

	void SierpinskiGasket::DoInitialiseVariations()
	{
		// x = a[0]*x + a[1]*y + a[2]
		// y = b[0]*x + b[1]*y + b[2]
		m_arrayVariations[0]	= Point3d( 0.500,  0.000,  0.000 );
		m_arrayVariations[1]	= Point3d( 0.000,  0.500,  0.000 );
		m_arrayProbabilities[0] = Point2i( 0, 100 );
		m_arrayVariations[2]	= Point3d( 0.500,  0.000,  0.500 );
		m_arrayVariations[3]	= Point3d( 0.000,  0.500,  0.000 );
		m_arrayProbabilities[1] = Point2i( 0, 100 );
		m_arrayVariations[4]	= Point3d( 0.500,  0.000,  0.250 );
		m_arrayVariations[5]	= Point3d( 0.000,  0.500,  0.433 );
		m_arrayProbabilities[2] = Point2i( 0, 100 );
	}
}
