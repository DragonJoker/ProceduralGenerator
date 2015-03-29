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
#ifndef ___GENERATOR_OVERLAY_MANAGER_H___
#define ___GENERATOR_OVERLAY_MANAGER_H___

#include "Overlay.h"
#include "GlHolder.h"

#pragma warning( push )
#pragma warning( disable:4251 )
#pragma warning( disable:4275 )

namespace ProceduralTextures
{
	/*!
	\author 	Sylvain DOREMUS
	\date 		23/02/2015
	\version	2.0.0
	\brief		Overlay collection, with additional add and remove functions to manage Z-Index
	*/
	class GeneratorAPI OverlayManager
		: gl::Holder
	{
	public:
		typedef std::vector< std::shared_ptr< Overlay > >::iterator iterator;
		typedef std::vector< std::shared_ptr< Overlay > >::const_iterator const_iterator;

	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGl instance
		 */
		OverlayManager( std::shared_ptr< gl::OpenGl > p_openGl );
		/**
		 *\brief		Destructor
		 */
		virtual ~OverlayManager();
		/**
		 *\brief		Initialises the programs
		 */
		void Initialise();
		/**
		 *\brief		Clears all overlays lists and destroys thee programs
		 */
		void Cleanup();
		/**
		 *\brief		Renders the overlays
		 *\param[in]	p_size	The rendering surface size
		 */
		void Render( Size const & p_size );
		/**
		 *\brief		Creates a panel overlay
		 *\param[in]	p_name		The overlay name
		 *\param[in]	p_position	The overlay position
		 *\param[in]	p_size		The overlay size
		 *\param[in]	p_material	The overlay material
		 *\param[in]	p_pParent	The parent overlay
		 *\return		The created overlay
		 */
		std::shared_ptr< PanelOverlay > CreatePanel( String const & p_name, Point2d const & p_position, Point2d const & p_size, Material const & p_material, std::shared_ptr< Overlay > p_parent = nullptr );
		/**
		 *\brief		Creates a panel overlay
		 *\param[in]	p_name		The overlay name
		 *\param[in]	p_position	The overlay pixel position
		 *\param[in]	p_size		The overlay pixel size
		 *\param[in]	p_material	The overlay material
		 *\param[in]	p_pParent	The parent overlay
		 *\return		The created overlay
		 */
		std::shared_ptr< PanelOverlay > CreatePanel( String const & p_name, Position const & p_position, Size const & p_size, Material const & p_material, std::shared_ptr< Overlay > p_parent = nullptr );
		/**
		 *\brief		Creates a border panel overlay
		 *\param[in]	p_name				The overlay name
		 *\param[in]	p_position			The overlay position
		 *\param[in]	p_size				The overlay size
		 *\param[in]	p_material			The overlay material
		 *\param[in]	p_bordersSize		The overlay borders size
		 *\param[in]	p_bordersMaterial	The overlay borders material
		 *\param[in]	p_pParent			The parent overlay
		 *\return		The created overlay
		 */
		std::shared_ptr< BorderPanelOverlay > CreateBorderPanel( String const & p_name, Point2d const & p_position, Point2d const & p_size, Material const & p_material, Point4d const & p_bordersSize, Material const & p_bordersMaterial, std::shared_ptr< Overlay > p_parent = nullptr );
		/**
		 *\brief		Creates a panel overlay
		 *\param[in]	p_name				The overlay name
		 *\param[in]	p_position			The overlay pixel position
		 *\param[in]	p_size				The overlay pixel size
		 *\param[in]	p_material			The overlay material
		 *\param[in]	p_bordersSize		The overlay borders pixel size
		 *\param[in]	p_bordersMaterial	The overlay borders material
		 *\param[in]	p_pParent			The parent overlay
		 *\return		The created overlay
		 */
		std::shared_ptr< BorderPanelOverlay > CreateBorderPanel( String const & p_name, Position const & p_position, Size const & p_size, Material const & p_material, Point4i const & p_bordersSize, Material const & p_bordersMaterial, std::shared_ptr< Overlay > p_parent = nullptr );
		/**
		 *\brief		Creates a border panel overlay
		 *\param[in]	p_name		The overlay name
		 *\param[in]	p_position	The overlay position
		 *\param[in]	p_size		The overlay size
		 *\param[in]	p_material	The overlay material
		 *\param[in]	p_font		The overlay font
		 *\param[in]	p_pParent	The parent overlay
		 *\return		The created overlay
		 */
		std::shared_ptr< TextOverlay > CreateText( String const & p_name, Point2d const & p_position, Point2d const & p_size, Material const & p_material, std::shared_ptr< Font > p_font, std::shared_ptr< Overlay > p_parent = nullptr );
		/**
		 *\brief		Creates a panel overlay
		 *\param[in]	p_name		The overlay name
		 *\param[in]	p_position	The overlay pixel position
		 *\param[in]	p_size		The overlay pixel size
		 *\param[in]	p_material	The overlay material
		 *\param[in]	p_font		The overlay font
		 *\param[in]	p_pParent	The parent overlay
		 *\return		The created overlay
		 */
		std::shared_ptr< TextOverlay > CreateText( String const & p_name, Position const & p_position, Size const & p_size, Material const & p_material, std::shared_ptr< Font > p_font, std::shared_ptr< Overlay > p_parent = nullptr );
		/**
		 *\brief		Add an overlay to the lists
		 *\param[in]	p_overlay	The overlay
		 *\param[in]	p_pParent	The parent overlay
		 */
		void AddOverlay( std::shared_ptr< Overlay > p_overlay, std::shared_ptr< Overlay > p_parent );
		/**
		 *\brief		Add an overlay to the lists
		 *\param[in]	p_overlay	The overlay
		 */
		void AddOverlay( std::shared_ptr< Overlay > p_overlay );
		/**
		 *\brief		Add an overlay to the lists, given it's z-index
		 *\param[in]	p_zIndex	The wanted z-index
		 *\param[in]	p_overlay	The overlay
		 */
		void AddOverlay( uint64_t p_zIndex, std::shared_ptr< Overlay > p_pOverlay );
		/**
		 *\brief		Checks if an overlay exists at given z-index
		 *\param[in]	p_zIndex	The z-index
		 *\return		\p true if an overlay is defined at the given z-index
		 */
		bool HasOverlay( uint64_t p_zIndex )const;
		/**
		 *\brief		Checks if an overlay with the given name exists
		 *\param[in]	p_name	The name
		 *\return		\p true if an overlay is defined with given name
		 */
		bool HasOverlay( String const & p_name )const;
		/**
		 *\brief		Retrieves the overlay with the given name
		 *\param[in]	p_name	The name
		 *\return		The overlay, \p nullptr if not found
		 */
		std::shared_ptr< Overlay > GetOverlay( String const & p_name )const;
		/**
		 *\brief		Retrieves the overlay at given z-index
		 *\param[in]	p_zIndex	The z-index
		 *\return		The overlay, \p nullptr if not found
		 */
		std::shared_ptr< Overlay > GetOverlay( int p_zIndex )const;
		/**
		 *\brief		Loads a font
		 *\param[in]	p_fontName	The font name
		 *\param[in]	p_height	The font height
		 *\param[in]	p_fontPath	The font file path
		 *\return		The loaded font
		 */
		std::shared_ptr< Font > LoadFont( String const & p_fontName, uint32_t p_height, String const & p_fontPath );
		/**
		 *\brief		Retrieves a font
		 *\param[in]	p_fontName	The font name
		 *\param[in]	p_height	The font height
		 *\return		The font, nullptr if it does not exist
		 */
		std::shared_ptr< Font > GetFont( String const & p_fontName, uint32_t p_height );
		/**
		 *\brief		Retrieves the shader program for overlays with a colour material
		 *\return		The program
		 */
		std::shared_ptr< gl::ShaderProgram > GetProgramColour()const
		{
			return m_programColour;
		}
		/**
		 *\brief		Retrieves the shader program for overlays with a texture material
		 *\return		The program
		 */
		std::shared_ptr< gl::ShaderProgram > GetProgramTexture()const
		{
			return m_programTexture;
		}
		/**
		 *\brief		Retrieves the shader program for text overlays with a colour material
		 *\return		The program
		 */
		std::shared_ptr< gl::ShaderProgram > GetProgramTextColour()const
		{
			return m_programTextColour;
		}
		/**
		 *\brief		Retrieves the shader program for text overlays with a texture material
		 *\return		The program
		 */
		std::shared_ptr< gl::ShaderProgram > GetProgramTextTexture()const
		{
			return m_programTextTexture;
		}

	private:
		/**
		 *\brief		Add an overlay to the lists
		 *\param[in]	p_overlay	The overlay
		 */
		void DoAddOverlay( std::shared_ptr< Overlay > p_overlay );
		/**
		 *\brief		Updates the orthographic matrix in the shaders
		 *\param[in]	p_size	The new size
		 */
		void DoUpdateSize( Size const & p_size );
		/**
		 *\brief		Updates the renderable overlays array
		 */
		void DoUpdate();

		//! Current available Z index
		uint64_t m_currentZIndex;
		//! The overlays, sorted by Z index
		OverlayPtrIntMap m_mapOverlaysByZIndex;
		//! The mutex protecting overlays lists (all and initialisable)
		mutable std::mutex m_mutex;
		//! The overlays to initialise array
		std::vector< std::weak_ptr< Overlay > > m_initialisable;
		//! The overlays array
		std::vector< std::shared_ptr< Overlay > > m_overlays;
		//! The program for overlays which have a colour material
		std::shared_ptr< gl::ShaderProgram > m_programColour;
		//! The MVP matrix for overlays which have a colour material program
		std::weak_ptr< gl::Mat4FrameVariable< float > > m_mvpColour;
		//! The program for overlays which have a texture material
		std::shared_ptr< gl::ShaderProgram > m_programTexture;
		//! The MVP matrix for overlays which have a texture material program
		std::weak_ptr< gl::Mat4FrameVariable< float > > m_mvpTexture;
		//! The program for text overlays which have a colour material
		std::shared_ptr< gl::ShaderProgram > m_programTextColour;
		//! The MVP matrix for text overlays which have a colour material program
		std::weak_ptr< gl::Mat4FrameVariable< float > > m_mvpTextColour;
		//! The program for text overlays which have a texture material
		std::shared_ptr< gl::ShaderProgram > m_programTextTexture;
		//! The MVP matrix for text overlays which have a texture material program
		std::weak_ptr< gl::Mat4FrameVariable< float > > m_mvpTextTexture;
		//! The loaded fonts
		std::map< String, std::shared_ptr< Font > > m_loadedFonts;
		//! The last displayed size
		Size m_size;
		//! Tells the overlays array has changed
		bool m_changed;
		//! The overlays array, renderable as is
		std::vector< std::weak_ptr< Overlay > > m_renderableOverlays;
	};
}

#pragma warning( pop )

#endif
