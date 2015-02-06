#include "TextOverlay.h"
#include "OpenGl.h"
#include "Buffer.h"

using namespace ProceduralTextures;

namespace ProceduralTextures
{
	TextOverlay::Glyph::Glyph()
	{
	}

	TextOverlay::Glyph::Glyph( const Glyph & p_glyph )
		:	m_ptPosition( p_glyph.m_ptPosition )
		,	m_size( p_glyph.m_size )
		,	m_bitmap( p_glyph.m_bitmap )
	{
	}

	TextOverlay::Glyph & TextOverlay::Glyph::operator =( Glyph const & p_glyph )
	{
		m_ptPosition	= p_glyph.m_ptPosition;
		m_size			= p_glyph.m_size;
		m_bitmap		= p_glyph.m_bitmap;
		return * this;
	}

	TextOverlay::Glyph::~Glyph()
	{
		m_bitmap.clear();
	}

	//*************************************************************************************************

	TextOverlay::TextOverlay( OpenGl * p_pOpenGl )
		:	m_pOpenGl( p_pOpenGl )
		,	m_vtxBuffer( p_pOpenGl )
		,	m_idxBuffer( p_pOpenGl )
		,	m_textureBackground( p_pOpenGl )
		,	m_textureForeground( p_pOpenGl )
		,	m_textureText( p_pOpenGl )
		,	m_textureBlendedText( p_pOpenGl )
		,	m_frameBufferBlendedText( p_pOpenGl )
		,	m_textureBackgroundCut( p_pOpenGl )
		,	m_frameBufferBackgroundCut( p_pOpenGl )
		,	m_textureResult( p_pOpenGl )
		,	m_frameBufferResult( p_pOpenGl )
		,	m_bTextModified( false )
		,	m_bBackgroundModified( false )
		,	m_bForegroundModified( false )
		,	m_bMatrixModified( false )
		,	m_iMaxHeight( 0 )
		,	m_arrayGlyphs( 256 )
	{
		std::memset( m_matrix, 0, sizeof( m_matrix ) );
		m_matrix[ 0] = 1.0;
		m_matrix[ 5] = 1.0;
		m_matrix[10] = 1.0;
		m_matrix[15] = 1.0;
		m_blendRgbTxt.function		= GL_MODULATE;
		m_blendRgbTxt.source1		= GL_TEXTURE;
		m_blendRgbTxt.operand1		= GL_SRC_ALPHA;
		m_blendRgbTxt.source2		= GL_CONSTANT;
		m_blendRgbTxt.operand2		= GL_SRC_COLOR;
		m_blendRgbTxt.source3		= GL_NONE;
		m_blendRgbTxt.operand3		= GL_NONE;
		m_blendAlphaTxt.function	= GL_REPLACE;
		m_blendAlphaTxt.source1		= GL_TEXTURE;
		m_blendAlphaTxt.operand1	= GL_SRC_ALPHA;
		m_blendAlphaTxt.source2		= GL_NONE;
		m_blendAlphaTxt.operand2	= GL_NONE;
		m_blendAlphaTxt.source3		= GL_NONE;
		m_blendAlphaTxt.operand3	= GL_NONE;
		m_blendRgbImg.function		= GL_MODULATE;
		m_blendRgbImg.source1		= GL_PREVIOUS;
		m_blendRgbImg.operand1		= GL_ONE_MINUS_SRC_ALPHA;
		m_blendRgbImg.source2		= GL_TEXTURE;
		m_blendRgbImg.operand2		= GL_SRC_COLOR;
		m_blendRgbImg.source3		= GL_NONE;
		m_blendRgbImg.operand3		= GL_NONE;
		m_blendAlphaImg.function	= GL_REPLACE;
		m_blendAlphaImg.source1		= GL_PREVIOUS;
		m_blendAlphaImg.operand1	= GL_ONE_MINUS_SRC_ALPHA;
		m_blendAlphaImg.source2		= GL_NONE;
		m_blendAlphaImg.operand2	= GL_NONE;
		m_blendAlphaImg.source3		= GL_NONE;
		m_blendAlphaImg.operand3	= GL_NONE;
	}

