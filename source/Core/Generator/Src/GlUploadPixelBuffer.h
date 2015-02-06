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
#ifndef ___GENERATOR_GL_UNPACK_PIXEL_BUFFER_H___
#define ___GENERATOR_GL_UNPACK_PIXEL_BUFFER_H___

#include "GlPixelBuffer.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Pack (upload to VRAM) buffer implementation
	*/
	class GeneratorAPI GlUploadPixelBuffer
		: public GlPixelBuffer
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGL instance
		 */
		GlUploadPixelBuffer( OpenGl * p_pOpenGl );
		/**
		 *\brief		Destructor
		 */
		virtual ~GlUploadPixelBuffer();
	};
}

#endif
