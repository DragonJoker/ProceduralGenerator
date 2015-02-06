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
#ifndef ___IFX_EFFECT_FUNCTOR_H___
#define ___IFX_EFFECT_FUNCTOR_H___

#include <GeneratorPrerequisites.h>
#include <Buffer.h>

namespace ProceduralTextures
{
	class ImageEffects;

	typedef enum
	{
		eEFFECT_TYPE_NONE,
		eEFFECT_TYPE_GAUSSIAN_BLUR,
		eEFFECT_TYPE_SHARPEN,
		eEFFECT_TYPE_GAUSSIAN_BLUR_SHARPEN,
		eEFFECT_TYPE_LINEAR_NR,
		eEFFECT_TYPE_ANISOTROPIC_NR,
		eEFFECT_TYPE_WAVE,
		eEFFECT_TYPE_GRAYSCALE,
		eEFFECT_TYPE_SEPIA,
		eEFFECT_TYPE_IMPLOSION,
		eEFFECT_TYPE_EXPLOSION,
		eEFFECT_TYPE_WATERSHED,
		eEFFECT_TYPE_REC_GAUSSIAN_BLUR,
		eEFFECT_TYPE_REC_SHARPEN,
		eEFFECT_TYPE_REC_GAUSSIAN_BLUR_SHARPEN,
		eEFFECT_TYPE_REC_LINEAR_NR,
		eEFFECT_TYPE_REC_ANISOTROPIC_NR,
		eEFFECT_TYPE_COUNT
	}	eEFFECT_TYPE;

	class EffectFunctor
	{
	public:
		EffectFunctor( eEFFECT_TYPE	p_eEffectType, const wxString & m_strName, bool p_bAllowsSecondEffect, bool p_bCanBeSecondEffect );
		virtual ~EffectFunctor();

		virtual void operator()( const PixelBuffer & p_bufferIn, PixelBuffer & p_bufferOut ) = 0;

		virtual void SetImage( const wxImage & p_image );

	public:
		inline bool AllowsSecondEffect()const
		{
			return m_bAllowsSecondEffect;
		}
		inline bool CanBeSecondEffect()const
		{
			return m_bCanBeSecondEffect;
		}
		inline eEFFECT_TYPE GetEffectType()const
		{
			return m_eEffectType;
		}
		inline const wxString &	GetName()const
		{
			return m_strName;
		}
		inline void SetImgWidth( int p_iVal )
		{
			m_iImgWidth = p_iVal;
		}
		inline void SetImgHeight( int p_iVal )
		{
			m_iImgHeight = p_iVal;
		}
		inline void SetGenerator( ImageEffects * p_pGenerator )
		{
			m_pGenerator = p_pGenerator;
		}

	protected:
		eEFFECT_TYPE m_eEffectType;
		bool m_bAllowsSecondEffect;
		bool m_bCanBeSecondEffect;
		int m_iImgWidth;
		int m_iImgHeight;
		wxString m_strName;
		ImageEffects * m_pGenerator;
	};
}

#endif
