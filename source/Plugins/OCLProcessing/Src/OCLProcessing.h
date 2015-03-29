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
#ifndef ___OCL_OCL_PROCESSING_H___
#define ___OCL_OCL_PROCESSING_H___

#include <Generator.h>

#include "OclCpuStep.h"
#include "OclGpuStep.h"

namespace OCLProcessing
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Life game geenrator
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
			eID_ANY				= -1,
			eID_IMAGEPATH		= 52,
			eID_SEPTYPE			= 53,
			eID_SEPOFFSET		= 54,
			eID_RESETTIME		= 55,
			eID_PROGRAMS		= 56,
			eID_REMOVE			= 57,
			eID_FILE			= 58,
			eID_KERNEL			= 59,
			eID_COMPILERLOG		= 60,
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
		 *\brief		Resets the time index
		 */
		void OnResetTime();
		/**
		 *\brief		Sets the separator type
		 *\param[in]	p_value	The new value
		 */
		void OnSepType( int p_value );
		/**
		 *\brief		Sets the separator offset
		 *\param[in]	p_value	The new value
		 */
		void OnSepOffset( int p_value );
		/**
		 *\brief		Selects a shader
		 */
		void OnSelectProgram( uint32_t p_value );
		/**
		 *\brief		Removes the current chader
		 */
		void OnRemove();
		/**
		 *\brief		Retrieves the compiler log
		 */
		void OnCompilerLog();
		/**
		 *\brief		Sets the OpenCL program file path
		 *\param[in]	p_path	The new value
		 */
		void OnFilePath();
		/**
		 *\brief		Sets the OpenCL kernel
		 *\param[in]	p_value	The new value
		 */
		void OnSelectKernel( uint32_t p_value );
		/**
		 *\brief		Sets the image buffer
		 *\param[in]	p_path	The new value
		 */

		void OnImage();

	private:
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonImage;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticSeparator;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboSeparator;
		std::shared_ptr< ProceduralTextures::SliderCtrl > m_sliderOffset;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonReset;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticPrograms;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboPrograms;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonFile;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticKernel;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboKernels;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonCompilerLog;
		std::shared_ptr< ProceduralTextures::ButtonCtrl  > m_buttonRemove;
	};
}

#endif
