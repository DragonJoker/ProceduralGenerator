/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_GPU_STEP_H___
#define ___GENERATOR_GPU_STEP_H___

#include "Position.h"
#include "Size.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <chrono>

namespace ProceduralTextures
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		12/03/2015
	@version
		2.0.0
	@brief
		Base class for every GPU step
	*/
	class GeneratorAPI GpuStep
		: public std::enable_shared_from_this< GpuStep >
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The displayed surface dimensions
		@param[in] p_bordersSize
			The windows' borders size
		*/
		GpuStep( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size, Size const & p_bordersSize );

		/** Destructor
		*/
		virtual ~GpuStep();

		/** Initialises the generator
		*/
		void Initialise();

		/** Cleans up the generator
		*/
		void Cleanup();

		/** Renders the frame : parallel execution of CPU and GPU steps
		@param[in] p_swapFunction
			The function used to swap OpenGL buffers
		*/
		void Render( std::function< void() > p_swapFunction );

		/** Resize the generator computation dimensions
		@param[in] p_iWidth, p_iHeight
			The new dimensions
		*/
		void Resize( int p_iWidth, int p_iHeight );

		/** Resize the generator computation dimensions
		@param[in] p_size
			The new dimensions
		*/
		void Resize( Size const & p_size );

		/** Sets displayed image size, respecting the working image ratio
		@param[in] p_size
			The new value
		*/
		void UpdateDisplaySize( Size const & p_size );

		/** Tells the next frame must be saved
		*/
		void SaveFrame();

		/** Updates the final buffer to the one given
		@param[in] p_buffer
			The new value
		*/
		void UpdateBuffer( PixelBuffer const & p_buffer );

		/** Tells if the generator is initialised
		@return
			The value
		*/
		inline bool IsInitialised()const
		{
			return m_initialised;
		}

		/** Retrieves the displayed image dimensions
		@return
			The value
		*/
		inline Size const & GetDisplaySize()const
		{
			return m_sizeDisplay;
		}

		/** Retrieves the displayed image offset
		@return
			The value
		*/
		inline Position const & GetDisplayOffset()const
		{
			return m_displayOffset;
		}

		/** Retrieves the saved frame buffer
		@return
			The value
		*/
		inline PixelBuffer const & GetSavedFrame()const
		{
			return *m_saveBuffer;
		}

		/** Retrieves the saved frame buffer
		@return
			The value
		*/
		inline PixelBuffer & GetSavedFrame()
		{
			return *m_saveBuffer;
		}

		/** Retrieves the overlay manager
		@return
			The value
		*/
		inline std::shared_ptr< OverlayManager > GetOverlayManager()
		{
			return m_overlayManager;
		}

		/** Retrieves the GPU step duration
		@return
			The value
		*/
		inline std::chrono::milliseconds const & GetTime()const
		{
			return m_time;
		}

		/** Retrieves the button texture
		@return
			The value
		*/
		inline std::shared_ptr< gl::Texture > GetButtonTexture()const
		{
			return m_textureButton;
		}

		/** Retrieves the button mouse over texture
		@return
			The value
		*/
		inline std::shared_ptr< gl::Texture > GetButtonMouseOverTexture()const
		{
			return m_textureButtonMouseOver;
		}

	protected:
		/** All OpenGL global initialisations must be made in this function
		*/
		virtual void DoInitialise() = 0;

		/** All OpenGL global cleanups must be made in this function
		*/
		virtual void DoCleanup() = 0;

		/** Resize things like frame buffers, render textures, ... to the image size
		*/
		virtual void DoResize() = 0;

		/** All OpenGL frame initialisations must be made in this function
		*/
		virtual void DoPreRender() = 0;

		/** All OpenGL draw calls must be made in this function
		@remarks
			Drawings will go in the generator frame buffer
		*/
		virtual void DoRender( bool & p_bChanged ) = 0;

		/** All OpenGL frame cleanups must be made in this function
		*/
		virtual void DoPostRender() = 0;

		/** Renders the quad, using the enabled program and binding the given vertex and index attributes
		@param[in] p_vertex
			The vertex attribute
		@param[in] p_texture
			The texture attribute
		*/
		void DoSubRender( uint32_t p_vertex, uint32_t p_texture );

		/** Renders the given framebuffer into the main framebuffer
		@param[in] p_frameBuffer
			The framebuffer to render
		*/
		void DoRenderFrameBuffer( gl::FrameBuffer & p_frameBuffer );

	private:
		/** Initialises internal buffers to the given size
		@param[in] p_size
			The new value
		*/
		void DoInitBuffers( Size const & p_size );

		/** OpenGL pre-render operations
		*/
		void PreRender();

		/** OpenGL render operations
		@param[in] p_bChanged
			Tells if the computed image has changed
		*/
		void Render( bool p_bChanged );

		/** OpenGL post-render operations
		*/
		void PostRender();

		/** Retrieves the saved framebuffer content into the saved data buffer
		*/
		void DoSaveFrame();

	protected:
		//! The parent generator
		std::weak_ptr< GeneratorBase > m_generator;
		//! The windows' borders size
		Size m_bordersSize;
		//! The copmuting image dimensions
		Size m_sizeImage;
		//! The displayed image dimensions
		Size m_sizeDisplay;
		//! The displayed image offset from origin
		Position m_displayOffset;
		//! The front buffer
		std::shared_ptr< PixelBuffer > m_finalBuffer;
		//! Tells we need to save a frame
		bool m_toSave;
		//! The saved buffer
		std::shared_ptr< PixelBuffer > m_saveBuffer;
		//! Tells the generator is initialised
		bool m_initialised;
		//! The OpenGL instance
		std::unique_ptr< gl::OpenGl > m_openGl;
		//! The geometry buffer
		std::shared_ptr< GeometryBuffers > m_geometryBuffers;
		//! The computations result
		std::shared_ptr< gl::Texture > m_texture;
		//! The framebuffer
		std::shared_ptr< gl::FrameBuffer > m_frameBuffer;
		//! The saved texture
		std::shared_ptr< gl::Texture > m_textureSave;
		//! The framebuffer where the saved frame is rendered
		std::shared_ptr< gl::FrameBuffer > m_frameBufferSave;
		//! The overlay texture
		std::shared_ptr< gl::Texture > m_textureOverlay;
		//! The framebuffer where the overlay frame is rendered
		std::shared_ptr< gl::FrameBuffer > m_frameBufferOverlay;
		//! The rendering shader program
		std::shared_ptr< gl::ShaderProgram > m_program;
		//! The uniform variable holding the index of the texture to render
		std::weak_ptr< gl::FrameVariable< int > > m_uniformImage;
		//! The button texture
		std::shared_ptr< gl::Texture > m_textureButton;
		//! The button mouse over texture
		std::shared_ptr< gl::Texture > m_textureButtonMouseOver;
		//! The vertex attribute "vertex" location in the program
		uint32_t m_vertexAttribLocation;
		//! The vertex attribute "texture" location in the program
		uint32_t m_textureAttribLocation;
		//! The GPU step duration
		std::chrono::milliseconds m_time;
		//! The overlay manager
		std::shared_ptr< OverlayManager > m_overlayManager;
	};
	/*!
	@author
		Sylvain DOREMUS
	@date
		12/08/2015
	@version
		2.0.1
	@brief
		Default GPU step, used by plugins that don't work with GPU
	*/
	class GeneratorAPI DefaultGpuStep
		: public GpuStep
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The displayed surface dimensions
		@param[in] p_bordersSize
			The windows' borders size
		*/
		DefaultGpuStep( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size, Size const & p_bordersSize );

		/** Destructor
		*/
		virtual ~DefaultGpuStep();

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
	};
}

#endif
