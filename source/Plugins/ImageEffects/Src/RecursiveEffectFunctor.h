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
#ifndef ___IFX_RECURSIVE_EFFECT_FUNCTOR_H___
#define ___IFX_RECURSIVE_EFFECT_FUNCTOR_H___

#include "EffectFunctor.h"

namespace ImageEffects
{
	template< typename Functor >
	class RecursiveEffectFunctor
		:	public EffectFunctor
	{
	public:
		RecursiveEffectFunctor( eEFFECT_TYPE p_eEffectType, ProceduralTextures::String const & p_strName, bool p_bAllowsSecondEffect, bool p_bCanBeSecondEffect )
			:	EffectFunctor( p_eEffectType, p_strName, p_bAllowsSecondEffect, p_bCanBeSecondEffect )
		{
		}

		virtual ~RecursiveEffectFunctor()
		{
		}

		virtual void operator()( ProceduralTextures::PixelBuffer const & p_bufferIn, ProceduralTextures::PixelBuffer & p_bufferOut )
		{
			m_onceFunctor( p_bufferIn, p_bufferOut );
		}

		virtual void SetImage( ProceduralTextures::PixelBuffer const & p_image )
		{
			EffectFunctor::SetImage( p_image );
			m_onceFunctor.SetImage( p_image );
		}

	private:
		Functor m_onceFunctor;

	};
}

#endif
