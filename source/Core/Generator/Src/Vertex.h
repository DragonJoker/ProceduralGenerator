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
#ifndef ___GENERATOR_VERTEX_H___
#define ___GENERATOR_VERTEX_H___

#include "Point.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\version	0.6.1.0
	\date		29/08/2011
	\brief		Vertex definition
	*/
	class GeneratorAPI Vertex
	{
	public:
		/**
		 *\brief		Specified constructor
		 */
		Vertex();
		/**
		 *\brief		Copy constructor
		 */
		Vertex( const Vertex & p_vertex );
		/**
		 *\brief		Constructor from the difference between second and first argument
		 */
		 Vertex( const Vertex & p_v1, const Vertex & p_v2 );
		/**
		 *\brief		Destructor
		 */
		virtual ~Vertex();
		/**
		 *\brief		Copy assignment operator
		 */
		virtual Vertex  & operator =( const Vertex & p_vertex );
		/**
		 *\brief		Links the vertex coords to the ones in parameter.
		 *\remarks		The vertex no longer owns it's coords
		 *\param[in]	p_pBuffer	The coordinates buffer
		 */
		void LinkCoords( float * p_pBuffer );
		/**
		 *\brief		Unlinks the vertex coords.
		 *\remarks		The vertex now owns it's coords
		 */
		void UnlinkCoords();
		/**
		 *\brief		Sets the vertex position
		 *\param[in]	val	The new value
		 */
		void SetCoords( const Point2f & val );
		/**
		 *\brief		Sets the vertex position
		 *\param[in]	x, y	The new value
		 */
		void SetCoords( float x, float y );
		/**
		 *\brief		Sets the vertex position
		 *\param[in]	p_pCoords	The new value
		 */
		void SetCoords( const float * p_pCoords );
		/**
		 *\brief		Sets the vertex texture coordinates
		 *\param[in]	val	The new value
		 */
		void SetTexCoord( const Point2f & val );
		/**
		 *\brief		Sets the vertex texture coordinates
		 *\param[in]	x, y	The new value
		 */
		void SetTexCoord( float x, float y );
		/**
		 *\brief		Sets the vertex texture coordinates
		 *\param[in]	p_pCoords	The new value
		 */
		void SetTexCoord( const float * p_pCoords );
		/**
		 *\brief		Retrieves the vertex position
		 *\return		The value
		 */
		inline Point2f & GetCoords()
		{
			return m_position;
		}
		/**
		 *\brief		Retrieves the vertex position
		 *\return		The value
		 */
		inline const Point2f & GetCoords()const
		{
			return m_position;
		}
		/**
		 *\brief		Retrieves the vertex texture coordinates
		 *\return		The value
		 */
		inline Point2f & GetTexCoord()
		{
			return m_texture;
		}
		/**
		 *\brief		Retrieves the vertex texture coordinates
		 *\return		The value
		 */
		inline const Point2f & GetTexCoord()const
		{
			return m_texture;
		}
		/**
		 *\brief		Retrieves a constant pointer to the vertex buffer
		 *\return		The value
		 */
		inline const float * const_ptr()const
		{
			return reinterpret_cast< const float * >( m_pBuffer );
		}
		/**
		 *\brief		Retrieves a pointer to the vertex buffer
		 *\return		The value
		 */
		inline float * ptr()
		{
			return reinterpret_cast< float * >( m_pBuffer );
		}
		/**
		 *\brief		Retrieves the vertex stride (size of a vertex)
		 *\return		The value
		 */
		inline size_t GetStride()const
		{
			return 4 * sizeof( float );
		}
		/**
		 *\brief		Retrieves the vertex position component at wanted index
		 *\param[in]	p_uiIndex	The index
		 *\return		The value
		 */
		inline float & operator []( size_t p_uiIndex )
		{
			return GetCoords()[p_uiIndex];
		}
		/**
		 *\brief		Retrieves the vertex position constant component at wanted index
		 *\param[in]	p_uiIndex	The index
		 *\return		The value
		 */
		inline const float & operator []( size_t p_uiIndex )const
		{
			return GetCoords()[p_uiIndex];
		}

	private:
		void DoLink();
        
	public:
		//! Number of components of a Vertex (total of coordinates for coords, normal, tangent, texture coords ).
		static size_t Size;
		//! Total number of created vertex
		static unsigned long long Count;

	private:
		//! Tells if this element owns it's buffer or not
		bool m_bOwnBuffer;
		//! The buffer, containing buffer values
		float * m_pBuffer;
		//! The vertex position
		Point2f m_position;
		//! The vertex texture coordinates
		Point2f m_texture;
	};
}

#endif
