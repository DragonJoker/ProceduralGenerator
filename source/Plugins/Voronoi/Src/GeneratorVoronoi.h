#ifndef ___Voronoi___
#define ___Voronoi___

#include "ProceduralGenerator.h"

namespace ProceduralTextures
{
	class Voronoi : public ProceduralGenerator
	{
	public:
		enum eVORONOI_TYPE
		{
			eFirst,
			eSecond,
			eThird,
			eFourth,
			eDifference21,
			eDifference32,
			eCrackle
		};

		enum eDISTANCE_METHOD
		{
			eLength,
			eLength2,
			eManhattan,
			eChebychev,
			eQuadratic,
			eMinkowski4,
			eMinkowski5
		};

	private:
		class Thread : public ProceduralGenerator::Thread
		{
		protected:
			friend class Voronoi;
			int m_iStep;
			eVORONOI_TYPE m_eType;
			eDISTANCE_METHOD m_eDistance;
			PixelBuffer * m_pixels;

		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour);
			virtual ~Thread();

			virtual void Step();

			int Floor( double f) { return (f > 0 ? int( f) : int( f) - 1); }
			double Get( double x, double y, double z, eVORONOI_TYPE p_eDistance, UbPixel & p_rPixel);

			inline void SetType		( eVORONOI_TYPE p_eType)		{ m_eType = p_eType; }
			inline void SetDistance	( eDISTANCE_METHOD p_eDistance)	{ m_eDistance = p_eDistance; }
			inline void SetBuffer	( PixelBuffer * p_pixels)			{ m_pixels = p_pixels; }

		private:
			void _calculate( double x, double y, double z, double da[4], double pa[12]);
			const float * _getPoint( int x, int y, int z);
		};

	private:
		eVORONOI_TYPE m_eType;
		eDISTANCE_METHOD m_eDistance;

	public:
		Voronoi( int p_width, int p_height, eVORONOI_TYPE p_eType, eDISTANCE_METHOD p_eDistance);
		virtual ~Voronoi();

		virtual void InitialiseStep();

		void SetType( eVORONOI_TYPE p_eType);
		void SetDistance( eDISTANCE_METHOD p_eDistance);
	};
}

#endif