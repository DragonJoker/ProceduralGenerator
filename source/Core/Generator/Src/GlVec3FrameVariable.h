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
#ifndef ___GENERATOR_GL_VEC3_FRAME_VARIABLE_H___
#define ___GENERATOR_GL_VEC3_FRAME_VARIABLE_H___

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
		Represent a 3 components uniform variable to pass to shaders
	*/
	template< typename T >
	class GeneratorAPI Vec3FrameVariable
		: public TFrameVariable< T, 3 >
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_program
			The parent shader program
		 */
		Vec3FrameVariable( OpenGl & p_openGl, ShaderProgram & p_program )
			: TFrameVariable< T, 3 >( p_openGl, p_program )
		{
		}

		/** Destructor
		 */
		virtual ~Vec3FrameVariable()
		{
		}

		/** Defines the variable's value
		@param[in] p_x,
			p_y, p_z	The new value
		 */
		inline void SetValue( T p_x, T p_y, T p_z )
		{
			this->m_value[0] = p_x;
			this->m_value[1] = p_y;
			this->m_value[2] = p_z;
			this->SetChanged();
		}

		/** Defines the variable's value
		@param[in] p_point
			The new value
		 */
		inline void SetValue( Point< T, 3 > const & p_point )
		{
			this->m_value[0] = p_point[0];
			this->m_value[1] = p_point[1];
			this->m_value[2] = p_point[2];
			this->SetChanged();
		}
	};
}
}

//*************************************************************************************************

#endif
