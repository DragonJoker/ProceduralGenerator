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
#ifndef ___TFX_TEXT_EFFECTS_H___
#define ___TFX_TEXT_EFFECTS_H___

#include <Generator.h>

#include "TfxCpuStep.h"
#include "TfxGpuStep.h"

namespace TextEffects
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Text effects generator
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
			eID_ANY = -1,
			eID_TEXTBGTYPE = 51,
			eID_TEXTBG_COLOUR,
			eID_TEXTBG_IMAGE,
			eID_FONT,
			eID_TEXT,
			eID_BACKGROUND,
		}	eIDs;

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
		 *\brief		Sets the foreground type
		 *\param[in]	p_value	The new value
		 */
		void OnForegroundType( int p_value );
		/**
		 *\brief		Event called when a char key is typed in the edit
		 *\param[in]	p_value	The new edit content
		 */
		void OnChar( ProceduralTextures::String const & p_value );
		/**
		 *\brief		Sets the background image
		 *\param[in]	p_value	The new value
		 */
		void OnBackgroundImage();
		/**
		 *\brief		Sets the foreground colour
		 *\param[in]	p_value	The new value
		 */
		void OnForegroundColour();
		/**
		 *\brief		Sets the foreground image
		 *\param[in]	p_value	The new value
		 */
		void OnForegroundImage();
		/**
		 *\brief		Sets the text font file
		 *\param[in]	p_value	The new value
		 */
		void OnFont();

	private:
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticForeground;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboForeground;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonForegroundColour;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonForegroundImage;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonBackgroundImage;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonFont;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticCaption;
		std::shared_ptr< ProceduralTextures::EditCtrl > m_editCaption;
	};
}

#endif
