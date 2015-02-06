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
#ifndef ___GENERATOR_SPECIFIC_CONTROL_PARAMETERS_BASE_H___
#define ___GENERATOR_SPECIFIC_CONTROL_PARAMETERS_BASE_H___

#include "GeneratorPrerequisites.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Base class for specific control type parameters
	*/
	class GeneratorAPI SpecificControlParametersBase
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_eCtrlType	The control type
		 */
		SpecificControlParametersBase( eCONTROL_TYPE p_eCtrlType )
			:	m_eCtrlType( p_eCtrlType )
		{
		}
		/**
		 *\brief		Destructor
		 */
		virtual ~SpecificControlParametersBase()
		{
		}
		/**
		 *\brief		Retrieves the control type
		 */
		eCONTROL_TYPE GetControlType()const
		{
			return m_eCtrlType;
		}

	public:
		//! The control type
		eCONTROL_TYPE m_eCtrlType;
	};
}

#endif
