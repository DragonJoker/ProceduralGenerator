/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___CLG_CELL_H___
#define ___CLG_CELL_H___

#include <GeneratorPrerequisites.h>
#include <Pixel.h>

namespace ProceduralTextures
{
	struct ClgCell
	{
		UbPixel * m_nextPixel;
		UbPixel * m_deadPixel;
		UbPixel * m_alivePixel;
		UiPixel * m_stepPixel;
		bool m_alive;

		ClgCell();
		ClgCell( const ClgCell & cell );
		~ClgCell();

		ClgCell  & operator =( const ClgCell & cell );

		void Set( UbPixel * p_pixel, UbPixel * p_deadPixel, UbPixel * p_alivePixel, UiPixel * p_stepPixel, bool p_alive );
		void Die();
		void Live();

		void SetAlive( bool p_alive );
	};

	template <> class Policy< ProceduralTextures::ClgCell >
	{
	private:
		typedef ProceduralTextures::ClgCell value_type;

	public:
		static value_type zero()
		{
			return value_type();
		}
		static value_type unit()
		{
			return value_type();
		}
		static void init( value_type & p_a )
		{
			p_a = zero();
		}
		static bool is_null( const value_type & p_a )
		{
			return equals( p_a, zero() );
		}
		static value_type negate( const value_type & p_a )
		{
			return p_a;
		}
		static value_type ass_negate( value_type & p_a )
		{
			return assign( p_a, negate( p_a ) );
		}
		static value_type stick( value_type & p_a )
		{
			return p_a;
		}
		template< typename Ty > static value_type convert( const Ty & p_value )
		{
			return static_cast< value_type >( p_value );
		}
		template< typename Ty > static bool equals( const value_type & WXUNUSED( p_a ), const Ty & WXUNUSED( p_b ) )
		{
			return false;
		}
		template< typename Ty > static value_type add( const value_type & p_a, const Ty & WXUNUSED( p_b ) )
		{
			return p_a;
		}
		template< typename Ty > static value_type substract( const value_type & p_a, const Ty & WXUNUSED( p_b ) )
		{
			return p_a;
		}
		template< typename Ty > static value_type multiply( const value_type & p_a, const Ty & WXUNUSED( p_b ) )
		{
			return p_a;
		}
		template< typename Ty > static value_type divide( const value_type & p_a, const Ty & WXUNUSED( p_b ) )
		{
			return p_a;
		}
		template< typename Ty > static value_type ass_add( value_type & p_a, const Ty & WXUNUSED( p_b ) )
		{
			return p_a;
		}
		template< typename Ty > static value_type ass_substract( value_type & p_a, const Ty & WXUNUSED( p_b ) )
		{
			return p_a;
		}
		template< typename Ty > static value_type ass_multiply( value_type & p_a, const Ty & WXUNUSED( p_b ) )
		{
			return p_a;
		}
		template< typename Ty > static value_type ass_divide( value_type & p_a, const Ty & WXUNUSED( p_b ) )
		{
			return p_a;
		}
		template< typename Ty > static value_type assign( value_type & p_a, const Ty & p_b )
		{
			return p_a = convert< Ty >( p_b );
		}
	};
}

#endif