	TextOverlay::~TextOverlay()
	{
	}

	void TextOverlay::Initialise()
	{
		m_textureBackground.Create();
		m_textureForeground.Create();
		m_textureText.Create();
		m_textureBackgroundCut.Create();
		m_frameBufferBackgroundCut.Create();
		m_textureBlendedText.Create();
		m_frameBufferBlendedText.Create();
		m_textureResult.Create();
		m_frameBufferResult.Create();
		m_vtxBuffer.Initialise();
		m_idxBuffer.Initialise();
		m_textureBackground.Initialise( m_size );
		m_textureForeground.Initialise( m_size );
		m_textureText.Initialise( m_size );
		m_textureBackgroundCut.Initialise( m_size );
		m_frameBufferBackgroundCut.Initialise( m_size );
		m_textureBlendedText.Initialise( m_size );
		m_frameBufferBlendedText.Initialise( m_size );
		m_textureResult.Initialise( m_size );
		m_frameBufferResult.Initialise( m_size );
		m_textureBackground.GetBuffer().init();
		m_textureForeground.GetBuffer().init();
		m_textureText.GetBuffer().init();

		for ( int y = 0 ; y < m_size.y ; y++ )
		{
			for ( int x = 0 ; x < m_size.x ; x++ )
			{
				m_textureBackground.GetBuffer()[y][x].r = 0;
				m_textureBackground.GetBuffer()[y][x].g = 0;
				m_textureBackground.GetBuffer()[y][x].b = 0;
				m_textureBackground.GetBuffer()[y][x].a = 0;
				m_textureForeground.GetBuffer()[y][x].r = 255;
				m_textureForeground.GetBuffer()[y][x].g = 255;
				m_textureForeground.GetBuffer()[y][x].b = 255;
				m_textureForeground.GetBuffer()[y][x].a = 255;
			}
		}
	}

	void TextOverlay::Cleanup()
	{
		m_textureBackground.Cleanup();
		m_textureForeground.Cleanup();
		m_textureText.Cleanup();
		m_textureBackgroundCut.Cleanup();
		m_textureBlendedText.Cleanup();
		m_textureResult.Cleanup();
		m_frameBufferBackgroundCut.Cleanup();
		m_frameBufferBlendedText.Cleanup();
		m_frameBufferResult.Cleanup();
		m_vtxBuffer.Destroy();
		m_idxBuffer.Destroy();
		m_textureBackground.Destroy();
		m_textureForeground.Destroy();
		m_textureText.Destroy();
		m_textureBackgroundCut.Destroy();
		m_textureBlendedText.Destroy();
		m_textureResult.Destroy();
		m_frameBufferBackgroundCut.Destroy();
		m_frameBufferBlendedText.Destroy();
		m_frameBufferResult.Destroy();
	}

