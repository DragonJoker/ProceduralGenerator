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
#ifndef ___PLN_PERLIN_NOISE_H___
#define ___PLN_PERLIN_NOISE_H___

#include <ConfigPanel.h>

namespace ProceduralTextures
{
	class PerlinNoise
		:	public ProceduralGenerator
	{
	public:
		typedef enum
		{
			eNOISE_TYPE_TURBULENCE,
			eNOISE_TYPE_CLOUD,
			eNOISE_TYPE_MARBLE,
			eNOISE_TYPE_PARAM,
			eNOISE_TYPE_COUNT,
		}	eNOISE_TYPE;

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

		wxString m_arrayFuncNames[eNOISE_PARAM_FUNCTION_COUNT];
		wxString m_arrayTypeNames[eNOISE_TYPE_COUNT];

	private:
		typedef float( * NoiseFunction )( float );

		class Thread
			:	public ProceduralGenerator::Thread
		{
		private:
			typedef void ( Thread::* TypeFunction )( int x, int y, int z, UbPixel & p_rPixel );

		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour );
			virtual ~Thread();

			virtual void Step();

			void SetType( eNOISE_TYPE p_eType );
			void SetFunction( NoiseFunction p_pfnFunction );

			inline void SetBuffer( PixelBuffer * p_pixels )
			{
				m_pixels = p_pixels;
			}

		private:
			void _turbulence( int x, int y, int z, UbPixel & p_rPixel );
			void _clouds( int x, int y, int z, UbPixel & p_rPixel );
			void _marble( int x, int y, int z, UbPixel & p_rPixel );
			void _param( int x, int y, int z, UbPixel & p_rPixel );
			double _fade( double t );
			double _lerp( double t, double a, double b );
			double _grad( int hash, double x, double y, double z );
			double _noise( double x, double y, double z );
			void _perlin();

		public:
			int m_iPermutations[512];
			eNOISE_TYPE m_eType;
			int m_iStep;
			int m_iNumberOfOctaves;
			float m_fPersistency;
			PixelBuffer * m_pixels;
			NoiseFunction m_pfnNoise;
			TypeFunction m_pfnType;
			TypeFunction m_pfnArrayTypes[eNOISE_TYPE_COUNT];
		};

	public:
		PerlinNoise( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame, int p_iNumberOfOctaves = 2, float p_fPersistency = 0.05f, eNOISE_TYPE p_eType = eNOISE_TYPE_TURBULENCE );
		virtual ~PerlinNoise();


		void SetNumberOfOctaves( int val );
		void SetPersistency( float val );
		void SetType( eNOISE_TYPE p_eType );
		void SetFunction( eNOISE_PARAM_FUNCTION p_eFunction );

		int GetNumberOfOctaves()const
		{
			return m_iNumberOfOctaves;
		}
		float GetPersistency()const
		{
			return m_fPersistency;
		}

	private:
		virtual void DoGlInitialise() {}
		virtual void DoGlPreRender() {}
		virtual void DoGlRender( bool & WXUNUSED( p_bChanged ) ) {}
		virtual void DoGlPostRender() {}
		virtual void DoGlCleanup() {}
		virtual void DoGeneratePanel();

		typedef enum
		{
			eID_REDVALUE	= 50,
			eID_GREENVALUE	= 51,
			eID_BLUEVALUE	= 52,
			eID_OCTAVES		= 53,
			eID_FREQUENCY	= 54,
			eID_TYPE		= 55,
			eID_FUNCTION	= 56,
		}	eID;

		void OnSliderRed( wxCommandEvent & p_event );
		void OnSliderGreen( wxCommandEvent & p_event );
		void OnSliderBlue( wxCommandEvent & p_event );
		void OnOctaves( wxCommandEvent & p_event );
		void OnFrequency( wxCommandEvent & p_event );
		void OnType( wxCommandEvent & p_event );
		void OnFunction( wxCommandEvent & p_event );

	private:
		int m_iNumberOfOctaves;
		float m_fPersistency;
		int m_iPermutations[512];
		int m_iStep;
		eNOISE_TYPE m_eType;
		NoiseFunction m_functions[eNOISE_PARAM_FUNCTION_COUNT];
		bool m_bModifying;

		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatR;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideR;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatG;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideG;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatB;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideB;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatOctavesTitle;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatOctaves;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideOctaves;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatFrequencyTitle;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatFrequency;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSlideFrequency;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStatType;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboType;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboFunction;
	};
}

#endif
