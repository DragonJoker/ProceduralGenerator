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
#ifndef ___GPU_GPU_PROGRAMMING_H___
#define ___GPU_GPU_PROGRAMMING_H___

#include <Generator.h>

#include "GpuCpuStep.h"
#include "GpuGpuStep.h"

namespace GpuProgramming
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		23/05/2012
	@brief
		GPU programming generator
	*/
	class Generator
		: public ProceduralTextures::Generator< CpuStep, GpuStep >
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
			eID_SEPTYPE			= 52,
			eID_RESETTIME		= 53,
			eID_SHADERS			= 54,
			eID_REMOVE			= 55,
			eID_VERTEXFILE		= 56,
			eID_FRAGMENTFILE	= 57,
			eID_COMPILE			= 58,
			eID_COMPILERLOG		= 59,
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

		/** Resets the time index
		 */
		void OnResetTime();

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
		 */
		void OnVertexShaderPath();

		/** Sets the fragment shader file path
		 */
		void OnFragmentShaderPath();

	private:
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
