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
#ifndef ___IFX_IMAGE_EFFECTS_H___
#define ___IFX_IMAGE_EFFECTS_H___

#include "EffectFunctor.h"

#include <ConfigPanel.h>
#include <Plugin.h>

namespace ProceduralTextures
{
	class ImageEffects : public ProceduralGenerator
	{
	public:
		typedef enum
		{
			eID_FIRSTFUNCTION	= 50,
			eID_SECONDFUNCTION	= 51,
			eID_IMAGEPATH		= 52,
		}	eID;

		class Thread
			: public ProceduralGenerator::Thread
		{
		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour );
			virtual ~Thread();

			virtual void Step();

			void SetImage( const wxImage & p_image );
			void SetFirstFunction( eEFFECT_TYPE p_eType );
			void SetSecondFunction( eEFFECT_TYPE p_eType );

			static void InitialiseFunctors( ImageEffects * p_pParent );
			static void CleanupFunctors();

		private:
			int m_iImgWidth;
			int m_iImgHeight;
			EffectFunctor * m_pFirstFunction;
			EffectFunctor * m_pNextFirstFunction;
			EffectFunctor * m_pSecondFunction;
			EffectFunctor * m_pNextSecondFunction;
			PixelBuffer * m_pFirstFunctionBufferIn;
			PixelBuffer * m_pNextFirstFunctionBufferIn;

		public:
			PixelBuffer * m_backBuffer;
			PixelBuffer m_firstInBuffer;
			PixelBuffer m_secondInBuffer;
			static EffectFunctor * m_pFunctors[eEFFECT_TYPE_COUNT];
		};

	public:
		ImageEffects( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame );
		virtual ~ImageEffects();


		void SetFirstFunction( eEFFECT_TYPE p_eType );
		void SetSecondFunction( eEFFECT_TYPE p_eType );
		void SetImagePath( const wxString & p_strImagePath );

		static EffectFunctor * GetFunctor( eEFFECT_TYPE p_eType );

	private:
		virtual void DoGlInitialise() {}
		virtual void DoGlPreRender() {}
		virtual void DoGlRender( bool & WXUNUSED( p_bChanged ) ) {}
		virtual void DoGlPostRender() {}
		virtual void DoGlCleanup() {}
		virtual void DoGeneratePanel();

		void OnSize( wxSizeEvent & p_event );
		void OnFirstFunction( wxCommandEvent & p_event );
		void OnSecondFunction( wxCommandEvent & p_event );
		void OnImagePath( wxCommandEvent & p_event );

	private:
		wxImage m_image;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticFirstFunction;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboFirstFunction;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticSecondFunction;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboSecondFunction;
		SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > m_specButtonImage;
	};
}

#endif
