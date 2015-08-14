/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___IFS_GENERATOR_H___
#define ___IFS_GENERATOR_H___

#include "IfsCell.h"

#include <Buffer.h>
#include <Point.h>

namespace IteratedFunctionSystem
{
	typedef enum
	{
		eIFS_GENERATOR_SIERPINSKI_CARPET,
		eIFS_GENERATOR_SIERPINSKI_GASKET,
		eIFS_GENERATOR_FRACTAL_LEAF,
		eIFS_GENERATOR_HEIGHWAY_DRAGON,
		eIFS_GENERATOR_MCWORTHER_PENTIGREE,
		eIFS_GENERATOR_PENTADENTRITE,
		eIFS_GENERATOR_KOCH_CURVE,
		eIFS_GENERATOR_LEVY_DRAGON,
		eIFS_GENERATOR_COUNT,
	}	eIFS_GENERATOR;

	class IfsGenerator
	{
	public:
		IfsGenerator( std::shared_ptr< ProceduralTextures::PixelBuffer > p_pBackBuffer, int p_iWidth, int p_iHeight, int p_iAngle, int p_iShift, int p_iSize, ProceduralTextures::Point2d const & p_ptPosition, size_t p_uiVariationCount );
		virtual ~IfsGenerator();

		void Initialise( std::shared_ptr< CellMap > p_arrayGeneratedCellsMap );
		bool Generate( Cell const & p_cell, CellMap & p_arrayGeneratedCells );
		bool Generate( Cell const & p_cell, CellLst & p_arrayGeneratedCells );
		bool Generate( Cell p_cell, CellVct & p_arrayGeneratedCells );

		bool NextGeneration();

		inline void SetAngle( int val )
		{
			m_iAngle = val;
			DoInitialiseVariations();
		}
		inline void SetShift( int val )
		{
			m_iShift = val;
			DoInitialiseVariations();
		}
		inline void SetSize( int val )
		{
			m_iSize = val;
			DoInitialiseVariations();
		}

		inline int GetShift()const
		{
			return m_iShift;
		}
		inline int GetAngle()const
		{
			return m_iAngle;
		}
		inline int GetSize()const
		{
			return m_iSize;
		}
		inline size_t GetVariationCount()const
		{
			return m_uiVariationCount;
		}
		inline size_t GetInitCellsMapSize()const
		{
			return m_arrayInitCellsMap.size();
		}
		inline std::shared_ptr< CellMap > GetInitCellsMap( size_t p_uiIndex )
		{
			return m_arrayInitCellsMap[p_uiIndex];
		}

		void Resample();

	protected:
		virtual void DoInitialiseVariations() = 0;

	private:
		bool DoInsertCell( ProceduralTextures::Point2d const & p_ptPosition, CellMap & p_arrayGeneratedCells, Cell * p_pResult = NULL );

	protected:
		int m_iWidth;
		int m_iHeight;
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_pBackBuffer;
		size_t m_uiVariationCount;
		int m_iAngle;
		int m_iShift;
		int m_iSize;
		std::vector< ProceduralTextures::Point3d > m_arrayVariations;
		std::vector< ProceduralTextures::Point2i > m_arrayProbabilities;
		ProceduralTextures::Point2d m_ptPositionInSection;
		ProceduralTextures::Point2i m_ptMin;
		ProceduralTextures::Point2i m_ptMax;
		std::vector< std::shared_ptr< CellMap > > m_arrayInitCellsMap;
		ProceduralTextures::UbPixel m_pxColour;
	};
}

#endif
