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
#ifndef ___GENERATOR_GL_FRAME_VARIABLE_BASE_H___
#define ___GENERATOR_GL_FRAME_VARIABLE_BASE_H___

#include "GeneratorPrerequisites.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Uniform variable base class
	*/
	class GeneratorAPI GlFrameVariableBase
	{
	protected:
		friend class GlShaderProgram;

	protected:
		/**
		 *\brief		Constructor
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pProgram	The parent shader program
		 */
		GlFrameVariableBase( OpenGl * p_pOpenGl, GlShaderProgram * p_pProgram );
		/**
		 *\brief		Destructor
		 */
		virtual ~GlFrameVariableBase();

	public:
		/**
		 *\brief		Assigns and activate the frame variable
		 *\return		true if the variable is assigned and bound, or if it is not used in the shader program
		 */
		virtual bool Apply() = 0;
		/**
		 *\brief		Retrieves the variable's name
		 *\return		The value
		 */
		inline wxString GetName()const
		{
			return m_strName;
		}
		/**
		 *\brief		Retrieves the variable's location in the program
		 *\return		The value
		 */
		inline int GetGlIndex()const
		{
			return m_glIndex;
		}
		/**
		 *\brief		Defines the variable's name
		 *\param[in]	p_value	The new value
		 */
		inline void SetName( const wxString & p_value )
		{
			m_strName = p_value;
		}
		/**
		 *\brief		Defines the variable's changed status
		 *\param[in]	p_value	The new value
		 */
		inline void SetChanged( bool p_value = true )
		{
			m_bChanged = p_value;
		}

	protected:
		//! The OpenGL instance
		OpenGl * m_pOpenGl;
		//! The variable location in the program
		int m_glIndex;
		//! Tells if the variable is used in the program
		bool m_used;
		//! The parent shader program
		GlShaderProgram * m_pProgram;
		//! The variable name
		wxString m_strName;
		//! Tells that the variable value has changed
		bool m_bChanged;
	};
}

#endif
