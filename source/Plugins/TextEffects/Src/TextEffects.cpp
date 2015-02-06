#include "TextEffects.h"

#include <cmath>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

namespace ProceduralTextures
{
	TextEffects::Glyph::Glyph()
	{
	}

	TextEffects::Glyph::Glyph( const Glyph & p_glyph )
		:	m_ptPosition( p_glyph.m_ptPosition )
		,	m_size( p_glyph.m_size )
		,	m_bitmap( p_glyph.m_bitmap )
	{
	}

	TextEffects::Glyph & TextEffects::Glyph::operator =( const Glyph & p_glyph )
	{
		m_ptPosition = p_glyph.m_ptPosition;
		m_size = p_glyph.m_size;
		m_bitmap = p_glyph.m_bitmap;
		return * this;
	}

	TextEffects::Glyph::~Glyph()
	{
		m_bitmap.clear();
	}

	//*************************************************************************************************

	wxString g_bgChoices[] =
	{
		wxT( "Colour" )
		,	wxT( "Texture" )
	};

	TextEffects::TextEffects( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame )
		:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame, true, 20 )
		,	m_bFontModified( false )
		,	m_bTextModified( false )
		,	m_bNewTexture( false )
		,	m_bNewTxtTexture( false )
		,	m_specStaticTextBackground( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Text Background" ) ) )
		,	m_specComboTextBackground( SpecificControlParameters< eCONTROL_TYPE_COMBO >( g_bgChoices[0], g_bgChoices ) )
		,	m_specButtonTextBgColour( SpecificControlParameters< eCONTROL_TYPE_BUTTON >( _( "Colour" ) ) )
		,	m_specButtonTextBgImage( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Image" ) ) )
		,	m_specButtonBackground( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Background" ) ) )
		,	m_specButtonFont( SpecificControlParameters< eCONTROL_TYPE_FILE_BUTTON >( _( "Text Font" ) ) )
		,	m_specStaticCaption( SpecificControlParameters< eCONTROL_TYPE_STATIC >( _( "Caption" ) ) )
		,	m_specEditCaption( SpecificControlParameters< eCONTROL_TYPE_EDIT >( wxT( "" ) ) )
		,	m_frameBufferImg( &m_openGl )
		,	m_frameBufferTxt( &m_openGl )
		,	m_textureImg( &m_openGl )
		,	m_textureTmpImg( &m_openGl )
		,	m_textureTxt( &m_openGl )
		,	m_textureTmpTxt( &m_openGl )
		,	m_textureTxtImg( &m_openGl )
	{
		m_blendRgbTxt.function = GL_MODULATE;
		m_blendRgbTxt.source1 = GL_TEXTURE;
		m_blendRgbTxt.operand1 = GL_SRC_ALPHA;
		m_blendRgbTxt.source2 = GL_CONSTANT;
		m_blendRgbTxt.operand2 = GL_SRC_COLOR;
		m_blendRgbTxt.source3 = GL_NONE;
		m_blendRgbTxt.operand3 = GL_NONE;
		m_blendAlphaTxt.function = GL_REPLACE;
		m_blendAlphaTxt.source1 = GL_TEXTURE;
		m_blendAlphaTxt.operand1 = GL_SRC_ALPHA;
		m_blendAlphaTxt.source2 = GL_NONE;
		m_blendAlphaTxt.operand2 = GL_NONE;
		m_blendAlphaTxt.source3 = GL_NONE;
		m_blendAlphaTxt.operand3 = GL_NONE;
		m_blendRgbImg.function = GL_MODULATE;
		m_blendRgbImg.source1 = GL_PREVIOUS;
		m_blendRgbImg.operand1 = GL_ONE_MINUS_SRC_ALPHA;
		m_blendRgbImg.source2 = GL_TEXTURE;
		m_blendRgbImg.operand2 = GL_SRC_COLOR;
		m_blendRgbImg.source3 = GL_NONE;
		m_blendRgbImg.operand3 = GL_NONE;
		m_blendAlphaImg.function = GL_REPLACE;
		m_blendAlphaImg.source1 = GL_PREVIOUS;
		m_blendAlphaImg.operand1 = GL_ONE_MINUS_SRC_ALPHA;
		m_blendAlphaImg.source2 = GL_NONE;
		m_blendAlphaImg.operand2 = GL_NONE;
		m_blendAlphaImg.source3 = GL_NONE;
		m_blendAlphaImg.operand3 = GL_NONE;
		wxSize l_size( p_width, p_height );
		m_textureImg.Resize( l_size );
		m_textureTmpImg.Resize( l_size );
		m_textureTxt.Resize( l_size );
		m_textureTmpTxt.Resize( l_size );
		m_frameBufferImg.Resize( l_size );
		m_frameBufferTxt.Resize( l_size );
		m_frameBufferImg.AttachTexture( GL_COLOR_ATTACHMENT0, & m_textureTmpImg );
		m_frameBufferTxt.AttachTexture( GL_COLOR_ATTACHMENT0, & m_textureTmpTxt );
		wxSystemOptions::SetOption( wxT( "msw.remap" ), 0 );
		wxInitAllImageHandlers();
		DoGeneratePanel();
		m_initialised = true;
		Connect( eEVENT_TYPE_TEXT_UPDATED, wxEVT_NULL, wxCommandEventHandler( TextEffects::OnChar ), NULL, this );
	}

	TextEffects::~TextEffects()
	{
		m_initialised = false;
		_cleanupArrays();
		wxImage::CleanUpHandlers();
		DoCleanup();
	}

	void TextEffects::SetImagePath( const wxString & p_strImagePath )
	{
		m_initialised = false;
		wxSize l_size = DoLoadImage( p_strImagePath, m_image, m_backBuffer );
		Resize( l_size );
		m_bNewTexture = true;
		m_initialised = true;
		SetRendered( true );
	}

	void TextEffects::SetTxtColour( const wxColour & p_clrColour )
	{
		m_txtColour.Set( p_clrColour.Red(), p_clrColour.Green(), p_clrColour.Blue(), uint8_t( 255 ) );
		m_textureTxt.SetColour( m_txtColour );
	}

	void TextEffects::SetTxtImagePath( const wxString & p_strImagePath )
	{
		if ( m_txtImage.LoadFile( p_strImagePath ) )
		{
			_updateTxtImage();
		}
	}

	void TextEffects::SetTextBackgroundType( eBG_TYPE val )
	{
		m_eTextBackgroundType = val;

		if ( m_eTextBackgroundType == eBG_TYPE_COLOUR )
		{
			m_blendRgbTxt.source2 = GL_CONSTANT;
		}
		else
		{
			m_blendRgbTxt.source2 = GL_PREVIOUS;
		}
	}

	void TextEffects::_loadFtFont()
	{
		if ( ! m_strFontFile.empty() )
		{
			FT_Library l_ftLibrary;
			int l_iFontHeight = 64;
			FT_ULong l_ulMin = 0;
			FT_ULong l_ulMax = 256 * 256;
			m_arrayChars.resize( size_t( l_ulMax ) + 1, Glyph() );
			m_iMaxHeight = 0;
			m_iMaxTop = 0;

			if ( FT_Init_FreeType( & l_ftLibrary ) == 0 )
			{
				FT_Face l_ftFace;

				if ( FT_New_Face( l_ftLibrary, m_strFontFile.char_str(), 0, & l_ftFace ) == 0 )
				{
					if ( FT_Select_Charmap( l_ftFace, FT_ENCODING_UNICODE ) == 0 && FT_Set_Char_Size( l_ftFace, l_iFontHeight << 6, l_iFontHeight << 6, 96, 96 ) == 0 )
					{
						wxSize l_size;
						wxPoint l_ptPosition;
						std::vector< uint8_t > l_bitmap;
						size_t l_uiIndex;
						FT_Glyph l_ftGlyph;
						size_t l_uiSize;

						for ( FT_ULong c = l_ulMin ; c < l_ulMax ; c++ )
						{
							if ( FT_Load_Glyph( l_ftFace, FT_Get_Char_Index( l_ftFace, c ), FT_LOAD_DEFAULT ) == 0 )
							{
								if ( FT_Get_Glyph( l_ftFace->glyph, & l_ftGlyph ) == 0 )
								{
									FT_Glyph_To_Bitmap( & l_ftGlyph, ft_render_mode_normal, 0, 1 );
									FT_BitmapGlyph l_ftBmpGlyph = ( FT_BitmapGlyph )l_ftGlyph;
									FT_Bitmap & l_ftBitmap = l_ftBmpGlyph->bitmap;
									m_iMaxTop = std::max< int >( m_iMaxTop, l_ftBmpGlyph->top );
									l_uiSize = std::abs( l_ftBitmap.pitch ) * l_ftBitmap.rows;
									l_size = wxSize( std::abs( l_ftBitmap.pitch ), l_ftBitmap.rows );
									l_ptPosition = wxPoint( l_ftBmpGlyph->left, l_ftBmpGlyph->top );
									l_bitmap.resize( l_uiSize, 0 );
									l_uiIndex = 0;

									if ( c == L' ' )
									{
										l_size.x = l_iFontHeight / 2;
									}

									for ( int i = 0 ; i < l_ftBitmap.rows ; i++ )
									{
										for ( int j = 0 ; j < std::abs( l_ftBitmap.pitch ) ; j++ )
										{
											l_bitmap[l_uiIndex++] = l_ftBitmap.buffer[l_ftBitmap.pitch * i + j];
										}
									}

									m_arrayChars[c].SetSize( l_size );
									m_arrayChars[c].SetPosition( l_ptPosition );
									m_arrayChars[c].SetBitmap( l_bitmap );
								}
							}
						}

						for ( FT_ULong c = l_ulMin ; c < l_ulMax ; c++ )
						{
							l_ptPosition = m_arrayChars[c].GetPosition();
							l_size = m_arrayChars[c].GetSize();
							l_ptPosition = wxPoint( l_ptPosition.x, m_iMaxTop - l_ptPosition.y );
							m_arrayChars[c].SetPosition( l_ptPosition );
							m_iMaxHeight = std::max< int >( m_iMaxHeight, l_ptPosition.y + l_size.y );
						}
					}

					FT_Done_Face( l_ftFace );
				}

				FT_Done_FreeType( l_ftLibrary );
			}
		}
	}

	void TextEffects::_cleanupArrays()
	{
		m_arrayChars.clear();
	}

	void TextEffects::_updateTxtImage()
	{
		if ( m_txtImage.IsOk() )
		{
			wxSize l_size( m_sizeImage.x, m_sizeImage.y );
			m_txtImage.Rescale( m_sizeImage.x, m_sizeImage.y, wxIMAGE_QUALITY_HIGH );
			PixelBuffer & l_buffer = m_textureTxtImg.GetBuffer();
			l_buffer.init( Size( l_size.x, l_size.y ) );

			if ( m_txtImage.HasAlpha() )
			{
				l_buffer.set< PG_UINT8_RGBA >( m_txtImage.GetData(), m_txtImage.GetAlpha() );
			}
			else
			{
				l_buffer.set< PG_UINT8_RGB >( m_txtImage.GetData(), NULL );
			}

			m_bNewTxtTexture = true;
		}
	}

	void TextEffects::DoResize( const wxSize & p_size )
	{
		m_textureImg.Resize( p_size );
		m_textureTmpImg.Resize( p_size );
		m_textureTxt.Resize( p_size );
		m_textureTmpTxt.Resize( p_size );
		m_textureTxtImg.Resize( p_size );
		m_frameBufferImg.Resize( p_size );
		m_frameBufferTxt.Resize( p_size );
		_updateTxtImage();
	}

	void TextEffects::DoGlInitialise()
	{
		m_textureImg.Create();
		m_textureTmpImg.Create();
		m_textureTxt.Create();
		m_textureTmpTxt.Create();
		m_textureTxtImg.Create();
		m_frameBufferImg.Create();
		m_frameBufferTxt.Create();
	}

	void TextEffects::DoGlPreRender()
	{
		m_textureImg.Initialise( & m_backBuffer );
		m_textureTmpImg.Initialise( m_sizeImage );
		m_textureTxt.Initialise( m_sizeImage );
		m_textureTmpTxt.Initialise( m_sizeImage );
		m_textureTxtImg.Initialise( m_sizeImage );
		m_frameBufferImg.Initialise( m_sizeImage );
		m_frameBufferTxt.Initialise( m_sizeImage );

		if ( m_bFontModified )
		{
			m_bFontModified = false;
			m_bTextModified = true;
			_cleanupArrays();
			_loadFtFont();
		}

		wxString l_strText = m_specEditCaption.m_pControl->GetValue();

		if ( m_bTextModified && m_arrayChars.size() > 0 && m_strText != l_strText )
		{
			m_strText = l_strText;
			wxPoint l_ptPos( 0, 0 );
			wxPoint l_ptCharPos;
			wxChar c;
			wxSize l_sizeChar;
			std::vector < uint8_t > l_bitmap;
			size_t l_uiIndex = 0;
			m_textureTxt.GetBuffer().init();

			for ( wxString::iterator l_it = m_strText.begin() ; l_it != m_strText.end() ; ++l_it )
			{
				c = ( * l_it );
				l_sizeChar = m_arrayChars[c].GetSize();
				l_ptCharPos = m_arrayChars[c].GetPosition();
				l_bitmap = m_arrayChars[c].GetBitmap();
				l_uiIndex = 0;

				if ( c == wxT( '\n' ) )
				{
					l_ptPos = wxPoint( 0, l_ptPos.y + m_iMaxHeight );
				}
				else if ( c == wxT( '\r' ) )
				{
					l_ptPos = wxPoint( 0, l_ptPos.y );
				}
				else
				{
					if ( l_ptPos.x + l_ptCharPos.x + l_sizeChar.x >= m_sizeImage.x )
					{
						l_ptPos = wxPoint( 0, l_ptPos.y + m_iMaxHeight );
					}

					if ( l_ptPos.y + l_ptCharPos.y + l_sizeChar.y < m_sizeImage.y )
					{
						for ( int i = 0 ; i < l_sizeChar.y ; i++ )
						{
							for ( int j = 0 ; j < l_sizeChar.x ; j++ )
							{
								uint8_t l_r = l_bitmap[l_uiIndex++];

								if ( l_r != 0 )
								{
									if ( l_ptPos.x + j + l_ptCharPos.x >= 0 )
									{
										m_textureTxt.GetBuffer()[( m_sizeImage.y - 1 ) - ( l_ptCharPos.y + l_ptPos.y + i )][l_ptPos.x + j + l_ptCharPos.x].Set( l_r, l_r, l_r, l_r );
									}
									else
									{
										m_textureTxt.GetBuffer()[( m_sizeImage.y - 1 ) - ( l_ptCharPos.y + l_ptPos.y + i )][l_ptPos.x + j].Set( l_r, l_r, l_r, l_r );
									}
								}
							}
						}
					}

					l_ptPos.x += l_sizeChar.x + l_ptCharPos.x;
				}
			}

			m_textureTxt.Activate();
			m_textureTxt.UploadSync();
			m_textureTxt.Deactivate();
			m_bTextModified = false;
		}

		m_openGl.Viewport( 0, 0, m_sizeImage.x, m_sizeImage.y );
		m_openGl.MatrixMode( GL_PROJECTION );
		m_openGl.LoadIdentity();
		m_openGl.Ortho( 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 );
		m_openGl.MatrixMode( GL_MODELVIEW );
		m_openGl.LoadIdentity();
	}

	void TextEffects::DoGlRender( bool & p_bChanged )
	{
		if ( m_bNewTexture )
		{
			m_textureImg.Activate();
			m_textureImg.UploadSync();
			m_textureImg.Deactivate();
			m_bNewTexture = false;
		}

		if ( m_bNewTxtTexture )
		{
			m_textureTxtImg.Activate();
			m_textureTxtImg.UploadSync();
			m_textureTxtImg.Deactivate();
			m_bNewTxtTexture = false;
		}

		// First, we blend text with chosen background
		m_frameBufferTxt.Activate();
		m_openGl.Clear( GL_COLOR_BUFFER_BIT );

		if ( m_eTextBackgroundType == eBG_TYPE_TEXTURE )
		{
			// Text background is a texture
			m_textureTxtImg.Activate(	GL_TEXTURE0,	GL_REPLACE,	NULL,				NULL );
			m_textureTxt.Activate(	GL_TEXTURE1,	GL_COMBINE,	& m_blendRgbTxt,	& m_blendAlphaTxt );
			DoSubRender();
			m_textureTxt.Deactivate(	GL_TEXTURE1 );
			m_textureTxtImg.Deactivate(	GL_TEXTURE0 );
		}
		else
		{
			// Text background is a colour
			m_textureTxt.Activate(	GL_TEXTURE0,	GL_COMBINE,	& m_blendRgbTxt,	& m_blendAlphaTxt );
			DoSubRender();
			m_textureTxt.Deactivate(	GL_TEXTURE0 );
		}

		m_frameBufferTxt.Deactivate();
		// Then we cut the background image where the text must be displayed
		m_frameBufferImg.Activate();
		m_openGl.Clear( GL_COLOR_BUFFER_BIT );
		m_openGl.Enable( GL_BLEND );
		m_textureTmpTxt.Activate(	GL_TEXTURE0 );
		m_textureImg.Activate(	GL_TEXTURE1,	GL_COMBINE,	& m_blendRgbImg,	& m_blendAlphaImg );
		DoSubRender();
		m_textureImg.Deactivate(	GL_TEXTURE1 );
		m_textureTmpTxt.Deactivate(	GL_TEXTURE0 );
		m_openGl.Disable( GL_BLEND );
		m_frameBufferImg.Deactivate();
		// Eventually we add the two generated textures
		m_frameBuffer.Activate();
		m_openGl.Clear( GL_COLOR_BUFFER_BIT );
		m_openGl.Enable( GL_BLEND );
		m_textureTmpImg.Activate(	GL_TEXTURE0,	GL_REPLACE );
		m_textureTmpTxt.Activate(	GL_TEXTURE1,	GL_ADD );
		DoSubRender();
		m_textureTmpTxt.Deactivate(	GL_TEXTURE1 );
		m_textureTmpImg.Deactivate(	GL_TEXTURE0 );
		m_openGl.Disable( GL_BLEND );
		m_frameBuffer.Deactivate();
		p_bChanged = false;
	}

	void TextEffects::DoGlPostRender()
	{
	}

	void TextEffects::DoGlCleanup()
	{
		m_textureImg.Cleanup();
		m_textureTmpImg.Cleanup();
		m_textureTxt.Cleanup();
		m_textureTmpTxt.Cleanup();
		m_textureTxtImg.Cleanup();
		m_frameBufferImg.Destroy();
		m_frameBufferTxt.Destroy();
		m_textureImg.Destroy();
		m_textureTmpImg.Destroy();
		m_textureTxt.Destroy();
		m_textureTmpTxt.Destroy();
		m_textureTxtImg.Destroy();
	}

	void TextEffects::DoGeneratePanel()
	{
		int iWidth = 150;
		int iHeight = m_pMainFrame->GetClientSize().y;
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 0 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticTextBackground,		0,													eEVENT_TYPE_NONE,				NULL ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_COMBO,		eID_TEXTBGTYPE,		wxPoint( 10, wxDEFAULT_HEIGHT * 1 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specComboTextBackground,		wxCB_READONLY,										eEVENT_TYPE_COMBOBOX_SELECTED,	wxCommandEventHandler(	TextEffects::OnTxtBackgroundType ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_BUTTON,		eID_TEXTBG_COLOUR,	wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonTextBgColour,		0,													eEVENT_TYPE_LOAD_COLOURA,		wxCommandEventHandler(	TextEffects::OnTxtBackgroundColour ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_TEXTBG_IMAGE,	wxPoint( 10, wxDEFAULT_HEIGHT * 2 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonTextBgImage,		0,													eEVENT_TYPE_LOAD_IMAGEA,		wxCommandEventHandler(	TextEffects::OnTxtBackgroundImage ),	false ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_BACKGROUND,		wxPoint( 10, wxDEFAULT_HEIGHT * 3 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonBackground,			0,													eEVENT_TYPE_LOAD_IMAGEB,		wxCommandEventHandler(	TextEffects::OnBackground ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_FILE_BUTTON,	eID_FONT,			wxPoint( 10, wxDEFAULT_HEIGHT * 4 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specButtonFont,				0,													eEVENT_TYPE_LOAD_BINARY_FILEA,	wxCommandEventHandler(	TextEffects::OnFont ) ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_STATIC,		wxID_ANY,			wxPoint( 10, wxDEFAULT_HEIGHT * 5 ), wxSize( iWidth -  20, wxDEFAULT_HEIGHT ), &m_specStaticCaption,			0,													eEVENT_TYPE_NONE,				NULL ) );
		m_arrayControls.push_back( ControlInfos( eCONTROL_TYPE_EDIT,		eID_TEXT,			wxPoint( 10, wxDEFAULT_HEIGHT * 6 ), wxSize( iWidth -  20, iHeight - wxDEFAULT_HEIGHT * 6 ), &m_specEditCaption,				wxTE_MULTILINE | wxTE_PROCESS_TAB | wxTE_WORDWRAP,	eEVENT_TYPE_TEXT_UPDATED,		wxCommandEventHandler(	TextEffects::OnChar ) ) );
	}

	void TextEffects::OnTxtBackgroundType( wxCommandEvent & p_event )
	{
		eBG_TYPE l_eType = eBG_TYPE( p_event.GetInt() );
		SetTextBackgroundType( l_eType );

		if ( l_eType == eBG_TYPE_COLOUR )
		{
			m_specButtonTextBgColour.m_pControl->Show();
			m_specButtonTextBgImage.m_pControl->Hide();
		}
		else
		{
			m_specButtonTextBgColour.m_pControl->Hide();
			m_specButtonTextBgImage.m_pControl->Show();
		}
	}

	void TextEffects::OnChar( wxCommandEvent & p_event )
	{
		m_bTextModified = true;
		p_event.Skip();
	}

	void TextEffects::OnTxtBackgroundColour( wxCommandEvent & p_event )
	{
		SetTxtColour( wxColour( p_event.GetInt() ) );
		p_event.Skip();
	}

	void TextEffects::OnTxtBackgroundImage( wxCommandEvent & p_event )
	{
		SetTxtImagePath( p_event.GetString() );
		p_event.Skip();
	}

	void TextEffects::OnBackground( wxCommandEvent	& p_event )
	{
		SetImagePath( p_event.GetString() );
		p_event.Skip();
	}

	void TextEffects::OnFont( wxCommandEvent	& p_event )
	{
		SetFont( p_event.GetString() );
		p_event.Skip();
	}

	void TextEffects::OnSize( wxSizeEvent & p_event )
	{
		p_event.Skip();
	}
}
