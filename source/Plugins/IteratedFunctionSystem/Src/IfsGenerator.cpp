#include "IfsGenerator.h"

#include <random>

using namespace ProceduralTextures;

namespace IteratedFunctionSystem
{
	IfsGenerator::IfsGenerator( std::shared_ptr< PixelBuffer > p_pBackBuffer, int p_iWidth, int p_iHeight, int p_iAngle, int p_iShift, int p_iSize, Point2d const & p_ptPosition, size_t p_uiVariationCount )
		:	m_pBackBuffer( p_pBackBuffer )
		,	m_iHeight( p_iHeight )
		,	m_iWidth( p_iWidth )
		,	m_iAngle( p_iAngle )
		,	m_iShift( p_iShift )
		,	m_iSize( p_iSize )
		,	m_ptPositionInSection( p_ptPosition )
		,	m_uiVariationCount( p_uiVariationCount )
		,	m_pxColour( 16, 16, 16, 255 )
	{
		m_arrayVariations.resize( m_uiVariationCount * 2 );
		m_arrayProbabilities.resize( m_uiVariationCount );
	}

	IfsGenerator::~IfsGenerator()
	{
	}

	void IfsGenerator::Initialise( std::shared_ptr< CellMap > p_arrayGeneratedCellsMap )
	{
		m_arrayInitCellsMap.clear();
		Point2d l_ptPosition;
		size_t l_iCount = 0;

		for ( size_t k = 0; k < m_uiVariationCount; k++ )
		{
			l_ptPosition[0] = m_arrayVariations[l_iCount++][2];
			l_ptPosition[1] = m_arrayVariations[l_iCount++][2];
			Cell l_cell;

			if ( DoInsertCell( l_ptPosition, *p_arrayGeneratedCellsMap, &l_cell ) )
			{
				m_arrayInitCellsMap.push_back( p_arrayGeneratedCellsMap );
			}
		}
	}

	bool IfsGenerator::Generate( Cell const & p_cell, CellMap & p_arrayGeneratedCells )
	{
		bool l_bReturn = false;
		Point2d l_ptPosition;
		size_t l_iCount = 0;
		std::random_device l_randDevice;
		std::default_random_engine l_randEngine( l_randDevice() );
		std::uniform_int_distribution< int > l_randDistribution( 0, 99 );
		int l_probability = l_randDistribution( l_randEngine );

		for ( size_t k = 0; k < m_uiVariationCount; k++ )
		{
			if ( l_probability >= m_arrayProbabilities[k][0] && l_probability < m_arrayProbabilities[k][1] )
			{
				l_ptPosition = p_cell.m_ptPosition;
				l_ptPosition[0] = l_ptPosition[0] * m_arrayVariations[l_iCount][0] + l_ptPosition[1] * m_arrayVariations[l_iCount][1] + m_arrayVariations[l_iCount][2];
				l_iCount++;
				l_ptPosition[1] = l_ptPosition[0] * m_arrayVariations[l_iCount][0] + l_ptPosition[1] * m_arrayVariations[l_iCount][1] + m_arrayVariations[l_iCount][2];
				l_iCount++;
				l_bReturn |= DoInsertCell( l_ptPosition, p_arrayGeneratedCells );
			}
			else
			{
				l_iCount++;
				l_iCount++;
			}
		}

		return l_bReturn;
	}

	void IfsGenerator::Resample()
	{
	}

	bool IfsGenerator::NextGeneration()
	{
		if ( m_pxColour.r < 239 )
		{
			m_pxColour += UbPixel( 16, 16, 16, 255 );
		}

		return true;
	}

	bool IfsGenerator::DoInsertCell( Point2d const & p_ptPosition, CellMap & p_arrayGeneratedCells, Cell * p_pResult )
	{
		CellMap::iterator l_it1;
		CellIntMap::iterator l_it2;
		Cell l_cell;
		bool l_bReturn = false;
		int i = int( p_ptPosition[0] * m_iWidth );
		int j = int( p_ptPosition[1] * m_iHeight );
		m_ptMin[0] = std::min( i, m_ptMin[0] );
		m_ptMin[1] = std::min( j, m_ptMin[1] );
		m_ptMax[0] = std::max( i, m_ptMax[0] );
		m_ptMax[1] = std::max( j, m_ptMax[1] );
		l_it1 = p_arrayGeneratedCells.find( i );

		if ( l_it1 == p_arrayGeneratedCells.end() )
		{
			p_arrayGeneratedCells.insert( std::make_pair( i, std::map < int, Cell >() ) );
			l_it1 = p_arrayGeneratedCells.find( i );
		}

		l_it2 = l_it1->second.find( j );

		if ( l_it2 == l_it1->second.end() )
		{
			if ( i >= 0 && j >= 0 && i < m_iWidth && j < m_iHeight )
			{
				l_cell.Set( &( *m_pBackBuffer.lock() )[j][i], p_ptPosition );
				l_cell.m_pixel->Set( m_pxColour.r, m_pxColour.g, m_pxColour.b, m_pxColour.a );
			}
			else
			{
				l_cell.Set( NULL, p_ptPosition );
			}

			l_it1->second.insert( std::make_pair( j, l_cell ) );
			l_bReturn = true;

			if ( p_pResult )
			{
				( *p_pResult ) = l_cell;
			}
		}

		return l_bReturn;
	}
}
