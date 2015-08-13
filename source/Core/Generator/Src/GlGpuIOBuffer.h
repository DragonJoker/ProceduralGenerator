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
#ifndef ___GENERATOR_GL_PIXEL_BUFFER_H___
#define ___GENERATOR_GL_PIXEL_BUFFER_H___

#include "GlBufferObject.h"

#include "Vertex.h"

namespace ProceduralTextures
{
namespace gl
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		14/02/2010
	@brief
		Base class for pixel transfer buffers
	*/
	class GeneratorAPI GpuIOBuffer
		: public BufferObject
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_packMode
			The pixel transfer direction (pack or unpack)
		@param[in] p_drawMode
			The buffer mode (GL_STATIC_DRAW, ...)
		 */
		GpuIOBuffer( OpenGl & p_openGl, unsigned int p_packMode, unsigned int p_drawMode );

		/** Destructor
		 */
		virtual ~GpuIOBuffer();

		/** Initialises the buffer
		@param[in] p_size
			The buffer size
		@return
			true if it is successfully initialised
		 */
		virtual bool Initialise( size_t p_size );

		/** Tries to activate the buffer
		@return
			true if it is successfully activated
		 */
		virtual bool Activate();

		/** Deactivates the buffer
		 */
		virtual void Deactivate();

	private:
		/** Dummy, not to use
		@return
			false
		 */
		virtual bool Initialise();
	};
}
}

#endif
