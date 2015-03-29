#include "SierpinskyCarpet.h"

using namespace ProceduralTextures;

namespace IteratedFunctionSystem
{
	SierpinskiCarpet::SierpinskiCarpet( std::shared_ptr< PixelBuffer > p_pBackBuffer, int p_iWidth, int p_iHeight )
		:	IfsGenerator( p_pBackBuffer, p_iWidth, p_iHeight, 50, 50, 50, Point2d( 0, 0 ), 8 )
	{
		DoInitialiseVariations();
	}

	void SierpinskiCarpet::DoInitialiseVariations()
	{
		// x = a[0]*x + a[1]*y + a[2]
		// y = b[0]*x + b[1]*y + b[2]
		m_arrayVariations[0]	= Point3d( 0.333,  0.000,  0.000 );
		m_arrayVariations[1]	= Point3d( 0.000,  0.333,  0.000 );
		m_arrayProbabilities[0] = Point2i( 0, 100 );
		m_arrayVariations[2]	= Point3d( 0.333,  0.000,  0.000 );
		m_arrayVariations[3]	= Point3d( 0.000,  0.333,  0.333 );
		m_arrayProbabilities[1] = Point2i( 0, 100 );
		m_arrayVariations[4]	= Point3d( 0.333,  0.000,  0.000 );
		m_arrayVariations[5]	= Point3d( 0.000,  0.333,  0.667 );
		m_arrayProbabilities[2] = Point2i( 0, 100 );
		m_arrayVariations[6]	= Point3d( 0.333,  0.000,  0.333 );
		m_arrayVariations[7]	= Point3d( 0.000,  0.333,  0.000 );
		m_arrayProbabilities[3] = Point2i( 0, 100 );
		m_arrayVariations[8]	= Point3d( 0.333,  0.000,  0.333 );
		m_arrayVariations[9]	= Point3d( 0.000,  0.333,  0.667 );
		m_arrayProbabilities[4] = Point2i( 0, 100 );
		m_arrayVariations[10]	= Point3d( 0.333,  0.000,  0.667 );
		m_arrayVariations[11]	= Point3d( 0.000,  0.333,  0.000 );
		m_arrayProbabilities[5] = Point2i( 0, 100 );
		m_arrayVariations[12]	= Point3d( 0.333,  0.000,  0.667 );
		m_arrayVariations[13]	= Point3d( 0.000,  0.333,  0.333 );
		m_arrayProbabilities[6] = Point2i( 0, 100 );
		m_arrayVariations[14]	= Point3d( 0.333,  0.000,  0.667 );
		m_arrayVariations[15]	= Point3d( 0.000,  0.333,  0.667 );
		m_arrayProbabilities[7] = Point2i( 0, 100 );
	}
}
