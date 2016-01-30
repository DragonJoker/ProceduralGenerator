#include "ColorizeFunctor.h"

using namespace ProceduralTextures;

namespace ImageEffects
{
	namespace
	{
		void AddColour( PixelMap & p_colours, uint8_t r, uint8_t g, uint8_t b )
		{
			uint8_t l_gray = uint8_t( double( r ) * 0.21 + double( g ) * 0.71 + double( b ) * 0.07 );
			int l_index = 0;

			while ( l_index < MAX_PIXELS && p_colours[l_gray][l_index].second )
			{
				l_index++;
			}

			if ( l_index < MAX_PIXELS )
			{
				p_colours[l_gray][l_index].first = UbPixel( r, g, b, uint8_t( 0 ) );
				p_colours[l_gray][l_index].second = true;
			}
		}

		PixelMapPair & GetColour( PixelMap & p_colours, uint8_t p_gray )
		{
			bool l_found = false;
			uint8_t l_index = 0;
			uint8_t l_min = p_gray;
			uint8_t l_max = p_gray;

			while ( l_index < MAX_PIXELS && !p_colours[p_gray][l_index].second )
			{
				if ( l_min > 0 )
				{
					while ( --l_min > 0 && !p_colours[l_min][l_index].second )
					{
						// Empty
					}
				}


				if ( l_max < MAX_GRAYS )
				{
					while ( ++l_max < MAX_GRAYS && !p_colours[l_max][l_index].second )
					{
						// Empty
					}
				}

				PixelMapPair & l_minp = p_colours[l_min][l_index];
				PixelMapPair & l_maxp = p_colours[l_max][l_index];
				uint8_t l_idiff = p_gray - l_min;
				uint8_t l_adiff = l_max - p_gray;
				uint8_t l_tdiff = l_idiff + l_adiff;
				AddColour( p_colours,
						   uint8_t( ( double( l_minp.first.r ) * l_idiff + double( l_maxp.first.r ) * l_adiff ) / l_tdiff ),
						   uint8_t( ( double( l_minp.first.g ) * l_idiff + double( l_maxp.first.g ) * l_adiff ) / l_tdiff ),
						   uint8_t( ( double( l_minp.first.b ) * l_idiff + double( l_maxp.first.b ) * l_adiff ) / l_tdiff ) );
			}

			return p_colours[p_gray][l_index];
		}

		UbPixel & FindColour( PixelMap & p_colours,
							  UbPixel const & p_tl, UbPixel const & p_tm, UbPixel const & p_tr,
							  UbPixel const & p_ml, UbPixel const & p_mm, UbPixel const & p_mr,
							  UbPixel const & p_bl, UbPixel const & p_bm, UbPixel const & p_br )
		{
			auto & l_tl = GetColour( p_colours, p_tl.r );
			auto & l_tm = GetColour( p_colours, p_tm.r );
			auto & l_tr = GetColour( p_colours, p_tr.r );
			auto & l_ml = GetColour( p_colours, p_ml.r );
			auto & l_mm = GetColour( p_colours, p_mm.r );
			auto & l_mr = GetColour( p_colours, p_mr.r );
			auto & l_bl = GetColour( p_colours, p_bl.r );
			auto & l_bm = GetColour( p_colours, p_bm.r );
			auto & l_br = GetColour( p_colours, p_br.r );

			return l_mm.first;
		}
	}

	ColorizeFunctor::ColorizeFunctor()
		: EffectFunctor( eEFFECT_TYPE_COLORIZE, _( "Colorize" ), true, true )
	{
	}

	ColorizeFunctor::~ColorizeFunctor()
	{
	}

	void ColorizeFunctor::operator()( PixelBuffer const & p_bufferIn, PixelBuffer & p_bufferOut )
	{
		for ( uint8_t r = 0; r < 255; r += 5 )
		{
			for ( uint8_t g = 0; g < 255; g += 5 )
			{
				for ( uint8_t b = 0; b < 255; b += 5 )
				{
					AddColour( m_colours, r, g, b );
				}
			}
		}

		DynPoint< UbPixel > const * l_pixelsT = &p_bufferIn[0];
		DynPoint< UbPixel > const * l_pixelsM = &p_bufferIn[1];
		DynPoint< UbPixel > const * l_pixelsB = &p_bufferIn[2];
		DynPoint< UbPixel > * l_pixelsOut = &p_bufferOut[1];

		for ( uint32_t i = 1; i < m_size.y() - 1; i++ )
		{
			UbPixel const * l_pixelTL = &( *l_pixelsT )[0];
			UbPixel const * l_pixelML = &( *l_pixelsM )[0];
			UbPixel const * l_pixelBL = &( *l_pixelsB )[0];
			UbPixel const * l_pixelTM = &( *l_pixelsT )[1];
			UbPixel const * l_pixelMM = &( *l_pixelsM )[1];
			UbPixel const * l_pixelBM = &( *l_pixelsB )[1];
			UbPixel const * l_pixelTR = &( *l_pixelsT )[2];
			UbPixel const * l_pixelMR = &( *l_pixelsM )[2];
			UbPixel const * l_pixelBR = &( *l_pixelsB )[2];
			UbPixel * l_pixelOut = &( *l_pixelsOut )[1];

			for ( uint32_t j = 1; j < m_size.x() - 1; j++ )
			{
				l_pixelOut->Set< 3, 1 >( &FindColour( m_colours,
													  *l_pixelTL, *l_pixelML, *l_pixelBL,
													  *l_pixelTM, *l_pixelMM, *l_pixelBM,
													  *l_pixelTR, *l_pixelMR, *l_pixelBR ).r,
										 &l_pixelMM->a );

				++l_pixelTL;
				++l_pixelML;
				++l_pixelBL;
				++l_pixelTM;
				++l_pixelMM;
				++l_pixelBM;
				++l_pixelTR;
				++l_pixelMR;
				++l_pixelBR;
				++l_pixelOut;
			}

			++l_pixelsT;
			++l_pixelsM;
			++l_pixelsB;
			++l_pixelsOut;
		}
	}
}
