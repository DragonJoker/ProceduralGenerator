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
#ifndef ___SWM_SHADER_WEBCAM_H___
#define ___SWM_SHADER_WEBCAM_H___

#include <ConfigPanel.h>

#include "SwmEffect.h"

namespace ProceduralTextures
{
	class ShaderWebcam
		:	public ProceduralGenerator
	{
	private:
		typedef enum
		{
			eID_SEPTYPE			= 53,
			eID_SEPOFFSET		= 54,
			eID_RESETTIME		= 55,
			eID_SHADERS			= 56,
			eID_REMOVE			= 57,
			eID_VERTEXFILE		= 58,
			eID_FRAGMENTFILE	= 59,
			eID_COMPILE			= 60,
			eID_COMPILERLOG		= 61,
		}	eID;

		class Thread
			:	public ProceduralGenerator::Thread
		{
		public:
			Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour );
			virtual ~Thread();

			void Initialise( cv::VideoCapture * p_pCapture, PixelBuffer * p_pBuffer );
			virtual void Step();

		private:
			cv::VideoCapture * m_pCapture;
			PixelBuffer * m_pBuffer;
		};

	private:
		typedef std::vector< Effect * >	EffectPtrArray;

	public:
		ShaderWebcam( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame );
		virtual ~ShaderWebcam();

		void ResetTime();

		bool EffectAdd();
		bool EffectRemove( size_t p_uiIndex );
		void EffectSetVertexFile( size_t p_uiIndex, const wxString & p_strPath );
		void EffectSetFragmentFile( size_t p_uiIndex, const wxString & p_strPath );
		void EffectSetImagePath( size_t p_uiIndex, size_t p_iImage, const wxString & p_strImagePath );
		void EffectCompile( size_t p_uiIndex );
		wxString EffectGetCompilerLog( size_t p_uiIndex, eSHADER_OBJECT_TYPE p_eType );
		wxString EffectGetLinkerLog( size_t p_uiIndex );
		bool EffectActivate( size_t p_uiIndex );
		void EffectDeactivate( size_t p_uiIndex );

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
		void OnSelectShader( wxCommandEvent & p_event );
		void OnRemove( wxCommandEvent & p_event );
		void OnShaderCompile( wxCommandEvent & p_event );
		void OnCompilerLog( wxCommandEvent & p_event );
		void OnVertexShaderPath( wxCommandEvent & p_event );
		void OnFragmentShaderPath( wxCommandEvent & p_event );

	private:
		cv::VideoCapture * m_pCapture;
		wxImage m_image;
		EffectPtrArray m_arrayEffects;
		eSEPARATION m_eSeparationType;
		int m_iSeparationOffset;
		GlFrameBuffer m_frameBuffer1;
		GlFrameBuffer m_frameBuffer2;
		GlTexture m_texture1;
		GlTexture m_texture2;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticSeparator;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboSeparator;
		SpecificControlParameters< eCONTROL_TYPE_SLIDER > m_specSliderOffset;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonReset;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticShaders;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboShaders;
		SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > m_specButtonVertexFile;
		SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > m_specButtonFragmentFile;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonCompile;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonCompilerLog;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonRemove;
		Effect * m_pSelectedEffect;
	};
}

#endif
