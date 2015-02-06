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
#ifndef ___GENERATOR_CONTROL_INFOS_H___
#define ___GENERATOR_CONTROL_INFOS_H___

#include "GeneratorPrerequisites.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Description of a control, in order to be able to create it completely
	\remark	It also holds an event and a callback function to call on reaction to the event
	*/
	class GeneratorAPI ControlInfos
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_eType			The control type
		 *\param[in]	p_iId			The control ID
		 *\param[in]	p_ptPosition	The control position
		 *\param[in]	p_szSize		The control dimensions
		 *\param[in]	p_pSpecific		Control's specific data
		 *\param[in]	p_iStyle		The control style
		 *\param[in]	p_event			An event on which a function is called
		 *\param[in]	p_pfnFunction	Function to use for specific event launched by the control
		 *\param[in]	p_bVisible		Initial visibility status
		 */
		ControlInfos( eCONTROL_TYPE p_eType, wxWindowID p_iId, wxPoint const & p_ptPosition, wxSize const & p_szSize, SpecificControlParametersBase * p_pSpecific, int p_iStyle = 0, eEVENT_TYPE p_eEventType = eEVENT_TYPE_NONE, wxObjectEventFunction p_pfnFunction = NULL, bool p_bVisible = true );
		/**
		 *\brief		Copy constructor
		 *\param[in]	p_copy	Element to copy
		 */
		ControlInfos( ControlInfos const & p_copy );
		/**
		 *\brief		Move constructor
		 *\param[in]	p_copy	Element to move
		 */
		ControlInfos( ControlInfos && p_copy );
		/**
		 *\brief		Copy assignment operator
		 *\param[in]	p_copy	Element to copy
		 *\return		A reference over the current element
		 */
		ControlInfos & operator =( ControlInfos const & p_copy );
		/**
		 *\brief		Move assignment operator
		 *\param[in]	p_copy	Element to move
		 *\return		A reference over the current element
		 */
		ControlInfos & operator =( ControlInfos && p_copy );
		/**
		 *\brief		Destructor
		 */
		virtual ~ControlInfos();

	public:
		//! The control type
		eCONTROL_TYPE m_eType;
		//! The control ID
		wxWindowID m_iId;
		//! The control position
		wxPoint m_ptPosition;
		//! The control dimensions
		wxSize m_szSize;
		//! Control's specific data
		SpecificControlParametersBase * m_pSpecific;
		//! The control style
		int m_iStyle;
		//! An event on which a function is called
		eEVENT_TYPE m_eEventType;
		//! Function to use for specific event launched by the control
		wxObjectEventFunction m_pfnFunction;
		//! Initial visibility status
		bool m_bVisible;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Typedef on an array of ControlInfos
	*/
	typedef std::vector< ControlInfos > CtrlInfosArray;
}

#endif
