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
#ifndef ___TFX_TEXT_EFFECTS_H___
#define ___TFX_TEXT_EFFECTS_H___

#include <ConfigPanel.h>

namespace ProceduralTextures
{
	typedef enum
	{
		eBG_TYPE_COLOUR,
		eBG_TYPE_TEXTURE,
	}	eBG_TYPE;

	class TextEffects
		:	public ProceduralGenerator
	{
	private:
		typedef enum
		{
			eID_TEXTBGTYPE	= 51,
			eID_TEXTBG_COLOUR,
			eID_TEXTBG_IMAGE,
			eID_FONT,
			eID_TEXT,
			eID_BACKGROUND,
		}	eIDs;

		class Glyph
		{
		private:
			wxPoint m_ptPosition;
			wxSize m_size;
			std::vector< uint8_t > m_bitmap;

		public:
			Glyph();
			Glyph( const Glyph & p_glyph );
			Glyph & operator =( const Glyph & p_glyph );
			~Glyph();

			inline const wxSize & GetSize()const
			{
				return m_size;
			}
			inline const wxPoint & GetPosition()const
			{
				return m_ptPosition;
			}
			inline const std::vector< uint8_t > & GetBitmap()const
			{
				return m_bitmap;
			}
			inline void	SetBitmap( const std::vector< uint8_t > & val )
			{
				m_bitmap = val;
			}
			inline void SetPosition( const wxPoint & val )
			{
				m_ptPosition = val;
			}
			inline void SetSize( const wxSize & val )
			{
				m_size = val;
			}
		};

	public:
		TextEffects( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame );
		virtual ~TextEffects();

		void SetImagePath( const wxString & p_strImagePath );
		void SetTxtColour( const wxColour & p_clrColour );
		void SetTxtImagePath( const wxString & p_strImagePath );
		void SetTextBackgroundType( eBG_TYPE val );

		inline void SetFont( const wxString & p_strFont )
		{
			m_strFontFile = p_strFont;
			m_bFontModified = true;
		}
		inline void	SetText( const wxString & p_strText )
		{
			m_strText = p_strText;
			m_bTextModified = true;
		}

	private:
		void _loadFtFont();
		void _cleanupArrays();
		void _updateTxtImage();

		virtual void DoResize( const wxSize & p_size );
		virtual void DoGlInitialise();
		virtual void DoGlPreRender();
		virtual void DoGlRender( bool & p_bChanged );
		virtual void DoGlPostRender();
		virtual void DoGlCleanup();
		virtual void DoGeneratePanel();

		void OnTxtBackgroundType( wxCommandEvent & p_event );
		void OnChar( wxCommandEvent & p_event );
		void OnBackground( wxCommandEvent & p_event );
		void OnTxtBackgroundColour( wxCommandEvent & p_event );
		void OnTxtBackgroundImage( wxCommandEvent & p_event );
		void OnFont( wxCommandEvent & p_event );
		void OnSize( wxSizeEvent & p_event );

	private:
		wxImage m_image;
		int m_iImgWidth;
		int m_iImgHeight;
		wxString m_strText;
		wxString m_strFontFile;
		bool m_bFontModified;
		bool m_bTextModified;
		std::vector< Glyph > m_arrayChars;
		int m_iMaxHeight;
		int m_iMaxTop;
		GlTexture m_textureImg;
		GlTexture m_textureTmpImg;
		GlTexture m_textureTxtImg;
		GlTexture m_textureTxt;
		GlTexture m_textureTmpTxt;
		GlFrameBuffer m_frameBufferImg;
		GlFrameBuffer m_frameBufferTxt;
		bool m_bNewTexture;
		bool m_bNewTxtTexture;
		UbPixel m_txtColour;
		wxImage m_txtImage;
		GlTexture::BlendMode m_blendRgbTxt;
		GlTexture::BlendMode m_blendAlphaTxt;
		GlTexture::BlendMode m_blendRgbImg;
		GlTexture::BlendMode m_blendAlphaImg;
		eBG_TYPE m_eTextBackgroundType;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticTextBackground;
		SpecificControlParameters< eCONTROL_TYPE_COMBO > m_specComboTextBackground;
		SpecificControlParameters< eCONTROL_TYPE_BUTTON > m_specButtonTextBgColour;
		SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > m_specButtonTextBgImage;
		SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > m_specButtonBackground;
		SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON > m_specButtonFont;
		SpecificControlParameters< eCONTROL_TYPE_STATIC > m_specStaticCaption;
		SpecificControlParameters< eCONTROL_TYPE_EDIT > m_specEditCaption;
	};
}

#endif