	void TextOverlay::Create( int p_iHeight, wxFontStyle p_eStyle, wxFontWeight p_eWeight )
	{
		m_textureBackground.Resize( m_size );
		m_textureForeground.Resize( m_size );
		m_textureText.Resize( m_size );
		m_textureBackgroundCut.Resize( m_size );
		m_frameBufferBackgroundCut.Resize( m_size );
		m_frameBufferBackgroundCut.AttachTexture( GL_COLOR_ATTACHMENT0, &m_textureBackgroundCut );
		m_textureBlendedText.Resize( m_size );
		m_frameBufferBlendedText.Resize( m_size );
		m_frameBufferBlendedText.AttachTexture( GL_COLOR_ATTACHMENT0, &m_textureBlendedText );
		m_textureResult.Resize( m_size );
		m_frameBufferResult.Resize( m_size );
		m_frameBufferResult.AttachTexture( GL_COLOR_ATTACHMENT0, &m_textureResult );
		m_iMaxHeight = p_iHeight + 10;
		wxSize l_sizeImg( m_iMaxHeight, m_iMaxHeight );
		wxFont l_font( p_iHeight, wxFONTFAMILY_SWISS, p_eStyle, p_eWeight, false, wxT( "Arial" ) );
		// We first create an image to write each character into.
		wxBitmap l_bitmapMeasure( l_sizeImg.x, l_sizeImg.y, 24 );
		wxMemoryDC l_dcMeasure( l_bitmapMeasure );
		l_dcMeasure.SetFont( l_font );

		// Then we draw the characters.
		for ( uint16_t i = 0 ; i < 256 ; i++ )
		{
			wxChar l_chars[2] = { uint8_t( i ), 0 };
			int l_iWidth = l_dcMeasure.GetTextExtent( l_chars ).x;

			if ( l_iWidth > 0 )
			{
				wxBitmap l_bitmap( l_iWidth, l_sizeImg.y, 24 );
				wxMemoryDC l_dc( l_bitmap );
				l_dc.SetBackground( *wxBLACK_BRUSH );
				l_dc.Clear();
				l_dc.SetTextBackground( *wxBLACK );
				l_dc.SetTextForeground( *wxWHITE );
				l_dc.DrawText( l_chars, 0, 5 );
				ByteArray l_buffer( m_iMaxHeight * m_iMaxHeight );
				wxNativePixelData l_data( l_bitmap );
				uint8_t * l_pBuffer = &l_buffer[0];

				if ( l_data )
				{
					wxNativePixelData::Iterator l_it( l_data );

					for ( int y = 0 ; y < m_iMaxHeight ; ++y )
					{
						wxNativePixelData::Iterator l_itRowStart = l_it;

						for ( int x = 0 ; x < m_iMaxHeight ; ++x, ++l_it )
						{
							*l_pBuffer = l_it.Red();
							l_pBuffer++;
						}

						l_it = l_itRowStart;
						l_it.OffsetY( l_data, 1 );
					}
				}

				m_arrayGlyphs[i].SetSize( wxSize( l_iWidth, m_iMaxHeight ) );
				m_arrayGlyphs[i].SetPosition( wxPoint( 0, 0 ) );
				m_arrayGlyphs[i].SetBitmap( l_buffer );
			}
		}
	}

