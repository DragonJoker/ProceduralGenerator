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
#ifndef ___IFX_WAVE_FUNCTOR_H___
#define ___IFX_WAVE_FUNCTOR_H___

#include "EffectFunctor.h"

namespace ImageEffects
{
	class WaveFunctor
		: public EffectFunctor
	{
	public:
		WaveFunctor();
		virtual ~WaveFunctor();

		virtual void operator()( ProceduralTextures::PixelBuffer const & p_bufferIn, ProceduralTextures::PixelBuffer & p_bufferOut );

	private:
		static double m_dDegToRad;
		static double m_dPiMult2;
		static double m_dCos[360];
		static double m_dSin[360];
		double m_dTimeIndexX;
		double m_dTimeIndexY;
		int m_iDecalX;
		int m_iDecalY;
		double m_dTimeScaleX;
		double m_dTimeScaleY;
		double m_dScaleX;
		double m_dScaleY;
		double m_dTranslateX;
		double m_dTranslateY;
		double m_dCurTranslateX;
		double m_dCurTranslateY;
		double m_dNbPixelsX;
		double m_dNbPixelsY;
	};
}

#endif
