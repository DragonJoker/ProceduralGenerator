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
#ifndef ___SWM_GPU_STEP_H___
#define ___SWM_GPU_STEP_H___

#include "SwmEffect.h"

#include <GpuStep.h>

namespace ShaderWebcam
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Shader webcam GPU step
	*/
	class GpuStep
		: public ProceduralTextures::GpuStep
	{
	private:
		typedef std::vector< std::unique_ptr< Effect > > EffectPtrArray;

	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The displayed surface dimensions
		@param[in] p_bordersSize
			The windows' borders size
		 */
		GpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize );
		/** Destructor
		 */
		virtual ~GpuStep();
		/** Resets the time index
		 */
		void ResetTime();
		/** Sets the separator type
		@param[in] p_value
			The new value
		 */
		void SetSepType( int p_value );
		/** Sets the separator offset
		@param[in] p_value
			The new value
		 */
		void SetSepOffset( int p_value );
		/** Selects a shader
		 */
		void SelectEffect( uint32_t p_value );
		/** Removes the current effect
		 */
		void AddEffect();
		/** Removes the current effect
		 */
		void RemoveEffect();
		/** Compiles the shader
		 */
		void CompileEffect();
		/** Retrieves the compiler log
		 */
		ProceduralTextures::String GetCompilerLog();
		/** Sets the vertex shader file path
		@param[in] p_path
			The new value
		 */
		void SetVertexShaderPath( ProceduralTextures::String const & p_path );
		/** Sets the fragment shader file path
		@param[in] p_path
			The new value
		 */
		void SetFragmentShaderPath( ProceduralTextures::String const & p_path );

	private:
		/** @copydoc ProceduralTexture::GpuStep::DoInitialise
		 */
		virtual void DoInitialise();
		/** @copydoc ProceduralTexture::GpuStep::DoCleanup
		 */
		virtual void DoCleanup();
		/** @copydoc ProceduralTexture::GpuStep::DoResize
		 */
		virtual void DoResize();
		/** @copydoc ProceduralTexture::GpuStep::DoPreRender
		 */
		virtual void DoPreRender();
		/** @copydoc ProceduralTexture::GpuStep::DoRender
		 */
		virtual void DoRender( bool & p_bChanged );
		/** @copydoc ProceduralTexture::GpuStep::DoPostRender
		 */
		virtual void DoPostRender();

	private:
		//! The first framebuffer
		std::shared_ptr< ProceduralTextures::gl::FrameBuffer > m_frameBuffer1;
		//! The second framebuffer
		std::shared_ptr< ProceduralTextures::gl::FrameBuffer > m_frameBuffer2;
		//! The first framebuffer image
		std::shared_ptr< ProceduralTextures::gl::Texture > m_texture1;
		//! The second framebuffer image
		std::shared_ptr< ProceduralTextures::gl::Texture > m_texture2;
		//! The efects array
		EffectPtrArray m_arrayEffects;
		//! The separation type
		eSEPARATION m_eSeparationType;
		//! The separation offset
		int m_iSeparationOffset;
		//! Iterator to the selected effect
		EffectPtrArray::iterator m_itSelectedEffect;
	};
}

#endif
