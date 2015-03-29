/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

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
#ifndef ___GENERATOR_PANEL_OVERLAY_H___
#define ___GENERATOR_PANEL_OVERLAY_H___

#include "Overlay.h"

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
	\brief		A simple rectangular overlay
	*/
	class GeneratorAPI PanelOverlay
		:	public Overlay
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGl instance
		 *\param[in]	p_material	The material
		 *\param[in]	p_parent	The parent overlay (if any)
		 */
		PanelOverlay( std::shared_ptr< gl::OpenGl > p_openGl, Material const & p_material, std::shared_ptr< Overlay > p_parent );
		/**
		 *\brief		Destructor
		 */
		virtual ~PanelOverlay();

	protected:
		/**
		 *\brief		Initialises the overlay and its buffers
		 */
		virtual void DoInitialise();
		/**
		 *\brief		Cleans the overlay and its buffers up
		 */
		virtual void DoCleanup();
		/**
		 *\brief		Draws the overlay
		 */
		virtual void DoRender();
		/**
		 *\brief		Updates the vertex buffer
		 */
		virtual void DoUpdate();
	};
}

#pragma warning( pop )

#endif
