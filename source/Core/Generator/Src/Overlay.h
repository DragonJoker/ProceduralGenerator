/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

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
#ifndef ___GENERATOR_OVERLAY_H___
#define ___GENERATOR_OVERLAY_H___

#include "GeneratorPrerequisites.h"
#include "Point.h"
#include "Position.h"
#include "Pixel.h"
#include "Size.h"
#include "GlHolder.h"
#include "Material.h"

#pragma warning( push )
#pragma warning( disable:4251 )
#pragma warning( disable:4275 )

#ifdef DrawText
#	undef DrawText
#endif

namespace ProceduralTextures
{
	/*!
	\author 	Sylvain DOREMUS
	\date 		23/02/2015
	\version	2.0.0
	\brief		Overlay class
	*/
	class GeneratorAPI Overlay
		: public std::enable_shared_from_this< Overlay >
		, public gl::Holder
	{
	protected:
		typedef std::map< int, std::weak_ptr< Overlay > >::iterator iterator;
		typedef std::map< int, std::weak_ptr< Overlay > >::const_iterator const_iterator;

	protected:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGl instance
		 *\param[in]	p_material	The material
		 *\param[in]	p_type		The overlay type
		 *\param[in]	p_parent	The parent overlay (if any)
		 */
		Overlay( std::shared_ptr< gl::OpenGl > p_openGl, Material const & p_material, eOVERLAY_TYPE p_type, std::shared_ptr< Overlay > p_parent );

	public:
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		virtual ~Overlay();
		/**
		 *\brief		Draws the overlay
		 *\param[in]	p_size	The rendering surface size
		 */
		void Render( Size const & p_size );
		/**
		 *\brief		Adds a child to the overlay
		 *\param[in]	p_pOverlay	The overlay to add
		 *\param[in]	p_iZIndex	The wanted Z Index for the child
		 *\return		\p true if successful, false if not
		 */
		bool AddChild( std::shared_ptr< Overlay > p_pOverlay, uint64_t p_iZIndex );
		/**
		 *\brief		Initialises the overlay and its buffers
		 */
		void Initialise();
		/**
		 *\brief		Cleans the overlay and its buffers up
		 */
		void Cleanup();
		/**
		 *\brief		Updates the overlay position and size, taking care of wanted pixel size and position
		 *\param[in]	p_size	The display size
		 */
		void UpdatePositionAndSize( Size const & p_size );
		/**
		 *\brief		Retrieves the overlay absolute position
		 *\return		The value
		 */
		Point2d GetAbsolutePosition()const;
		/**
		 *\brief		Retrieves the overlay absolute size
		 *\return		The value
		 */
		Point2d GetAbsoluteSize()const;
		/**
		 *\brief		Retrieves the overlay absolute position, in pixels
		 *\return		The value
		 */
		Position GetAbsolutePixelPosition()const;
		/**
		 *\brief		Retrieves the overlay absolute size, in pixels
		 *\return		The value
		 */
		Size GetAbsolutePixelSize()const;
		/**
		 *\brief		Sets the shader program
		 *\param[in]	p_type		The new value
		 *\param[in]	p_program	The new value
		 */
		virtual void SetProgram( eMATERIAL_TYPE p_type, std::shared_ptr< gl::ShaderProgram > p_program );
		/**
		 *\brief		Sets the colour of the overlay
		 *\param[in]	p_colour	The new colour
		 */
		void SetColour( Colour const & p_colour );
		/**
		 *\brief		Sets the texture of the overlay
		 *\param[in]	p_texture	The new texture
		 */
		void SetTexture( std::shared_ptr< gl::Texture > p_texture );
		/**
		 *\brief		Retrieves the visibility status
		 *\return		The value
		 */
		bool IsVisible()const;
		/**
		 *\brief		Sets the visibility status
		 *\param[in]	p_visible	The new value
		 */
		void SetVisible( bool p_visible );
		/**
		 *\brief		Retrieves the overlay position
		 *\return		The value
		 */
		inline Point2d const & GetPosition()const
		{
			return m_position;
		}
		/**
		 *\brief		Sets the overlay position
		 *\param[in]	p_position	The new value
		 */
		void SetPosition( Point2d const & p_position )
		{
			m_position = p_position;
			m_pixelPositioned = false;
			m_changed = true;
		}
		/**
		 *\brief		Retrieves the overlay size
		 *\return		The value
		 */
		inline Point2d const & GetSize()const
		{
			return m_size;
		}
		/**
		 *\brief		Sets the overlay size
		 *\param[in]	p_size	The new value
		 */
		void SetSize( Point2d const & p_size )
		{
			m_size = p_size;
			m_changed = true;
		}
		/**
		 *\brief		Retrieves the overlay size in pixels
		 *\return		The value
		 */
		inline Size const & GetPixelSize()const
		{
			return m_pixelSize;
		}
		/**
		 *\brief		Sets the overlay size in pixels
		 *\param[in]	p_size	The new value
		 */
		inline void SetPixelSize( Size const & p_size )
		{
			m_pixelSize = p_size;
			m_changed = true;
		}
		/**
		 *\brief		Retrieves the overlay position in pixels
		 *\return		The value
		 */
		inline Position const & GetPixelPosition()const
		{
			return m_pixelPosition;
		}
		/**
		 *\brief		Sets the overlay position in pixels
		 *\param[in]	p_position	The new value
		 */
		inline void SetPixelPosition( Position const & p_position )
		{
			m_pixelPosition = p_position;
			m_pixelPositioned = true;
			m_changed = true;
		}
		/**
		 *\brief		Retrieves the overlay type
		 *\return		The value
		 */
		inline eOVERLAY_TYPE GetOverlayType()const
		{
			return m_overlayType;
		}
		/**
		 *\brief		Retrieves the overlay type
		 *\return		The value
		 */
		inline eMATERIAL_TYPE GetMaterialType()const
		{
			return m_material.GetType();
		}
		/**
		 *\brief		Retrieves the overlay name
		 *\return		The value
		 */
		inline String const & GetName()const
		{
			return m_name;
		}
		/**
		 *\brief		Sets the name of the overlay
		 *\param[in]	p_name	The new name
		 */
		inline void SetName( String const & p_name )
		{
			m_name = p_name;
		}
		/**
		 *\brief		Retrieves the overlay colour
		 *\return		The value
		 */
		inline Colour const & GetColour()const
		{
			return m_material.GetColour();
		}
		/**
		 *\brief		Retrieves the overlay texture
		 *\return		The value
		 */
		inline std::shared_ptr< gl::Texture > GetTexture()const
		{
			return m_material.GetTexture();
		}
		/**
		 *\brief		Retrieves the parent overlay
		 *\return		The value
		 */
		inline std::shared_ptr< Overlay > GetParent()const
		{
			return m_parent.lock();
		}
		/**
		 *\brief		Retrieves the Z-index for this overlay
		 *\return		The value
		 */
		inline uint64_t GetZIndex()const
		{
			return m_zIndex;
		}
		/**
		 *\brief		Sets the Z-index
		 *\param[in]	p_zIndex	The new value
		 */
		inline void SetZIndex( uint64_t p_zIndex )
		{
			m_zIndex = p_zIndex;
			m_changed = true;
		}
		/**
		 *\brief		Retrieves the overlay level
		 *\return		The value
		 */
		inline uint32_t GetLevel()const
		{
			return m_level;
		}
		/**
		 *\brief		Retrieves the childs count
		 *\return		The value
		 */
		inline uint32_t GetChildsCount()const
		{
			return uint32_t( m_overlaysByZIndex.size() );
		}
		/**
		 *\brief		Retrieves an iterator to the beginning of the childs map, sorted by Z-Index
		 *\return		The iterator
		 */
		inline std::map< uint64_t, std::weak_ptr< Overlay > >::iterator Begin()
		{
			return m_overlaysByZIndex.begin();
		}
		/**
		 *\brief		Retrieves an iterator to the beginning of the childs map, sorted by Z-Index
		 *\return		The iterator
		 */
		inline std::map< uint64_t, std::weak_ptr< Overlay > >::const_iterator Begin()const
		{
			return m_overlaysByZIndex.begin();
		}
		/**
		 *\brief		Retrieves an iterator to the end of the childs map, sorted by Z-Index
		 *\return		The iterator
		 */
		inline std::map< uint64_t, std::weak_ptr< Overlay > >::iterator End()
		{
			return m_overlaysByZIndex.end();
		}
		/**
		 *\brief		Retrieves an iterator to the end of the childs map, sorted by Z-Index
		 *\return		The iterator
		 */
		inline std::map< uint64_t, std::weak_ptr< Overlay > >::const_iterator End()const
		{
			return m_overlaysByZIndex.end();
		}

	protected:
		/**
		 *\brief		Initialises the overlay and its buffers
		 */
		virtual void DoInitialise() = 0;
		/**
		 *\brief		Cleans the overlay and its buffers up
		 */
		virtual void DoCleanup() = 0;
		/**
		 *\brief		Draws the overlay
		 */
		virtual void DoRender() = 0;
		/**
		 *\brief		Updates the vertex buffer
		 */
		virtual void DoUpdate() = 0;
		/**
		 *\brief		Updates the overlay position and size, taking care of wanted pixel size and position
		 *\param[in]	p_size	The display size
		 */
		virtual void DoUpdatePositionAndSize( Size const & p_size ) {}
		/**
		 *\brief		Check if this overlay has changed (or if its parent has changed)
		 *\return		The change status
		 */
		bool IsChanged()const;

	protected:
		//! The geometry buffers
		std::shared_ptr< GeometryBuffersI > m_geometryBuffers;
		//! The material
		Material m_material;
		//! The overlay name
		String m_name;
		//! The parent overlay, if any
		std::weak_ptr< Overlay > m_parent;
		//! The childs
		std::map< uint64_t, std::weak_ptr< Overlay > > m_overlaysByZIndex;
		//! The childs
		mutable std::vector< std::weak_ptr< Overlay > > m_arrayOverlays;
		//! The relative position (to parent or screen)
		Point2d m_position;
		//! The relative size (to parent or screen)
		Point2d m_size;
		//! The relative position (to parent or screen), in pixels
		Position m_pixelPosition;
		//! The absolute size in pixels
		Size m_pixelSize;
		//! The visibility
		bool m_visible;
		//! The overlay z index
		uint64_t m_zIndex;
		//! The overlay level (0 for root, incrementing for childs)
		int32_t m_level;
		//! The overlay type
		eOVERLAY_TYPE m_overlayType;
		//! Tells the overlay has changed and must be updated
		bool m_changed;
		//! The UV for the panel
		Point4d m_uv;
		//! The previous display size
		Size m_displaySize;
		//! Tells the overlay is positioned in pixels
		bool m_pixelPositioned;
	};
}

#pragma warning( pop )

#endif
