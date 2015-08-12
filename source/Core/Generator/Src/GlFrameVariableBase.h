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
#ifndef ___GENERATOR_GL_FRAME_VARIABLE_BASE_H___
#define ___GENERATOR_GL_FRAME_VARIABLE_BASE_H___

#include "GlHolder.h"

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
		Uniform variable base class
	*/
	class GeneratorAPI FrameVariableBase
		: public Holder
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_program
			The parent shader program
		 */
		FrameVariableBase( std::shared_ptr< OpenGl > p_openGl, std::shared_ptr< ShaderProgram > p_program );

		/** Destructor
		 */
		virtual ~FrameVariableBase();

		/** Assigns and activate the frame variable
		@return
			true if the variable is assigned and bound, or if it is not used in the shader program
		 */
		virtual bool Apply() = 0;

		/** Retrieves the variable's name
		@return
			The value
		 */
		inline String GetName()const
		{
			return m_strName;
		}

		/** Retrieves the variable's location in the program
		@return
			The value
		 */
		inline int GetGlIndex()const
		{
			return m_glIndex;
		}

		/** Retrieves the variable's location in the program
		@return
			The value
		 */
		inline std::shared_ptr< ShaderProgram > GetProgram()const
		{
			return m_program.lock();
		}

		/** Defines the variable's name
		@param[in] p_value
			The new value
		 */
		inline void SetName( String const & p_value )
		{
			m_strName = p_value;
		}

		/** Defines the variable's changed status
		@param[in] p_value
			The new value
		 */
		inline void SetChanged( bool p_value = true )
		{
			m_changed = p_value;
		}

		/** Retrieves the variable's changed status
		@return
			The value
		 */
		inline bool IsChanged()const
		{
			return m_changed;
		}

	protected:
		//! The variable location in the program
		int m_glIndex;
		//! Tells if the variable is used in the program
		bool m_used;
		//! The parent shader program
		std::weak_ptr< ShaderProgram > m_program;
		//! The variable name
		String m_strName;
		//! Tells that the variable value has changed
		bool m_changed;
	};
}
}

#endif
