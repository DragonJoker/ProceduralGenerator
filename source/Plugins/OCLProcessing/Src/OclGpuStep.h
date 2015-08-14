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
#ifndef ___OCL_GPU_STEP_H___
#define ___OCL_GPU_STEP_H___

#include "OclEffect.h"

#include <GpuStep.h>

namespace OCLProcessing
{
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Life game GPU step
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
		void SetSeparationType( int p_value );

		/** Sets the separator offset
		@param[in] p_value
			The new value
		*/
		void SetSeparationOffset( int p_value );

		/** Selects a effect
		*/
		void SelectEffect( uint32_t p_value );

		/** Retrieves the selected effect
		*/
		Effect * GetSelectedEffect()
		{
			return m_itSelectedEffect == m_arrayEffects.end() ? NULL : ( *m_itSelectedEffect ).get();
		}

		/** Removes the current effect
		*/
		void AddEffect();

		/** Removes the current effect
		@param[in] p_value
			The effect index
		*/
		void RemoveEffect( uint32_t p_value );

		/** Compiles the shader
		*/
		void CompileEffect();

		/** Selects a kernel from the current effect
		*/
		void SelectKernel( uint32_t p_value );

		/** Retrieves the compiler log
		*/
		ProceduralTextures::String GetCompilerLog();

		/** Sets the OpenCL program file path
		@param[in] p_path
			The new value
		*/
		bool SetFilePath( ProceduralTextures::String const & p_path );

		/** Sets the image buffer
		@param[in] p_path
			The new value
		*/
		void SetImage( ProceduralTextures::PixelBuffer const & p_image );

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
		//! The effects array
		EffectPtrArray m_arrayEffects;
		//! Iterator to the selected effect
		EffectPtrArray::iterator m_itSelectedEffect;
		//! The separation type
		eSEPARATION m_eSeparationType;
		//! The separation offset
		int m_iSeparationOffset;
		//! The output texture
		std::shared_ptr< ProceduralTextures::gl::Texture > m_outputTexture;
		//! The input OpenCL buffer
		std::shared_ptr< ProceduralTextures::PixelBuffer > m_inputBuffer;
		//! The output OpenCL buffer
		std::shared_ptr< ProceduralTextures::PixelBuffer > m_outputBuffer;
		//! The OpenCL platform
		cl::Platform m_clPlatform;
		//! The OpenCL device
		cl::Device m_clDevice;
		//! The OpenCL command queue
		cl::CommandQueue m_clQueue;
		//! The OpenCL cnotext
		cl::Context m_clContext;
	};
}

#endif
