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
#ifndef ___IFX_IMAGE_EFFECTS_H___
#define ___IFX_IMAGE_EFFECTS_H___

#include <Generator.h>

#include "IfxCpuStep.h"
#include <GpuStep.h>

namespace ImageEffects
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		23/05/2012
	@brief
		Image effects generator
	*/
	class Generator
		: public ProceduralTextures::Generator< CpuStep, ProceduralTextures::DefaultGpuStep >
	{
	private:
		/*!
		@author
			Sylvain DOREMUS
		@date
			23/05/2012
		@brief
			The controls IDs
		*/
		typedef enum
		{
			eID_ANY				= -1,
			eID_FIRSTFUNCTION	= 50,
			eID_SECONDFUNCTION	= 51,
			eID_IMAGEPATH		= 52,
		}	eID;

	public:
		/** Constructor
		*/
		Generator();

		/** Destructor
		*/
		virtual ~Generator();

	private:
		/** @copydoc ProceduralTexture::Generator::DoCreate
		*/
		virtual void DoCreate( ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize );

		/** @copydoc ProceduralTexture::Generator::DoDestroy
		*/
		virtual void DoDestroy();

		/** @copydoc ProceduralTexture::Generator::DoGeneratePanel
		*/
		virtual void DoGeneratePanel();

	private:
		/** Sets the computing image size
		@param[in] p_value
			The new value
		*/
		void OnSize( ProceduralTextures::Size & p_value );

		/** Sets the first function
		@param[in] p_value
			The new value
		*/
		void OnFirstFunction( uint32_t p_value );

		/** Sets the second function
		@param[in] p_value
			The new value
		*/
		void OnSecondFunction( uint32_t p_value );

		/** Sets the image
		@param[in] p_value
			The new value
		*/
		void OnImage();

	private:
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticFirstFunction;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboFirstFunction;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticSecondFunction;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboSecondFunction;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonImage;
	};
}

#endif