	void TextOverlay::Draw( GlFrameBuffer & p_frameBuffer )
	{
		if ( m_bBackgroundModified )
		{
			m_textureBackground.Activate();
			m_textureBackground.UploadSync();
			m_textureBackground.Deactivate();
			m_bBackgroundModified = false;
		}

		if ( m_bForegroundModified )
		{
			m_textureForeground.Activate();
			m_textureForeground.UploadSync();
			m_textureForeground.Deactivate();
			m_bForegroundModified = false;
		}

		m_textureBackground.Initialise( m_size );
		m_textureForeground.Initialise( m_size );
		m_textureText.Initialise( m_size );
		m_textureBackgroundCut.Initialise( m_size );
		m_frameBufferBackgroundCut.Initialise( m_size );
		m_textureBlendedText.Initialise( m_size );
		m_frameBufferBlendedText.Initialise( m_size );
		m_textureResult.Initialise( m_size );
		m_frameBufferResult.Initialise( m_size );

		if ( m_bTextModified && m_arrayGlyphs.size() > 0 )
		{
			wxPoint l_ptPos( 0, 0 );
			wxChar c;
			size_t l_uiIndex = 0;
			PixelBuffer & l_pxBuffer = m_textureText.GetBuffer();
			l_pxBuffer.init();

			for ( wxString::iterator l_it = m_strText.begin() ; l_it != m_strText.end() ; ++l_it )
			{
				c = *l_it;
				wxSize const & l_sizeChar = m_arrayGlyphs[c].GetSize();
				wxPoint const & l_ptCharPos = m_arrayGlyphs[c].GetPosition();
				ByteArray const & l_buffer = m_arrayGlyphs[c].GetBitmap();
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
					if ( l_ptPos.x + l_ptCharPos.x + l_sizeChar.x >= m_size.x )
					{
						l_ptPos = wxPoint( 0, l_ptPos.y + m_iMaxHeight );
					}

					if ( l_ptPos.y + l_ptCharPos.y + l_sizeChar.y <= m_size.y )
					{
						for ( int y = 0 ; y < l_sizeChar.y ; y++ )
						{
							for ( int x = 0 ; x < l_sizeChar.x ; x++ )
							{
								uint8_t l_r = l_buffer[l_uiIndex++];

								if ( l_r != 0 )
								{
									if ( l_ptPos.x + x + l_ptCharPos.x >= 0 )
									{
										l_pxBuffer[( m_size.y - 1 ) - ( l_ptCharPos.y + l_ptPos.y + y )][l_ptPos.x + x + l_ptCharPos.x].Set( l_r, l_r, l_r, l_r );
									}
									else
									{
										l_pxBuffer[( m_size.y - 1 ) - ( l_ptCharPos.y + l_ptPos.y + y )][l_ptPos.x + x].Set( l_r, l_r, l_r, l_r );
									}
								}
							}
						}
					}

					l_ptPos.x += l_sizeChar.x + l_ptCharPos.x;
				}
			}

			wxBitmap l_bitmap( m_size.x, m_size.y, 24 );
			wxNativePixelData l_data( l_bitmap );
			UbPixel * l_pBuffer = l_pxBuffer.ptr();

			if ( l_data )
			{
				wxNativePixelData::Iterator l_it( l_data );

				for ( int y = 0 ; y < m_iMaxHeight ; ++y )
				{
					wxNativePixelData::Iterator l_itRowStart = l_it;

					for ( int x = 0 ; x < m_iMaxHeight ; ++x, ++l_it )
					{
						l_it.Red()		= l_pBuffer->r;
						l_it.Green()	= l_pBuffer->r;
						l_it.Blue()		= l_pBuffer->r;
					}

					l_it = l_itRowStart;
					l_it.OffsetY( l_data, 1 );
				}
			}

			m_textureText.Activate();
			m_textureText.UploadSync();
			m_textureText.Deactivate();
			m_bTextModified = false;
		}

		if ( m_bMatrixModified )
		{
			wxSize const & l_size = p_frameBuffer.GetSize();
			m_matrix[ 0] = m_size.x / GLfloat( l_size.x );
			m_matrix[ 5] = m_size.y / GLfloat( l_size.y );
			m_matrix[12] = m_position.x / GLfloat( l_size.x );
			m_matrix[13] = m_position.y / GLfloat( l_size.y );
			m_bMatrixModified = false;
		}

