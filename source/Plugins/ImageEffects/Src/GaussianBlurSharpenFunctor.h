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
#ifndef ___IFX_GAUSSIAN_BLUR_SHARPEN_FUNCTOR_H___
#define ___IFX_GAUSSIAN_BLUR_SHARPEN_FUNCTOR_H___

#include "EffectFunctor.h"

namespace ImageEffects
{
	class GaussianBlurSharpenFunctor
		: public EffectFunctor
	{
	private:
		static const int m_iGaussLvl;
		static const int m_iGaussMask[];
		static const int m_iGaussSum;
		static const double m_dSharpenZ;
		static const double m_dSharpenMask[];
		ProceduralTextures::PixelBuffer m_temporaryBuffer1;
		ProceduralTextures::PixelBuffer m_temporaryBuffer2;

	public:
		GaussianBlurSharpenFunctor();
		virtual ~GaussianBlurSharpenFunctor();

		virtual void operator()( ProceduralTextures::PixelBuffer const & p_bufferIn, ProceduralTextures::PixelBuffer & p_bufferOut );

		virtual void SetImage( ProceduralTextures::PixelBuffer const & p_image );
	};
}

#endif
