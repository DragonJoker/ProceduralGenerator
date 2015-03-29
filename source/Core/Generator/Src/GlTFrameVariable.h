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
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Represent a single (not vec or mat) uniform variable to pass to shaders
	*/
	template< typename T, uint32_t Count >
	class GeneratorAPI TFrameVariable
		: public FrameVariableBase
		, public std::enable_shared_from_this< TFrameVariable< T, Count > >
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_openGl	The OpenGL instance
		 *\param[in]	p_program	The parent shader program
		 */
		TFrameVariable( std::shared_ptr< OpenGl > p_openGl, std::shared_ptr< ShaderProgram > p_program )
			: FrameVariableBase( p_openGl, p_program )
		{
			for ( T & l_value : m_value )
			{
				l_value = T();
			}
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~TFrameVariable()
		{
		}
		/**
		 *\brief		Assigns and activate the frame variable
		 *\return		true if the variable is assigned and bound, or if it is not used in the shader program
		 */
		virtual bool Apply()
		{
			bool l_bReturn = true;

			if ( m_used )
			{
				if ( m_glIndex == -1 )
				{
					m_glIndex = GetProgram()->GetUniformLocation( m_strName );
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
		/**
		 *\brief		Retrieves the variable's value
		 *\return		The value
		 */
		inline T const * GetValue()const
		{
			return &m_value[0];
		}
		/**
		 *\brief		Retrieves the variable's count
		 *\return		The value
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
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Applies a single int uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 1 >
	{
		/**
		 *\brief		Gives the variable value to the shader program
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pVariable	The uniform variable
		 *\return		true if the OpenGL call was successful
		 */
		static bool Apply( std::shared_ptr< OpenGl > p_pOpenGl, std::shared_ptr< TFrameVariable< T, 1 > > p_pVariable )
		{
			return p_pOpenGl->Uniform( p_pVariable->GetGlIndex(), *p_pVariable->GetValue() );
		}
	};
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Applies a single int uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 2 >
	{
		/**
		 *\brief		Gives the variable value to the shader program
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pVariable	The uniform variable
		 *\return		true if the OpenGL call was successful
		 */
		static bool Apply( std::shared_ptr< OpenGl > p_pOpenGl, std::shared_ptr< TFrameVariable< T, 2 > > p_pVariable )
		{
			return p_pOpenGl->UniformVec2( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Applies a single int uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 3 >
	{
		/**
		 *\brief		Gives the variable value to the shader program
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pVariable	The uniform variable
		 *\return		true if the OpenGL call was successful
		 */
		static bool Apply( std::shared_ptr< OpenGl > p_pOpenGl, std::shared_ptr< TFrameVariable< T, 3 > > p_pVariable )
		{
			return p_pOpenGl->UniformVec3( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Applies a single float uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 4 >
	{
		/**
		 *\brief		Gives the variable value to the shader program
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pVariable	The uniform variable
		 *\return		true if the OpenGL call was successful
		 */
		static bool Apply( std::shared_ptr< OpenGl > p_pOpenGl, std::shared_ptr< TFrameVariable< T, 4 > > p_pVariable )
		{
			return p_pOpenGl->UniformVec4( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Applies a single float uniform variable
	*/
	template< typename T > struct GeneratorAPI TVariableApplier< T, 4 * 4 >
	{
		/**
		 *\brief		Gives the variable value to the shader program
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pVariable	The uniform variable
		 *\return		true if the OpenGL call was successful
		 */
		static bool Apply( std::shared_ptr< OpenGl > p_pOpenGl, std::shared_ptr< TFrameVariable< T, 4 * 4 > > p_pVariable )
		{
			return p_pOpenGl->UniformMat4( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};
}
}

//*************************************************************************************************

#endif
