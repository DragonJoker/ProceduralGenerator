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
#ifndef ___GENERATOR_VERTEX_H___
#define ___GENERATOR_VERTEX_H___

#include "Point.h"

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
		Vertex definition
	*/
	template< typename PosType = float >
	class GeneratorAPI TVertex
	{
	public:
		typedef Point< PosType, 2 > TPoint;

	public:
		/** Default constructor
		*/
		TVertex()
			: m_bOwnBuffer( false )
			, m_pBuffer( NULL )
		{
			Count++;
			Unlink();
		}

		/** Specified constructor
		@param[in] p_position
			The position
		@param[in] p_texture
			The texture coordinates
		*/
		TVertex( TPoint const & p_position, Point2f const & p_texture )
			: TVertex()
		{
			Count++;
			Unlink();
			SetPosition( p_position );
			SetTexture( p_texture );
		}

		/** Copy constructor
		*/
		TVertex( TVertex const & p_vertex )
			: m_bOwnBuffer( false )
			, m_pBuffer( NULL )
			, m_position( p_vertex.m_position )
			, m_texture( p_vertex.m_texture )
		{
			Count++;
			Unlink();
		}

		/** Destructor
		*/
		~TVertex()
		{
			if ( m_bOwnBuffer && m_pBuffer )
			{
				delete [] m_pBuffer;
				m_pBuffer = NULL;
			}

			Count--;
		}

		/** Copy assignment operator
		*/
		inline TVertex & operator=( TVertex< PosType > const & p_vertex )
		{
			m_position = p_vertex.m_position;
			m_texture = p_vertex.m_texture;

			if ( m_bOwnBuffer && m_pBuffer )
			{
				delete [] m_pBuffer;
				m_pBuffer = NULL;
			}

			m_bOwnBuffer = false;
			Unlink();
			return * this;
		}

		/** Links the vertex coords to the ones in parameter.
		@remarks
			The vertex no longer owns it's coords
		@param[in] p_pBuffer
			The coordinates buffer
		*/
		inline void Link( uint8_t * p_pBuffer )
		{
			size_t l_count = GetStride();
			std::vector< uint8_t > l_pBufferSave( l_count );

			if ( m_pBuffer )
			{
				memcpy( &l_pBufferSave[0], m_pBuffer, GetStride() );
			}
			else
			{
				memcpy( &l_pBufferSave[0], m_position.ConstPtr(), 2 * sizeof( PosType ) );
				memcpy( &l_pBufferSave[2 * sizeof( PosType )], m_texture.ConstPtr(), 2 * sizeof( float ) );
			}

			m_position.Unlink();
			m_texture.Unlink();

			if ( m_bOwnBuffer && m_pBuffer )
			{
				delete [] m_pBuffer;
				m_pBuffer = NULL;
			}

			m_pBuffer = p_pBuffer;
			m_bOwnBuffer = false;
			DoLink();
			memcpy( m_pBuffer, &l_pBufferSave[0], GetStride() );
		}

		/** Unlinks the vertex coords.
		@remarks
			The vertex now owns it's coords
		*/
		inline void Unlink()
		{
			size_t l_count = GetStride();
			std::vector< uint8_t > l_pBufferSave( l_count );

			if ( m_pBuffer )
			{
				memcpy( &l_pBufferSave[0], m_pBuffer, GetStride() );
			}
			else
			{
				memcpy( &l_pBufferSave[0], m_position.ConstPtr(), 2 * sizeof( PosType ) );
				memcpy( &l_pBufferSave[2 * sizeof( PosType )], m_texture.ConstPtr(), 2 * sizeof( float ) );
			}

			m_position.Unlink();
			m_texture.Unlink();

			if ( !m_bOwnBuffer )
			{
				m_pBuffer = new uint8_t[l_count];
			}

			m_bOwnBuffer = true;
			DoLink();
			memcpy( m_pBuffer, &l_pBufferSave[0], GetStride() );
		}

		/** Sets the vertex position
		@param[in] val
			The new value
		*/
		inline void SetPosition( TPoint const & val )
		{
			m_position[0] = val[0];
			m_position[1] = val[1];
		}

		/** Sets the vertex position
		@param[in] x, y
			The new value
		*/
		inline void SetPosition( PosType x, PosType y )
		{
			m_position[0] = x;
			m_position[1] = y;
		}

		/** Sets the vertex position
		@param[in] p_pCoords
			The new value
		*/
		inline void SetPosition( const PosType * p_pCoords )
		{
			m_position[0] = p_pCoords[0];
			m_position[1] = p_pCoords[1];
		}

		/** Sets the vertex texture coordinates
		@param[in] val
			The new value
		*/
		inline void SetTexture( Point2f const & val )
		{
			m_texture[0] = val[0];
			m_texture[1] = val[1];
		}

		/** Sets the vertex texture coordinates
		@param[in] x, y
			The new value
		*/
		inline void SetTexture( float x, float y )
		{
			m_texture[0] = x;
			m_texture[1] = y;
		}

		/** Sets the vertex texture coordinates
		@param[in] p_pCoords
			The new value
		*/
		inline void SetTexture( const float * p_pCoords )
		{
			m_texture[0] = p_pCoords[0];
			m_texture[1] = p_pCoords[1];
		}

		/** Retrieves the vertex position
		@return
			The value
		*/
		inline TPoint & GetPosition()
		{
			return m_position;
		}

		/** Retrieves the vertex position
		@return
			The value
		*/
		inline TPoint const & GetPosition()const
		{
			return m_position;
		}

		/** Retrieves the vertex texture coordinates
		@return
			The value
		*/
		inline Point2f & GetTexture()
		{
			return m_texture;
		}

		/** Retrieves the vertex texture coordinates
		@return
			The value
		*/
		inline Point2f const & GetTexture()const
		{
			return m_texture;
		}

		/** Retrieves a constant pointer to the vertex buffer
		@return
			The value
		*/
		inline const uint8_t * ConstPtr()const
		{
			return m_pBuffer;
		}

		/** Retrieves a pointer to the vertex buffer
		@return
			The value
		*/
		inline uint8_t * Ptr()
		{
			return m_pBuffer;
		}

		/** Retrieves the vertex stride (size of a vertex)
		@return
			The value
		*/
		inline size_t GetStride()const
		{
			return 2 * sizeof( PosType ) + 2 * sizeof( float );
		}

		/** Retrieves the vertex position component At wanted index
		@param[in] p_uiIndex
			The index
		@return
			The value
		*/
		inline PosType & operator[]( size_t p_uiIndex )
		{
			return GetPosition()[p_uiIndex];
		}

		/** Retrieves the vertex position constant component At wanted index
		@param[in] p_uiIndex
			The index
		@return
			The value
		*/
		inline PosType const & operator[]( size_t p_uiIndex )const
		{
			return GetPosition()[p_uiIndex];
		}

	private:
		inline void DoLink()
		{
			m_position.Link( &m_pBuffer[0] );
			m_texture.Link( &m_pBuffer[2 * sizeof( PosType )] );
		}

	public:
		//! Number of components of a Vertex (total of coordinates for coords, normal, tangent, texture coords ).
		static size_t Size;
		//! Total number of created vertex
		static unsigned long long Count;

	private:
		//! Tells if this element owns it's buffer or not
		bool m_bOwnBuffer;
		//! The buffer, containing buffer values
		uint8_t * m_pBuffer;
		//! The vertex position
		TPoint m_position;
		//! The vertex texture coordinates
		Point2f m_texture;
	};
}

#endif
