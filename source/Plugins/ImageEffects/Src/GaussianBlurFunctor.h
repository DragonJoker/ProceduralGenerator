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
#ifndef ___IFX_GAUSSIAN_BLUR_FUNCTOR_H___
#define ___IFX_GAUSSIAN_BLUR_FUNCTOR_H___

#include "EffectFunctor.h"

namespace ImageEffects
{
	class GaussianBlurFunctor
		: public EffectFunctor
	{
	public:
		GaussianBlurFunctor();
		virtual ~GaussianBlurFunctor();

		virtual void operator()( ProceduralTextures::PixelBuffer const & p_bufferIn, ProceduralTextures::PixelBuffer & p_bufferOut );

		virtual void SetImage( ProceduralTextures::PixelBuffer const & p_image );

	private:
		static const int m_iGaussLvl;
		static const int m_iGaussMask[];
		static const int m_iGaussSum;
		ProceduralTextures::PixelBuffer m_temporaryBuffer;
	};
}

#endif
