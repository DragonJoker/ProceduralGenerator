/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___IFX_ANISOTROPIC_NOISE_REDUCTION_FUNCTOR_H___
#define ___IFX_ANISOTROPIC_NOISE_REDUCTION_FUNCTOR_H___

#include "EffectFunctor.h"

namespace ProceduralTextures
{
	class AnisotropicNoiseReductionFunctor
		: public EffectFunctor
	{
	private:
		static const int m_iAdThreshold;
		static const int m_iAdMask[];
		static const int m_iAdSum;

	public:
		AnisotropicNoiseReductionFunctor();
		virtual ~AnisotropicNoiseReductionFunctor();

		virtual void operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut );
	};
}

#endif
