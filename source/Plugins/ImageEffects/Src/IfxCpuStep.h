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
#ifndef ___IFX_CPU_STEP_H___
#define ___IFX_CPU_STEP_H___

#include "EffectFunctor.h"

#include <CpuStep.h>

namespace ImageEffects
{
	class CpuStep;
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Image effects worker thread
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
		 *\brief		Sets the image buffer
		 *\param[in]	p_image	The buffer
		 */
		void SetImage( ProceduralTextures::PixelBuffer const & p_image );
		/**
		 *\brief		Sets the first effect function
		 *\param[in]	p_type	The type
		 */
		void SetFirstFunction( eEFFECT_TYPE p_type );
		/**
		 *\brief		Sets the second effect function
		 *\param[in]	p_type	The type
		 */
		void SetSecondFunction( eEFFECT_TYPE p_type );

	private:
		/**
		 *\copydoc		ProceduralTexture::CpuStep::Thread::DoStep
		 */
		virtual void DoStep();

	public:
		//! The buffer
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_backBuffer;
		//! The first function input buffer
		std::shared_ptr< ProceduralTextures::PixelBuffer > m_firstInBuffer;
		//! The second function input buffer
		std::shared_ptr< ProceduralTextures::PixelBuffer > m_secondInBuffer;

	private:
		//! The computing dimensions
		ProceduralTextures::Size m_size;
		//! The current first function
		std::shared_ptr< EffectFunctor > m_pFirstFunction;
		//! The next first function
		std::shared_ptr< EffectFunctor > m_pNextFirstFunction;
		//! The current second function
		std::shared_ptr< EffectFunctor > m_pSecondFunction;
		//! The next second function
		std::shared_ptr< EffectFunctor > m_pNextSecondFunction;
		//! The current first function input buffer
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_pFirstFunctionBufferIn;
		//! The next first function input buffer
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_pNextFirstFunctionBufferIn;
		//! The parent generator
		std::weak_ptr< CpuStep > m_pParent;
	};
	/*!
	\author		Sylvain DOREMUS
	\date		05/03/2015
	\brief		Image effects CPU step
	*/
	class CpuStep
		: public ProceduralTextures::CpuStep< Thread >
	{
	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_generator	The parent generator
		 *\param[in]	p_size		The dimensions
		 */
		CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size );
		/**
		 *\brief		Destructor
		 */
		virtual ~CpuStep();
		/**
		 *\brief		Retrieves the effect functor
		 *\param[in]	p_type	The type
		 *\return		The functor
		 */
		std::shared_ptr< EffectFunctor > GetFunctor( eEFFECT_TYPE p_type );
		/**
		 *\brief		Sets the computing image size
		 *\param[in]	p_value	The new value
		 */
		void SetSize( ProceduralTextures::Size & p_value );
		/**
		 *\brief		Sets the first function
		 *\param[in]	p_value	The new value
		 */
		void SetFirstFunction( uint32_t p_value );
		/**
		 *\brief		Sets the second function
		 *\param[in]	p_value	The new value
		 */
		void SetSecondFunction( uint32_t p_value );
		/**
		 *\brief		Sets the image
		 *\param[in]	p_value	The new value
		 */
		void SetImage( ProceduralTextures::PixelBuffer const & p_value );

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
		 *\brief		Initialises the functors array
		 */
		void DoInitialiseFunctors();
		/**
		 *\brief		Cleans the functors array up
		 */
		void DoCleanupFunctors();

	private:
		//! The functors array
		std::array< std::shared_ptr< EffectFunctor >, eEFFECT_TYPE_COUNT > m_functors;
	};
}

#endif
