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
#ifndef ___SFX_SHADER_EFFECTS_H___
#define ___SFX_SHADER_EFFECTS_H___

#include <Generator.h>

#include "SfxCpuStep.h"
#include "SfxGpuStep.h"

namespace ShaderEffects
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Shader effects generator
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
			eID_SHADERS			= 56,
			eID_REMOVE			= 57,
			eID_VERTEXFILE		= 58,
			eID_FRAGMENTFILE	= 59,
			eID_COMPILE			= 60,
			eID_COMPILERLOG		= 61,
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
		void OnSelectShader( uint32_t p_value );
		/**
		 *\brief		Removes the current chader
		 */
		void OnRemove();
		/**
		 *\brief		Compiles the shader
		 */
		void OnShaderCompile();
		/**
		 *\brief		Retrieves the compiler log
		 */
		void OnCompilerLog();
		/**
		 *\brief		Sets the vertex shader file path
		 *\param[in]	p_path	The new value
		 */
		void OnVertexShaderPath();
		/**
		 *\brief		Sets the fragment shader file path
		 *\param[in]	p_path	The new value
		 */
		void OnFragmentShaderPath();
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
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticShaders;
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboShaders;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonVertexFile;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonFragmentFile;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonCompile;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonCompilerLog;
		std::shared_ptr< ProceduralTextures::ButtonCtrl > m_buttonRemove;
	};
}

#endif
