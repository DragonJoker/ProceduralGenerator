/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___DLA_DIFFUSION_LIMITED_AGGREGATION_H___
#define ___DLA_DIFFUSION_LIMITED_AGGREGATION_H___

#include <Generator.h>

#include "DlaCpuStep.h"
#include "DlaGpuStep.h"

namespace DiffusionLimitedAggregation
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Diffusion limited aggregation generator
	*/
	class Generator
		: public ProceduralTextures::Generator< CpuStep, GpuStep >
	{
	private:
		/*!
		\author		Sylvain DOREMUS
		\date		23/05/2012
		\brief		The controls IDs
		*/
		typedef enum
		{
			eID_ANY			= -1,
			eID_MASS		= 50,
			eID_MINVALUE	= 51,
		}	eID;

	public:
		/**
		 *\brief		Constructor
		 */
		Generator();
		/**
		 *\brief		Destructor
		 */
		virtual ~Generator();

	private:
		/**
		 *\copydoc		ProceduralTexture::Generator::DoCreate
		 */
		virtual void DoCreate( ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize );
		/**
		 *\copydoc		ProceduralTexture::Generator::DoDestroy
		 */
		virtual void DoDestroy();
		/**
		 *\copydoc		ProceduralTexture::Generator::DoGeneratePanel
		 */
		virtual void DoGeneratePanel();
		/**
		 *\brief		Sets the minimal value a cell must have to be considered 'white'
		 *\param[in]	val	The new value
		 */
		void DoSetMinValue( int p_value );
		/**
		 *\brief		Sets the 'white' cells count at which the generation stops
		 *\param[in]	val	The new value
		 */
		void DoSetMass( int p_value );

	private:
		//! The static control holding mass value
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticMass;
		//! The static control holding min value to consider a cell as 'white'
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticMinValue;
	};
}

#endif
