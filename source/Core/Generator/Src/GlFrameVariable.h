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
#ifndef ___GENERATOR_GL_FRAME_VARIABLE_H___
#define ___GENERATOR_GL_FRAME_VARIABLE_H___

#include "GlTFrameVariable.h"

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
		Represent a single (not vec or mat) uniform variable to pass to shaders
	*/
	template< typename T >
	class GeneratorAPI FrameVariable
		: public TFrameVariable< T, 1 >
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_program
			The parent shader program
		*/
		FrameVariable( OpenGl & p_openGl, ShaderProgram & p_program )
			: TFrameVariable< T, 1 >( p_openGl, p_program )
		{
		}

		/** Destructor
		*/
		virtual ~FrameVariable()
		{
		}

		/** Defines the variable's value
		@param[in] p_value
			The new value
		*/
		inline void SetValue( T p_value )
		{
			this->m_value[0] = p_value;
			this->SetChanged();
		}
	};
}
}

#endif
