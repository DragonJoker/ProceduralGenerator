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
#ifndef ___GENERATOR_GL_T_FRAME_VARIABLE_H___
#define ___GENERATOR_GL_T_FRAME_VARIABLE_H___

#include "GlFrameVariableBase.h"
#include "GlShaderProgram.h"
#include "OpenGl.h"

#include <array>
#include <memory>

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
		Represent a templated uniform variable to pass to shaders
	@args T
		The variable components data type
	@args Count
		The variable components count
	*/
	template< typename T, uint32_t Count >
	class GeneratorAPI TFrameVariable
		: public FrameVariableBase
		, public std::enable_shared_from_this< TFrameVariable< T, Count > >
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_program
			The parent shader program
		 */
		TFrameVariable( OpenGl & p_openGl, ShaderProgram & p_program )
			: FrameVariableBase( p_openGl, p_program )
		{
			for ( T & l_value : m_value )
			{
				l_value = T();
			}
		}

		/** Destructor
		 */
		virtual ~TFrameVariable()
		{
		}

		/** Assigns and activate the frame variable
		@return
			true if the variable is assigned and bound, or if it is not used in the shader program
		 */
		virtual bool Apply()
		{
			bool l_bReturn = true;

			if ( m_used )
			{
				if ( m_glIndex == -1 )
				{
					m_glIndex = m_program.GetUniformLocation( m_strName );
				}

				if ( m_glIndex != -1 )
				{
					l_bReturn = TVariableApplier< T, Count >::Apply( GetOpenGl(), this->shared_from_this() );
				}
				else
				{
					m_used = false;
				}
			}

			return l_bReturn;
		}

		/** Retrieves the variable's value
		@return
			The value
		 */
		inline T const * GetValue()const
		{
			return &m_value[0];
		}

		/** Retrieves the variable's count
		@return
			The value
		 */
		inline uint32_t GetCount()const
		{
			return Count;
		}

	protected:
		//! The frame variable value
		std::array< T, Count > m_value;
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		14/02/2010
	@brief
		Applies a 1 component uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 1 >
	{
		/** Gives the variable value to the shader program
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_pVariable
			The uniform variable
		@return
			true if the OpenGL call was successful
		 */
		static bool Apply( OpenGl & p_openGl, std::shared_ptr< TFrameVariable< T, 1 > > p_pVariable )
		{
			return p_openGl.Uniform( p_pVariable->GetGlIndex(), *p_pVariable->GetValue() );
		}
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		14/02/2010
	@brief
		Applies a 2 components uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 2 >
	{
		/** Gives the variable value to the shader program
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_pVariable
			The uniform variable
		@return
			true if the OpenGL call was successful
		 */
		static bool Apply( OpenGl & p_openGl, std::shared_ptr< TFrameVariable< T, 2 > > p_pVariable )
		{
			return p_openGl.UniformVec2( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		14/02/2010
	@brief
		Applies a 3 components uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 3 >
	{
		/** Gives the variable value to the shader program
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_pVariable
			The uniform variable
		@return
			true if the OpenGL call was successful
		 */
		static bool Apply( OpenGl & p_openGl, std::shared_ptr< TFrameVariable< T, 3 > > p_pVariable )
		{
			return p_openGl.UniformVec3( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		14/02/2010
	@brief
		Applies a 4 components uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 4 >
	{
		/** Gives the variable value to the shader program
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_pVariable
			The uniform variable
		@return
			true if the OpenGL call was successful
		 */
		static bool Apply( OpenGl & p_openGl, std::shared_ptr< TFrameVariable< T, 4 > > p_pVariable )
		{
			return p_openGl.UniformVec4( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		14/02/2010
	@brief
		Applies a 4x4 matrix uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 4 * 4 >
	{
		/** Gives the variable value to the shader program
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_pVariable
			The uniform variable
		@return
			true if the OpenGL call was successful
		 */
		static bool Apply( OpenGl & p_openGl, std::shared_ptr< TFrameVariable< T, 4 * 4 > > p_pVariable )
		{
			return p_openGl.UniformMat4( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};
}
}

//*************************************************************************************************

#endif
