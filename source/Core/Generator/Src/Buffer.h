/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option ) any later
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
#include "Size.h"

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@version
		0.6.1.0
	@date
		29/08/2011
	@brief
		Pixel buffer definition
	*/
	template< typename T >
	class Buffer
	{
	public:
		/** Default constructor
		 */
		Buffer();

		/** Constructor from a size
		@param[in] p_ptSize
			The buffer dimensions
		 */
		Buffer( Size const & p_ptSize );

		/** Copy constructor
		@param[in] p_pixelBuffer
			The buffer
		 */
		Buffer( Buffer const & p_pixelBuffer );

		/** Copy constructor
		@param[in] p_pixelBuffer
			The buffer
		@return
			A reference to this buffer
		 */
		Buffer & operator =( Buffer const & p_pixelBuffer );

		/** Destructor
		 */
		virtual ~Buffer();

		/** Releases the values buffer
		@remarks
			If the buffer doesn't own its data it is just set to NULL
		 */
		void Clear();

		/** Initialises the values buffer and the buffer columns
		 */
		void Initialise();

		/** Initialises the values buffer and the buffer columns
		@param[in] p_ptSize
			The new buffer dimensions
		 */
		void Initialise( Size const & p_ptSize );

		/** Copies the values buffer contained in the given pixel buffer
		@param[in] p_pixelBuffer
			The other buffer
		 */
		void Set( Buffer const & p_pixelBuffer );

		/** Copies the values buffer contained in the given pixel buffer
		@param[in] p_pBuffer
			The other buffer
		 */
		void Set( T const * p_pBuffer );

		/** Swaps this buffer and the one given
		@param[in] p_pixelBuffer
			The other buffer
		 */
		void swap( Buffer & p_pixelBuffer );

		/** Swaps the column order
		 */
		void Flip();

		/** Sets the buffer values from the given buffers
		@see
			Pixel::Set
		@param[in] p_pBuffer1
			The first buffer
		@param[in] p_pBuffer2
			The second buffer
		 */
		template< typename U, size_t Count1, size_t Count2, bool InvertedX, bool InvertedY >
		void Set( U const * p_pBuffer1, U const * p_pBuffer2 );

		/** Retrieves the column At given index
		@param[in] p_uiIndex
			The column index
		@return
			The column
		 */
		inline const DynPoint< T > & operator []( size_t p_uiIndex )const
		{
			return m_arrayRows[p_uiIndex];
		}

		/** Retrieves the column At given index
		@param[in] p_uiIndex
			The column index
		@return
			The column
		 */
		inline DynPoint< T > & operator []( size_t p_uiIndex )
		{
			return m_arrayRows[p_uiIndex];
		}

		/** Retrieves a constant pointer to the buffer pixels
		@return
			The value
		 */
		inline const T * ConstPtr()const
		{
			return m_pixels.data();
		}

		/** Retrieves a pointer to the buffer pixels
		@return
			The value
		 */
		inline T * Ptr()
		{
			return m_pixels.data();
		}

		/** Retrieves the buffer dimensions
		@return
			The value
		 */
		inline Size const & GetDimensions()const
		{
			return m_size;
		}

		/** Retrieves the buffer pixels count
		@return
			The value
		 */
		inline size_t GetElementsCount()const
		{
			return m_size.x() * m_size.y();
		}

		/** Retrieves the buffer memory size
		@return
			The value
		 */
		inline size_t GetSize()const
		{
			return GetElementsCount() * sizeof( T );
		}

		/** Retrieves an iterator to the beginning of the content
		@return
			The value
		 */
		inline typename std::vector< DynPoint< T > >::iterator begin()
		{
			return m_arrayRows.begin();
		}

		/** Retrieves an iterator to the beginning of the content
		@return
			The value
		 */
		inline typename std::vector< DynPoint< T > >::const_iterator begin()const
		{
			return m_arrayRows.begin();
		}

		/** Retrieves an iterator to the end of the content
		@return
			The value
		 */
		inline typename std::vector< DynPoint< T > >::iterator end()
		{
			return m_arrayRows.end();
		}

		/** Retrieves an iterator to the end of the content
		@return
			The value
		 */
		inline typename std::vector< DynPoint< T > >::const_iterator end()const
		{
			return m_arrayRows.end();
		}

	private:
		//! The continuous buffer of values
		std::vector< T > m_pixels;
		//! The dimensions of the buffer
		Size m_size;
		//! The array of rows
		std::vector< DynPoint< T > > m_arrayRows;
	};
}

#include "Buffer.inl"

#endif
