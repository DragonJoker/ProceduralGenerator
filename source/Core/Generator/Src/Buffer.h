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
#ifndef ___GENERATOR_BUFFER_H___
#define ___GENERATOR_BUFFER_H___

#include "Pixel.h"

namespace ProceduralTextures
{
	template< typename T >
	class Buffer
	{
	private:
		T * m_pPixels; //!< The contiguous buffer of values
		bool m_bOwnsPixels; //!< Tells if the buffer owns it's buffer
		Size m_ptSize; //!< The dimensions of the buffer
		std::vector< DynPoint< T > > m_arrayRows;	//!< The array of rows

	public:
		Buffer( const Size & p_ptSize );
		Buffer( const Buffer & p_pixelBuffer );
		virtual ~Buffer();

		Buffer & operator =( const Buffer & p_pixelBuffer );
		Buffer & operator =( const T * p_pBuffer );

		void clear();
		void init();
		void init( const Size & p_ptSize );
		void swap( Buffer & p_pixelBuffer );
		void link( T * p_pBuffer );
		void unlink();
		void mirror();
		void flip();
		template< typename U, size_t Count1, size_t Count2, bool InvertedX, bool InvertedY >
		void set( const U * p_pBuffer1, const U * p_pBuffer2 );

		inline const DynPoint< T > & operator []( size_t p_uiIndex )const
		{
			return m_arrayRows[p_uiIndex];
		}
		inline DynPoint< T > & operator []( size_t p_uiIndex )
		{
			return m_arrayRows[p_uiIndex];
		}
		inline const T * const_ptr()const
		{
			return & m_pPixels[0];
		}
		inline T * ptr()
		{
			return & m_pPixels[0];
		}
		inline Size const & dimensions()const
		{
			return m_ptSize;
		}
		inline size_t size()const
		{
			return m_ptSize[0] * m_ptSize[1] * sizeof( T );
		}
		inline size_t count()const
		{
			return m_ptSize[0] * m_ptSize[1];
		}
	};
}

#include "Buffer.inl"

#endif
