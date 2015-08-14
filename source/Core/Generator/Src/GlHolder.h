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
#ifndef ___GENERATOR_GL_HOLDER_H___
#define ___GENERATOR_GL_HOLDER_H___

#include "GeneratorPrerequisites.h"

namespace ProceduralTextures
{
namespace gl
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		12/02/2015
	@version
		2.0.0
	@brief
		Base class for each class needing an OpenGl weak pointer
	*/
	class Holder
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGl instance
		*/
		Holder( OpenGl & p_openGl )
			: m_openGl( p_openGl )
		{
		}

		/** Destructor
		*/
		virtual ~Holder()
		{
		}

		/** Retrieves the OpenGl instance
		@return
			The instance
		*/
		OpenGl const & GetOpenGl()const
		{
			return m_openGl;
		}

		/** Retrieves the OpenGl instance
		@return
			The instance
		*/
		OpenGl & GetOpenGl()
		{
			return m_openGl;
		}

	private:
		//! The OpenGl instance
		OpenGl & m_openGl;
	};
}
}

#endif
