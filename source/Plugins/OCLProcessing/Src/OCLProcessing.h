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
#ifndef ___OCL_OCL_PROCESSING_H___
#define ___OCL_OCL_PROCESSING_H___

#include <ConfigPanel.h>

#include "OclEffect.h"

#include <vector>
#include <map>
#include <string>

namespace ProceduralTextures
{
	class OCLProcessing
		:	public ProceduralGenerator
	{
	private:
		typedef std::vector< Effect * >	EffectPtrArray;

		typedef enum
		{
			eID_IMAGEPATH		= 52,
			eID_SEPTYPE			= 53,
			eID_SEPOFFSET		= 54,
			eID_RESETTIME		= 55,
			eID_PROGRAMS		= 56,
			eID_REMOVE			= 57,
			eID_FILE			= 58,
			eID_KERNEL			= 59,
			eID_COMPILERLOG		= 60,
		}	eID;

	public:
		OCLProcessing( int p_width, int p_height, int iWndId, wxFrame * p_pFrame );
		virtual ~OCLProcessing();

		void SetImagePath( const wxString & p_strImagePath );
		void ResetTime();

		bool EffectAdd();
		bool EffectRemove( size_t p_uiIndex );
		void EffectLoadFile( size_t p_uiIndex, const wxString & p_strPath );
		void EffectLoadKernel( size_t p_uiIndex, const wxString & p_strName );
		void EffectSetImagePath( size_t p_uiIndex, size_t p_iImage, const wxString & p_strImagePath );
		wxString EffectGetCompilerLog( size_t p_uiIndex );
		bool EffectApply( size_t p_uiIndex );

		inline void SetSeparationOffset( int p_iOffset )
		{
			m_iSeparationOffset = p_iOffset;
		}
		inline void SetSeparationType( eSEPARATION p_eType )
		{
			m_eSeparationType = p_eType;
		}

	private:
		virtual void DoResize( const wxSize & p_size );
		virtual void DoGlInitialise();
		virtual void DoGlPreRender();
		virtual void DoGlRender( bool & p_bChanged );
		virtual void DoGlPostRender();
		virtual void DoGlCleanup();
		virtual void DoGeneratePanel();

		void OnSepType( wxCommandEvent & p_event );
		void OnSepOffset( wxCommandEvent & p_event );
		void OnResetTime( wxCommandEvent & p_event );
		void OnSelectProgram( wxCommandEvent & p_event );
		void OnRemove( wxCommandEvent & p_event );
		void OnCompilerLog( wxCommandEvent & p_event );
		void OnImagePath( wxCommandEvent & p_event );
		void OnFilePath( wxCommandEvent & p_event );
		void OnSelectKernel( wxCommandEvent & p_event );

	private:
		wxImage m_image;
		eSEPARATION m_eSeparationType;
		int m_iSeparationOffset;
		GlTexture m_outputTexture;
		PixelBuffer m_inputBuffer;
		PixelBuffer m_outputBuffer;
		EffectPtrArray m_arrayEffects;
		cl::Platform m_clPlatform;
		cl::Device m_clDevice;
		cl::CommandQueue m_clQueue;
		cl::Context m_clContext;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonImage;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticSeparator;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboSeparator;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSliderOffset;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonReset;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticPrograms;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboPrograms;
		SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > m_specButtonFile;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticKernel;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboKernels;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonCompilerLog;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonRemove;
		Effect * m_pSelectedEffect;
	};
}

#endif
