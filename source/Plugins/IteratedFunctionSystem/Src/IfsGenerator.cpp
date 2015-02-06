#include "IfsGenerator.h"

#include <Random.h>

namespace ProceduralTextures
{
	IfsGenerator::IfsGenerator( PixelBuffer * p_pBackBuffer, int p_iWidth, int p_iHeight, int p_iAngle, int p_iShift, int p_iSize, const Point2d & p_ptPosition, size_t p_uiVariationCount )
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

	void IfsGenerator::Initialise( IfsCellMap & p_arrayGeneratedCellsMap, IfsCellLst & p_arrayGeneratedCellsLst, IfsCellVct & p_arrayGeneratedCellsVct )
	{
		m_arrayInitCellsMap.clear();
		Point2d l_ptPosition;
		size_t l_iCount = 0;
		m_arrayInitCellsVct.reserve( m_uiVariationCount );

		for ( size_t k = 0 ; k < m_uiVariationCount ; k++ )
		{
			l_ptPosition[0] = m_arrayVariations[l_iCount++][2];
			l_ptPosition[1] = m_arrayVariations[l_iCount++][2];
			IfsCell l_cell;

			if ( DoInsertCell( l_ptPosition, p_arrayGeneratedCellsMap, & l_cell ) )
			{
				m_arrayInitCellsMap.push_back( p_arrayGeneratedCellsMap );
				p_arrayGeneratedCellsLst.push_back( l_cell );
				m_arrayInitCellsLst.push_back( p_arrayGeneratedCellsLst );
				p_arrayGeneratedCellsVct.push_back( l_cell );
				m_arrayInitCellsVct.push_back( p_arrayGeneratedCellsVct );
			}
		}
	}

	bool IfsGenerator::Generate( const IfsCell & p_cell, IfsCellMap & p_arrayGeneratedCells )
	{
		bool l_bReturn = false;
		Point2d l_ptPosition;
		size_t l_iCount = 0;
		int iProbability = Random::rand() % 100;

		for ( size_t k = 0 ; k < m_uiVariationCount ; k++ )
		{
			if ( iProbability >= m_arrayProbabilities[k][0] && iProbability < m_arrayProbabilities[k][1] )
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

	bool IfsGenerator::Generate( const IfsCell & p_cell, IfsCellLst & p_arrayGeneratedCells )
	{
		bool l_bReturn = false;
		Point2d l_ptPosition;
		size_t l_iCount = 0;
		int iProbability = Random::rand() % 100;

		for ( size_t k = 0 ; k < m_uiVariationCount ; k++ )
		{
			if ( iProbability >= m_arrayProbabilities[k][0] && iProbability < m_arrayProbabilities[k][1] )
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

	bool IfsGenerator::Generate( IfsCell p_cell, IfsCellVct & p_arrayGeneratedCells )
	{
		bool l_bReturn = false;
		Point2d l_ptPosition;
		size_t l_iCount = 0;
		int iProbability = Random::rand() % 100;

		for ( size_t k = 0 ; k < m_uiVariationCount ; k++ )
		{
			if ( iProbability >= m_arrayProbabilities[k][0] && iProbability < m_arrayProbabilities[k][1] )
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

	bool IfsGenerator::DoInsertCell( const Point2d & p_ptPosition, IfsCellMap & p_arrayGeneratedCells, IfsCell * p_pResult )
	{
		IfsCellMap::iterator l_it1;
		IfsCellIntMap::iterator l_it2;
		IfsCell l_cell;
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
			p_arrayGeneratedCells.insert( std::make_pair( i, std::map < int, IfsCell >() ) );
			l_it1 = p_arrayGeneratedCells.find( i );
		}

		l_it2 = l_it1->second.find( j );

		if ( l_it2 == l_it1->second.end() )
		{
			if ( i >= 0 && j >= 0 && i < m_iWidth && j < m_iHeight )
			{
				l_cell.Set( & ( *m_pBackBuffer )[j][i], p_ptPosition );
				l_cell.m_pixel->Set( m_pxColour.r, m_pxColour.g, m_pxColour.b, m_pxColour.a );
			}
			else
			{
				l_cell.Set( NULL, p_ptPosition );
			}

			l_it1->second.insert( std::make_pair( j, l_cell ) );
			l_bReturn = true;

			if ( p_pResult != NULL )
			{
				( * p_pResult ) = l_cell;
			}
		}

		return l_bReturn;
	}

	bool IfsGenerator::DoInsertCell( const Point2d & p_ptPosition, IfsCellLst & p_arrayGeneratedCells, IfsCell * p_pResult )
	{
		IfsCell l_cell;
		bool l_bReturn = false;
		int i = int( p_ptPosition[0] * m_iWidth );
		int j = int( p_ptPosition[1] * m_iHeight );
		m_ptMin[0] = std::min( i, m_ptMin[0] );
		m_ptMin[1] = std::min( j, m_ptMin[1] );
		m_ptMax[0] = std::max( i, m_ptMax[0] );
		m_ptMax[1] = std::max( j, m_ptMax[1] );

		if ( i >= 0 && j >= 0 && i < m_iWidth && j < m_iHeight )
		{
			l_cell.Set( & ( *m_pBackBuffer )[i][j], p_ptPosition );
			l_cell.m_pixel->Set( m_pxColour.r, m_pxColour.g, m_pxColour.b, m_pxColour.a );
		}
		else
		{
			l_cell.Set( NULL, p_ptPosition );
		}

		p_arrayGeneratedCells.push_back( l_cell );
		l_bReturn = true;

		if ( p_pResult != NULL )
		{
			( * p_pResult ) = l_cell;
		}

		return l_bReturn;
	}

	bool IfsGenerator::DoInsertCell( const Point2d & p_ptPosition, IfsCellVct & p_arrayGeneratedCells, IfsCell * p_pResult )
	{
		IfsCell l_cell;
		bool l_bReturn = false;
		int i = int( p_ptPosition[0] * m_iWidth );
		int j = int( p_ptPosition[1] * m_iHeight );
		m_ptMin[0] = std::min( i, m_ptMin[0] );
		m_ptMin[1] = std::min( j, m_ptMin[1] );
		m_ptMax[0] = std::max( i, m_ptMax[0] );
		m_ptMax[1] = std::max( j, m_ptMax[1] );

		if ( i >= 0 && j >= 0 && i < m_iWidth && j < m_iHeight )
		{
			l_cell.Set( & ( *m_pBackBuffer )[i][j], p_ptPosition );
			l_cell.m_pixel->Set( m_pxColour.r, m_pxColour.g, m_pxColour.b, m_pxColour.a );
		}
		else
		{
			l_cell.Set( NULL, p_ptPosition );
		}

		p_arrayGeneratedCells.push_back( l_cell );
		l_bReturn = true;

		if ( p_pResult != NULL )
		{
			( * p_pResult ) = l_cell;
		}

		return l_bReturn;
	}
}
