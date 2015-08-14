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
#ifndef ___SWM_SHADER_WEBCAM_H___
#define ___SWM_SHADER_WEBCAM_H___

#include <Generator.h>

#include "SwmCpuStep.h"
#include "SwmGpuStep.h"

namespace ShaderWebcam
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		23/05/2012
	@brief
		Shader webcam generator
	*/
	class Generator
		: public ProceduralTextures::Generator< CpuStep, GpuStep >
	{
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

		/** Resets the time index
		*/
		void OnResetTime();

		/** Sets the separator type
		@param[in] p_value
			The new value
		*/
		void OnSepType( int p_value );

		/** Sets the separator offset
		@param[in] p_value
			The new value
		*/
		void OnSepOffset( int p_value );

		/** Selects a shader
		*/
		void OnSelectShader( uint32_t p_value );

		/** Removes the current chader
		*/
		void OnRemove();

		/** Compiles the shader
		*/
		void OnShaderCompile();

		/** Retrieves the compiler log
		*/
		void OnCompilerLog();

		/** Sets the vertex shader file path
		@param[in] p_path
			The new value
		*/
		void OnVertexShaderPath();

		/** Sets the fragment shader file path
		@param[in] p_path
			The new value
		*/
		void OnFragmentShaderPath();

		/** Displays the help
		*/
		void OnHelpOpen();

		/** Hides the help
		*/
		void OnHelpClose();

		/** Hides the help
		*/
		void OnHelpPanelClick( std::shared_ptr< ProceduralTextures::Control > p_static, ProceduralTextures::MouseEvent const & p_event );

	private:
		//! The OpenCV capture
		std::shared_ptr< cv::VideoCapture > m_capture;

		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticSeparator;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboSeparator;
		std::shared_ptr< ProceduralTextures::SliderCtrl > m_sliderOffset;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonReset;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticShaders;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboShaders;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonVertexFile;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonFragmentFile;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonCompile;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonCompilerLog;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonRemove;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonHelpOpen;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_panelHelp;
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_panelHelpText;
	};
}

#endif
