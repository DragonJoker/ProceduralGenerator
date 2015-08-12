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
#ifndef ___PLN_CPU_STEP_H___
#define ___PLN_CPU_STEP_H___

#include <CpuStep.h>

namespace PerlinNoise
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		The noise types enumeration
	*/
	typedef enum
	{
		eNOISE_TYPE_TURBULENCE,
		eNOISE_TYPE_CLOUD,
		eNOISE_TYPE_MARBLE,
		eNOISE_TYPE_PARAM,
		eNOISE_TYPE_COUNT,
	}	eNOISE_TYPE;
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		The parametered noise function enumeration
	*/
	typedef enum
	{
		eNOISE_PARAM_FUNCTION_COS,
		eNOISE_PARAM_FUNCTION_SIN,
		eNOISE_PARAM_FUNCTION_TAN,
		eNOISE_PARAM_FUNCTION_COSH,
		eNOISE_PARAM_FUNCTION_SINH,
		eNOISE_PARAM_FUNCTION_TANH,
		eNOISE_PARAM_FUNCTION_ACOS,
		eNOISE_PARAM_FUNCTION_ASIN,
		eNOISE_PARAM_FUNCTION_ATAN,
		eNOISE_PARAM_FUNCTION_SQRT,
		eNOISE_PARAM_FUNCTION_LN,
		eNOISE_PARAM_FUNCTION_EXP,
		eNOISE_PARAM_FUNCTION_LOG10,
		eNOISE_PARAM_FUNCTION_COUNT,
	}	eNOISE_PARAM_FUNCTION;
	//! The noise functor
	typedef std::function< float( float ) > NoiseFunction;
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Perlin noise worker thread
	*/
	class Thread
		:	public ProceduralTextures::Thread
	{
	private:
		typedef std::function< void( int, int, int, ProceduralTextures::UbPixel & ) > TypeFunction;

	public:
		/** Constructor
		@param[in] p_parent
			The parent generator
		@param[in] p_index
			The thread index
		@param[in] p_width
			The surface width
		@param[in] p_top
			The surface portion's top
		@param[in] p_bottom
			The surface portion's bottom
		@param[in] p_height
			The surface height
		 */
		Thread( std::shared_ptr< ProceduralTextures::CpuStepBase > p_parent, size_t p_index, int p_width, int p_top, int p_bottom, int p_totalHeight );
		/** Destructor
		 */
		virtual ~Thread();
		/** Sets the noise type
		@param[in] p_value
			The new value
		 */
		void SetType( eNOISE_TYPE p_type );
		/** Sets the noise function
		@param[in] p_function
			The new function
		 */
		void SetFunction( NoiseFunction p_function );
		/** Sets the pixel buffer
		@param[in] p_pixels
			The buffer
		 */
		inline void SetBuffer( std::shared_ptr< ProceduralTextures::PixelBuffer > p_pixels )
		{
			m_pixels = p_pixels;
		}
		/** Sets the red value of the pixel
		@param[in] val
			The new value
		 */
		inline void SetRed( uint8_t val )
		{
			m_colour.r = val;
		}
		/** Sets the green value of the pixel
		@param[in] val
			The new value
		 */
		inline void SetGreen( uint8_t val )
		{
			m_colour.g = val;
		}
		/** Sets the blue value of the pixel
		@param[in] val
			The new value
		 */
		inline void SetBlue( uint8_t val )
		{
			m_colour.b = val;
		}
		/** Sets the octaves count
		@param[in] val
			The new value
		 */
		inline void SetOctavesCount( int val )
		{
			m_octavesCount = val;
		}
		/** Retrieves the octaves count
		@return
				The value
		 */
		inline int GetOctavesCount()const
		{
			return m_octavesCount;
		}
		/** Sets the persistency
		@param[in] val
			The new value
		 */
		inline void SetPersistency( float val )
		{
			m_persistency = val;
		}
		/** Retrieves the persistency
		@return
				The value
		 */
		float GetPersistency()const
		{
			return m_persistency;
		}

	private:
		/** @copydoc ProceduralTexture::CpuStep::Thread::DoStep
		 */
		virtual void DoStep();
		/** Turbulence noise function
		@param[in] val
			The new value
		 */
		void DoTurbulence( int x, int y, int z, ProceduralTextures::UbPixel & p_rPixel );
		/** Cloudes noise function
		@param[in] val
			The new value
		 */
		void DoClouds( int x, int y, int z, ProceduralTextures::UbPixel & p_rPixel );
		/** Marble noise function
		@param[in] val
			The new value
		 */
		void DoMarble( int x, int y, int z, ProceduralTextures::UbPixel & p_rPixel );
		/** Parameterised noise function
		@param[in] val
			The new value
		 */
		void DoParam( int x, int y, int z, ProceduralTextures::UbPixel & p_rPixel );
		double DoFade( double t );
		double DoLerp( double t, double a, double b );
		double DoGrad( int hash, double x, double y, double z );
		double DoNoise( double x, double y, double z );

	private:
		//! The permutations
		int m_permutations[512];
		//! The noise type
		eNOISE_TYPE m_type;
		//! The step index
		int m_step;
		//! The octaves count
		int m_octavesCount;
		//! The persistency
		float m_persistency;
		//! The pixel buffer
		std::weak_ptr< ProceduralTextures::PixelBuffer > m_pixels;
		//! The parameterised noise function
		NoiseFunction m_paramNoise;
		//! The noise function
		TypeFunction m_noise;
		//! The noise functions
		TypeFunction m_noiseFunctions[eNOISE_TYPE_COUNT];
		//! The pixel colour (image is in computed in white and m_colour )
		ProceduralTextures::UbPixel m_colour;
		//! The mutex used to protect the noise functions
		std::mutex m_mutexNoise;
	};
	/*!
	@author
		Sylvain DOREMUS
	@date
		05/03/2015
	@brief
		Perlin noise CPU step
	*/
	class CpuStep
		: public ProceduralTextures::CpuStep< Thread >
	{
	public:
		/** Constructor
		@param[in] p_generator
			The parent generator
		@param[in] p_size
			The displayed surface dimensions
		 */
		CpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size );
		/** Destructor
		 */
		virtual ~CpuStep();
		/** Sets the red value of the pixel
		@param[in] val
			The new value
		 */
		void SetRed( uint8_t val );
		/** Sets the green value of the pixel
		@param[in] val
			The new value
		 */
		void SetGreen( uint8_t val );
		/** Sets the blue value of the pixel
		@param[in] val
			The new value
		 */
		void SetBlue( uint8_t val );
		/** Sets the octaves count
		@param[in] val
			The new value
		 */
		void SetOctavesCount( int val );
		/** Retrieves the octaves count
		@return
				The value
		 */
		int GetOctavesCount()const;
		/** Sets the persistency
		@param[in] val
			The new value
		 */
		void SetPersistency( float val );
		/** Retrieves the persistency
		@return
				The value
		 */
		float GetPersistency()const;
		/** Sets the noise type function
		@param[in] val
			The new value
		 */
		void SetType( eNOISE_TYPE val );
		/** Sets the parameteised noise type function
		@param[in] val
			The new value
		 */
		void SetFunction( eNOISE_PARAM_FUNCTION val );

	private:
		/** @copydoc ProceduralTexture::CpuStep::DoInitialiseStep
		 */
		virtual void DoInitialiseStep();
		/** @copydoc ProceduralTexture::CpuStep::DoInitialise
		 */
		virtual void DoInitialise();
		/** @copydoc ProceduralTexture::CpuStep::DoCleanup
		 */
		virtual void DoCleanup();
		/** @copydoc ProceduralTexture::CpuStep::DoSwapBuffers
		 */
		virtual void DoSwapBuffers();

	public:
		//! The parameterised noise functions names
		static ProceduralTextures::String ParamNames[eNOISE_PARAM_FUNCTION_COUNT];
		//! The noise functions types names
		static ProceduralTextures::String TypeNames[eNOISE_TYPE_COUNT];

	private:
		//! The parameterised noise functions
		NoiseFunction m_functions[eNOISE_PARAM_FUNCTION_COUNT];
	};
}

#endif
