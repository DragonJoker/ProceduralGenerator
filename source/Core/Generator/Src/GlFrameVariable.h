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
#ifndef ___GENERATOR_GL_FRAME_VARIABLE_H___
#define ___GENERATOR_GL_FRAME_VARIABLE_H___

#include "GlFrameVariableBase.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Represent a single (not vec or mat) uniform variable to pass to shaders
	*/
	template< typename T >
	class GeneratorAPI GlFrameVariable
		: public GlFrameVariableBase
	{
	protected:
		friend class GlShaderProgram;

	protected:
		/**
		 *\brief		Constructor
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pProgram	The parent shader program
		 */
		GlFrameVariable( OpenGl * p_pOpenGl, GlShaderProgram * p_pProgram )
			:	GlFrameVariableBase( p_pOpenGl, p_pProgram )
			,	m_tValue( T() )
		{
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~GlFrameVariable()
		{
		}

	public:
		/**
		 *\brief		Assigns and activate the frame variable
		 *\return		true if the variable is assigned and bound, or if it is not used in the shader program
		 */
		virtual bool Apply()
		{
			bool l_bReturn = true;

			if ( m_used )
			{
				if ( m_glIndex == GL_INVALID_INDEX )
				{
					m_glIndex = m_pOpenGl->GetUniformLocation( m_pProgram->GetProgramObject(), m_strName.char_str() );
				}

				if ( m_glIndex != GL_INVALID_INDEX )
				{
					l_bReturn = VariableApplier< T >::Apply( m_pOpenGl, this );
				}
				else
				{
					m_used = false;
				}
			}

			return l_bReturn;
		}
		/**
		 *\brief		Defines the variable's value
		 *\param[in]	p_value	The new value
		 */
		inline void SetValue( T p_value )
		{
			m_tValue = p_value;
			m_bChanged = true;
		}
		/**
		 *\brief		Retrieves the variable's value
		 *\return		The value
		 */
		inline T GetValue()const
		{
			return m_tValue;
		}

	protected:
		//! The frame variable value
		T m_tValue;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Applies a single int uniform variable
	*/
	template <> struct GeneratorAPI VariableApplier< int >
	{
		/**
		 *\brief		Gives the variable value to the shader program
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pVariable	The uniform variable
		 *\return		true if the OpenGL call was successful
		 */
		static bool Apply( OpenGl * p_pOpenGl, GlFrameVariable< int > * p_pVariable )
		{
			return p_pOpenGl->Uniform1i( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Applies a single float uniform variable
	*/
	template <> struct GeneratorAPI VariableApplier< float >
	{
		/**
		 *\brief		Gives the variable value to the shader program
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pVariable	The uniform variable
		 *\return		true if the OpenGL call was successful
		 */
		static bool Apply( OpenGl * p_pOpenGl, GlFrameVariable< float > * p_pVariable )
		{
			return p_pOpenGl->Uniform1f( p_pVariable->GetGlIndex(), p_pVariable->GetValue() );
		}
	};
}

//*************************************************************************************************

#endif