		m_pOpenGl->ClearColor( 0.0, 0.0, 0.0, 0.0 );
		m_pOpenGl->MatrixMode( GL_MODELVIEW );
		m_pOpenGl->LoadIdentity();
		// First, we blend text with chosen text foreground
		m_frameBufferBlendedText.Activate();
		m_pOpenGl->Clear( GL_COLOR_BUFFER_BIT );
		m_textureForeground.Activate(	GL_TEXTURE0,	GL_REPLACE,	NULL,			NULL );
		m_textureText.Activate(	GL_TEXTURE1,	GL_COMBINE,	&m_blendRgbTxt,	&m_blendAlphaTxt );
		DoSubRender();
		m_textureText.Deactivate(	GL_TEXTURE1 );
		m_textureForeground.Deactivate(	GL_TEXTURE0 );
		m_frameBufferBlendedText.Deactivate();
		m_pOpenGl->Enable( GL_BLEND );
		m_pOpenGl->BlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		// Then we blend the blended text with the background to obtain the full overlay
		m_frameBufferResult.Activate();
		m_pOpenGl->Clear( GL_COLOR_BUFFER_BIT );
		m_textureText.Activate(	GL_TEXTURE0 );
		DoSubRender();
		m_textureText.Deactivate(	GL_TEXTURE0 );
		m_frameBufferResult.Deactivate();
		// Eventually we render the overlay into the given buffer
		m_pOpenGl->MatrixMode( GL_MODELVIEW );
		m_pOpenGl->LoadIdentity();
		m_pOpenGl->MultMatrix( m_matrix );
		p_frameBuffer.Activate();
		m_textureResult.Activate(	GL_TEXTURE0 );
		DoSubRender();
		m_textureResult.Deactivate(	GL_TEXTURE0 );
		p_frameBuffer.Deactivate();
		m_pOpenGl->Disable( GL_BLEND );
	}

	void TextOverlay::SetBackground( wxColour const & p_colour )
	{
		PixelBuffer & l_buffer = m_textureBackground.GetBuffer();
		l_buffer.init( Size( m_size.x, m_size.y ) );

		for ( int y = 0 ; y < m_size.y ; y++ )
		{
			for ( int x = 0 ; x < m_size.x ; x++ )
			{
				l_buffer[y][x].r = p_colour.Red();
				l_buffer[y][x].g = p_colour.Green();
				l_buffer[y][x].b = p_colour.Blue();
				l_buffer[y][x].a = p_colour.Alpha();
			}
		}

		m_bBackgroundModified = true;
	}

	void TextOverlay::SetForeground( wxColour const & p_colour )
	{
		PixelBuffer & l_buffer = m_textureForeground.GetBuffer();
		l_buffer.init( Size( m_size.x, m_size.y ) );

		for ( int y = 0 ; y < m_size.y ; y++ )
		{
			for ( int x = 0 ; x < m_size.x ; x++ )
			{
				l_buffer[y][x].r = p_colour.Red();
				l_buffer[y][x].g = p_colour.Green();
				l_buffer[y][x].b = p_colour.Blue();
				l_buffer[y][x].a = p_colour.Alpha();
			}
		}

		m_bForegroundModified = true;
	}

	void TextOverlay::SetBackground( wxImage const & p_image )
	{
		wxImage l_image = p_image.ResampleBicubic( m_size.x, m_size.y );
		PixelBuffer & l_buffer = m_textureBackground.GetBuffer();
		l_buffer.init( Size( m_size.x, m_size.y ) );

		if ( p_image.HasAlpha() )
		{
			l_buffer.set< PG_UINT8_RGBA >( p_image.GetData(), p_image.GetAlpha() );
		}
		else
		{
			l_buffer.set< PG_UINT8_RGB >( p_image.GetData(), NULL );
		}

		m_bBackgroundModified = true;
	}

	void TextOverlay::SetForeground( wxImage const & p_image )
	{
		wxImage l_image = p_image.ResampleBicubic( m_size.x, m_size.y );
		PixelBuffer & l_buffer = m_textureForeground.GetBuffer();
		l_buffer.init( Size( m_size.x, m_size.y ) );

		if ( p_image.HasAlpha() )
		{
			l_buffer.set< PG_UINT8_RGBA >( p_image.GetData(), p_image.GetAlpha() );
		}
		else
		{
			l_buffer.set< PG_UINT8_RGB >( p_image.GetData(), NULL );
		}

		m_bForegroundModified = true;
	}

	void TextOverlay::DoSubRender()
	{
		if ( m_vtxBuffer.Activate() )
		{
			if ( m_idxBuffer.Activate() )
			{
				m_pOpenGl->DrawElements( GL_TRIANGLES, GlIndexBuffer::Size, GL_UNSIGNED_INT, 0 );
				m_idxBuffer.Deactivate();
			}

			m_vtxBuffer.Deactivate();
		}
	}
}
