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
#ifndef ___PLN_PERLIN_NOISE_H___
#define ___PLN_PERLIN_NOISE_H___

#include <Generator.h>

#include "PlnCpuStep.h"
#include "PlnGpuStep.h"

namespace PerlinNoise
{
	/*!
	\author		Sylvain DOREMUS
	\date		23/05/2012
	\brief		Perlin noise generator
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
			eID_ANY			= -1,
			eID_REDVALUE	= 50,
			eID_GREENVALUE	= 51,
			eID_BLUEVALUE	= 52,
			eID_OCTAVES		= 53,
			eID_FREQUENCY	= 54,
			eID_TYPE		= 55,
			eID_FUNCTION	= 56,
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
		 *\brief		Sets the red value of the pixel
		 *\param[in]	val	The new value
		 */
		void DoSetRed( uint8_t val );
		/**
		 *\brief		Sets the green value of the pixel
		 *\param[in]	val	The new value
		 */
		void DoSetGreen( uint8_t val );
		/**
		 *\brief		Sets the blue value of the pixel
		 *\param[in]	val	The new value
		 */
		void DoSetBlue( uint8_t val );
		/**
		 *\brief		Sets the octaves count
		 *\param[in]	val	The new value
		 */
		void DoSetOctaves( int p_value );
		/**
		 *\brief		Sets the persistency
		 *\param[in]	val	The new value
		 */
		void DoSetFrequency( int p_value );
		/**
		 *\brief		Sets the noise type function
		 *\param[in]	val	The new value
		 */
		void DoSetType( uint32_t p_value );
		/**
		 *\brief		Sets the parameteised noise type function
		 *\param[in]	val	The new value
		 */
		void DoSetFunction( uint32_t p_value );

	private:
		//! The static control holding the octaves count
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticOctaves;
		//! The static control holding the frequency
		std::shared_ptr< ProceduralTextures::StaticCtrl > m_staticFrequency;
		//! The combobox control holding the functions names
		std::shared_ptr< ProceduralTextures::ComboBoxCtrl > m_comboFunction;
	};
}

#endif
