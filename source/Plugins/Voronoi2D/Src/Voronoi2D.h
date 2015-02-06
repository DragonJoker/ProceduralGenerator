/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___V2D_VORONOI_2D_H___
#define ___V2D_VORONOI_2D_H___

#include <ConfigPanel.h>

namespace ProceduralTextures
{
	class Voronoi2D
		:	public ProceduralGenerator
	{
	public:
		typedef Point< uint8_t, 3 >	Colour;
		typedef std::vector< Colour >	ColourArray;
		typedef std::set< int >	IntSet;
		typedef std::map< int, int >	IntIntMap;
		typedef std::map< int, IntSet >	IntSetIntMap;
		typedef std::map< int, IntIntMap >	IntIntMapIntMap;


		typedef enum
		{
			eDISTANCE_FUNC_EUCLIDIAN,
			eDISTANCE_FUNC_SQUARED_EUCLIDIAN,
			eDISTANCE_FUNC_MANHATTAN,
			eDISTANCE_FUNC_MINKOWSKI,
			eDISTANCE_FUNC_CHEBYCHEV,
			eDISTANCE_FUNC_COUNT,
		}	eDISTANCE_FUNC;

	private:
		typedef enum
		{
			eID_DEPTH			= 50,
			eID_NEIGHBOURS		= 51,
			eID_DISTANCE_FUNC	= 52,
			eID_MINKOWSKI_ORDER	= 53,
		}	eID;

		class Thread
			:	public ProceduralGenerator::Thread
		{
		private:
			typedef int ( Voronoi2D::Thread::*DistanceFunction )( const Point2i & p_ptA, const Point2i & p_ptB );

		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour );
			virtual ~Thread();

			virtual void Step();

			void Initialise( PixelBuffer * p_pBackBuffer, IntSetIntMap * p_pSeeds, IntIntMapIntMap * p_pHeights, ColourArray * p_pArrayColours, ColourArray * p_pArrayStepColours, eDISTANCE_FUNC p_eTypeDistance, int p_iMaxDepth, int p_iMaxNeighbours, double p_dMkOrder );
			void ComputeColour( int x, int y, UbPixel & p_px );

			int EuclidianLength( const Point2i & p_ptA, const Point2i & p_ptB );
			int SquaredEuclidianLength( const Point2i & p_ptA, const Point2i & p_ptB );
			int ManhattanLength( const Point2i & p_ptA, const Point2i & p_ptB );
			int MinkowskiLength( const Point2i & p_ptA, const Point2i & p_ptB );
			int ChebychevLength( const Point2i & p_ptA, const Point2i & p_ptB );

		private:
			PixelBuffer * m_pBackBuffer;
			IntSetIntMap * m_pSeeds;
			IntIntMapIntMap * m_pHeights;
			ColourArray * m_pArrayColours;
			ColourArray * m_pArrayStepColours;
			eDISTANCE_FUNC m_eTypeDistance;
			int m_iMaxDepth;
			int m_iMaxNeighbours;
			int m_iX;
			int m_iY;
			double m_dMkOrder;
			DistanceFunction m_pFunctions[eDISTANCE_FUNC_COUNT];
			bool m_bComputed;
		};

	public:
		Voronoi2D( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame );
		virtual ~Voronoi2D();

		void SetMaxDepth( int p_iMaxDepth );
		void SetMaxNeighbours( int p_iMaxNeighbours );
		void SetTypeDistance( eDISTANCE_FUNC p_eType );
		void SetMinkowskiOrder( double p_dMkOrder );

		void Initialise();
		void Generate();

		inline int GetMaxDepth()const
		{
			return m_iMaxDepth;
		}
		inline int GetMaxNeighbours()const
		{
			return m_iMaxNeighbours;
		}
		inline eDISTANCE_FUNC GetTypeDistance()const
		{
			return m_eTypeDistance;
		}
		inline double GetMinkowskiOrder()const
		{
			return m_dMkOrder;
		}

	private:
		virtual void DoGlInitialise() {}
		virtual void DoGlPreRender() {}
		virtual void DoGlRender( bool & WXUNUSED( p_bChanged ) ) {}
		virtual void DoGlPostRender() {}
		virtual void DoGlCleanup() {}
		virtual void DoGeneratePanel();

		void OnMaxDepths( wxScrollEvent & p_event );
		void OnMaxNeighbours( wxScrollEvent & p_event );
		void OnMkOrder( wxScrollEvent & p_event );
		void OnDistanceType( wxCommandEvent & p_event );
		void OnMaxDepthsTrack( wxScrollEvent & p_event );
		void OnMaxNeighboursTrack( wxScrollEvent & p_event );
		void OnMkOrderTrack( wxScrollEvent & p_event );

	private:
		IntSetIntMap m_seeds;
		IntIntMapIntMap m_heights;
		IntSet m_setX;
		IntSet m_setY;
		int m_iMaxDepth;
		int m_iMaxNeighbours;
		ColourArray m_arrayColours;
		ColourArray m_arrayStepColours;
		eDISTANCE_FUNC m_eTypeDistance;
		double m_dMkOrder;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatDepthTitle;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatDepth;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideDepth;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatNeighboursTitle;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatNeighbours;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideNeighbours;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatDistance;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboDistance;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatMinkowski;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideMinkowski;
	};
}

#endif
