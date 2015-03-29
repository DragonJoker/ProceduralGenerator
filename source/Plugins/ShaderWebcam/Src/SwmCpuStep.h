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
#ifndef ___SWM_CPU_STEP_H___
#define ___SWM_CPU_STEP_H___

#include "SwmEffect.h"

#include <CpuStep.h>

namespace ShaderWebcam
{
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Shader webcam worker thread
	*/
	class Thread
		: public ProceduralTextures::Thread
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_parent	The parent generator
		 *\param[in]	p_index		The thread index
		 *\param[in]	p_width		The surface width
		 *\param[in]	p_top		The surface portion's top
		 *\param[in]	p_bottom	The surface portion's bottom
		 *\param[in]	p_height	The surface height
		 */
		Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_parent, size_t p_index, int p_width, int p_top, int p_bottom, int p_totalHeight );
		/**
		 *\brief		Destructor
		 */
		virtual ~Thread();
		/**
		 *\brief		Initialises the step thread
		 *\param[in]	p_pCapture	The OpenCV capture
		 *\param[in]	p_pBuffer	The pixel buffer
		 */
		void Initialise( std::shared_ptr< cv::VideoCapture > p_pCapture, std::shared_ptr< ProceduralTextures::PixelBuffer > p_pBuffer );

	private:
		/**
		 *\copydoc		ProceduralTexture::CpuStep::Thread::DoStep
		 */
		virtual void DoStep();

	private:
		//! The OpenCV capture
		std::weak_ptr< cv::VideoCapture > m_capture;
		//! The pixel buffer
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_pBuffer;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Shader webcam CPU step
	*/
	class CpuStep
		: public ProceduralTextures::CpuStep< Thread >
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_generator	The parent generator
		 *\param[in]	p_capture	The OpenCV capture
		 *\param[in]	p_size		The dimensions
		 */
		CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, std::shared_ptr< cv::VideoCapture > p_capture, ProceduralTextures::Size const & p_size );
		/**
		 *\brief		Destructor
		 */
		virtual ~CpuStep();

	private:
		/**
		 *\copydoc		ProceduralTexture::CpuStep::DoInitialiseStep
		 */
		virtual void DoInitialiseStep();
		/**
		 *\copydoc		ProceduralTexture::CpuStep::DoInitialise
		 */
		virtual void DoInitialise();
		/**
		 *\copydoc		ProceduralTexture::CpuStep::DoCleanup
		 */
		virtual void DoCleanup();
		/**
		 *\copydoc		ProceduralTexture::CpuStep::DoSwapBuffers
		 */
		virtual void DoSwapBuffers();
		/**
		 *\brief		Initialises OpenCV capture
		 */
		void DoInitialiseCapture();
		/**
		 *\brief		Cleans up OpenCV capture
		 */
		void DoCleanupCapture();

	private:
		//! The OpenCV capture
		std::weak_ptr< cv::VideoCapture > m_capture;
	};
}

#endif
