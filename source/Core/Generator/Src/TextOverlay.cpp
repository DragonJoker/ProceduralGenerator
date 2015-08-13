#include "TextOverlay.h"

#include "Font.h"
#include "GeometryBuffers.h"
#include "GlFrameVariable.h"
#include "GlVec4FrameVariable.h"
#include "GlShaderProgram.h"
#include "GlTexture.h"
#include "GlVertexBuffer.h"
#include "Buffer.h"

#if defined( DrawText )
#	undef DrawText
#endif

namespace ProceduralTextures
{
	TextOverlay::TextOverlay( gl::OpenGl & p_openGl, Material const & p_material, std::shared_ptr< Overlay > p_parent )
		: Overlay( p_openGl, p_material, eOVERLAY_TYPE_TEXT, p_parent )
		, m_wrappingMode( eTEXT_WRAPPING_MODE_NONE )
		, m_glyphsTexture( p_openGl )
		, m_hAlign( eHALIGN_LEFT )
		, m_vAlign( eVALIGN_CENTER )
	{
	}

	TextOverlay::~TextOverlay()
	{
		m_wpFont.reset();
	}

	void TextOverlay::SetFont( std::shared_ptr< Font > p_font )
	{
		if ( p_font )
		{
			m_wpFont = p_font;
		}

		m_changed = true;
	}

	void TextOverlay::SetProgram( eMATERIAL_TYPE p_type, std::shared_ptr< gl::ShaderProgram > p_program )
	{
		m_material.SetType( p_type, p_program );
		std::shared_ptr< gl::FrameVariable< int > > l_tex = p_program->GetIntFrameVariable( _T( "pxl_mapText" ) );
		m_uniformTextTexture = l_tex;

		if ( l_tex )
		{
			switch ( p_type )
			{
			case eMATERIAL_TYPE_COLOUR:
				l_tex->SetValue( 0 );
				break;

			case eMATERIAL_TYPE_TEXTURE:
				l_tex->SetValue( 1 );
				break;
			}
		}
	}

	Position const & TextOverlay::GetGlyphPosition( char32_t p_char )const
	{
		auto l_it = m_glyphsPositions.find( p_char );

		if ( l_it == m_glyphsPositions.end() )
		{
			throw std::runtime_error( std::string( "No loaded glyph for character " ) + std::to_string( p_char ) );
		}

		return l_it->second;
	}

	void TextOverlay::DoInitialise()
	{
		std::shared_ptr< Font > l_font = GetFont();

		if ( l_font )
		{
			uint32_t l_uiMaxWidth = l_font->GetMaxWidth();
			uint32_t l_uiMaxHeight = l_font->GetMaxHeight();
			uint32_t l_glyphsXCount = 16;
			uint32_t l_glyphsYCount = uint32_t( std::ceil( std::distance( l_font->Begin(), l_font->End() ) / double( l_glyphsXCount ) ) );
			Size l_sizeImg( l_uiMaxWidth * l_glyphsXCount, l_uiMaxHeight * l_glyphsYCount );

			m_glyphsTexture.Create();
			m_glyphsTexture.Initialise( l_sizeImg );

			Font::GlyphMap::const_iterator l_it = l_font->Begin();
			uint32_t l_uiTotalWidth = l_sizeImg.x();
			uint32_t l_uiTotalHeight = l_sizeImg.y();
			uint32_t l_uiOffY = l_uiTotalHeight - l_uiMaxHeight;
			UbPixel * l_pBuffer = m_glyphsTexture.GetBuffer().Ptr();
			size_t l_bufsize = m_glyphsTexture.GetBuffer().GetSize();

			for ( uint32_t y = 0; y < l_glyphsYCount && l_it != l_font->End(); ++y )
			{
				uint32_t l_uiOffX = 0;

				for ( uint32_t x = 0; x < l_glyphsXCount && l_it != l_font->End(); ++x )
				{
					Glyph const & l_glyph = l_it->second;
					Size l_size = l_glyph.GetSize();
					uint32_t l_glyphPitch = l_size.x() * 4;
					std::vector< UbPixel > l_buffer = l_glyph.GetBitmap();

					for ( uint32_t glyphY = 0; glyphY < l_size.y(); ++glyphY )
					{
						uint32_t l_lineOffset = l_uiTotalWidth * ( l_uiOffY + glyphY );// Go to the good line.
						assert( l_lineOffset + l_uiOffX + l_size.x() <= l_bufsize );
						std::memcpy( &l_pBuffer[l_lineOffset + l_uiOffX], &l_buffer[glyphY * l_size.x()], l_glyphPitch );
					}

					m_glyphsPositions[l_glyph.GetCharacter()] = Position( l_uiOffX, l_uiOffY );
					l_uiOffX += l_uiMaxWidth;
					++l_it;
				}

				l_uiOffY -= l_uiMaxHeight;
			}

			m_glyphsTexture.Bind();
			m_glyphsTexture.UploadSync();
			m_glyphsTexture.Unbind();
		}
	}

	void TextOverlay::DoCleanup()
	{
		m_glyphsTexture.Cleanup();
		m_glyphsTexture.Destroy();
	}

	void TextOverlay::DoRender()
	{
		if ( GetFont() && !m_caption.empty() )
		{
			uint32_t l_index = 0;

			if ( m_material.GetType() == eMATERIAL_TYPE_TEXTURE )
			{
				l_index++;
			}

			m_uniformTextTexture.lock()->SetValue( l_index );
			m_material.Activate();
			m_glyphsTexture.Activate( GL_TEXTURE0 + l_index );
			m_geometryBuffers.Draw( m_material.GetVertexAttribute(), m_material.GetTextureAttribute() );
			m_glyphsTexture.Deactivate( GL_TEXTURE0 + l_index );
			m_material.Deactivate();
		}
	}

	void TextOverlay::DoUpdate()
	{
		Position l_ovPosition = GetAbsolutePixelPosition();
		std::shared_ptr< Font > l_font = GetFont();

		if ( !m_caption.empty() && l_font )
		{
			if ( m_previousCaption != m_caption || m_changed || m_previousDisplaySize != m_displaySize )
			{
				m_previousCaption = m_caption;
				m_previousDisplaySize = m_displaySize;
				m_characters.clear();
				Size l_size = GetAbsolutePixelSize();
				Position l_position;
				std::vector< String > l_lines;
				size_t l_pos = 0;
				String l_caption( m_previousCaption );

				while ( ( l_pos = l_caption.find( _T( '\n' ) ) ) != String::npos )
				{
					String l_token = l_caption.substr( 0, l_pos );
					l_lines.push_back( l_token );
					l_caption.erase( 0, l_pos + 1 );
				}

				std::vector< std::vector< VertexI > > l_linesVtx;
				std::vector< VertexI > l_lineVtx;

				l_lines.push_back( l_caption );
				int32_t l_lineWidth = 0;

				for ( auto const & l_line : l_lines )
				{
					int32_t l_wordWidth = 0;
					String l_word;

					for ( StringUtils::Utf8Iterator l_itLine = l_line.begin(); l_itLine != l_line.end() && l_position.y() < int32_t( l_size.y() ); ++l_itLine )
					{
						char const & l_character = *l_itLine;
						Glyph const & l_glyph = l_font->GetGlyphAt( l_character );
						uint32_t l_charWidth = std::max( l_glyph.GetSize().x(), l_glyph.GetAdvance().x() );

						if ( l_character == _T( '\r' ) )
						{
							DoWriteWord( l_word, l_wordWidth, l_size, l_position, l_lineWidth, l_lineVtx, l_linesVtx );
							l_position.x() = 0;
							l_wordWidth = 0;
						}
						else if ( l_character == _T( ' ' ) )
						{
							DoWriteWord( l_word, l_wordWidth, l_size, l_position, l_lineWidth, l_lineVtx, l_linesVtx );
							l_word.clear();
							l_wordWidth = 0;
							l_position.x() += l_charWidth;
						}
						else if ( l_character == _T( '\t' ) )
						{
							DoWriteWord( l_word, l_wordWidth, l_size, l_position, l_lineWidth, l_lineVtx, l_linesVtx );
							l_word.clear();
							l_wordWidth = 0;
							l_position.x() += l_charWidth;
						}
						else
						{
							l_word += l_character;
							l_wordWidth += l_charWidth;
						}
					}

					if ( !l_word.empty() )
					{
						DoWriteWord( l_word, l_wordWidth, l_size, l_position, l_lineWidth, l_lineVtx, l_linesVtx );

						if ( !l_lineVtx.empty() )
						{
							DoAlignHorizontally( l_size.x(), l_lineWidth, l_lineVtx, l_linesVtx );
						}
					}

					l_linesVtx.push_back( l_lineVtx );
					l_position.x() = 0;
					l_position.y() += l_font->GetMaxHeight();
				}

				DoAlignVertically( l_size.y(), l_position.y(), l_linesVtx );
				std::vector< VertexI > l_arrayVtx;

				for ( auto && l_lineVtx : l_linesVtx )
				{
					l_arrayVtx.insert( l_arrayVtx.end(), l_lineVtx.begin(), l_lineVtx.end() );
				}

				if ( !l_arrayVtx.empty() )
				{
					m_geometryBuffers.GetVertexBuffer().SetBuffer( l_arrayVtx );
				}
			}
		}
	}

	void TextOverlay::DoUpdatePositionAndSize( Size const & p_size )
	{
	}

	void TextOverlay::DoWriteWord( String const & p_word, int32_t p_wordWidth, Size const & p_size, Position & p_position, int32_t & p_lineWidth, std::vector< VertexI > & p_lineVtx, std::vector< std::vector< VertexI > > & p_linesVtx )
	{
		Position l_ovPosition = GetAbsolutePixelPosition();
		std::shared_ptr< Font > l_font = GetFont();
		uint32_t l_maxHeight = l_font->GetMaxHeight();

		if ( p_position.x() + p_wordWidth > int32_t( p_size.x() ) && m_wrappingMode == eTEXT_WRAPPING_MODE_BREAK_WORDS )
		{
			p_position.x() = 0;
			p_position.y() += l_maxHeight;
		}

		for ( auto && l_it = p_word.begin(); l_it != p_word.end() && p_position.y() < int32_t( p_size.y() ); ++l_it )
		{
			char const & l_character = *l_it;
			int32_t l_charCrop = 0;
			Glyph const & l_glyph = l_font->GetGlyphAt( l_character );
			Size l_charSize( std::max( l_glyph.GetSize().x(), l_glyph.GetAdvance().x() ), std::max( l_glyph.GetSize().y(), l_glyph.GetAdvance().y() ) );

			p_position.x() += l_glyph.GetPosition().x();

			if ( p_position.x() > int32_t( p_size.x() ) )
			{
				// The character is beyond X limits
				if ( m_wrappingMode == eTEXT_WRAPPING_MODE_NONE )
				{
					l_charSize.x() = 0;
				}
				else if ( m_wrappingMode == eTEXT_WRAPPING_MODE_BREAK )
				{
					p_position.x() = 0;
					p_position.y() += l_maxHeight;
					DoAlignHorizontally( p_size.x(), p_lineWidth, p_lineVtx, p_linesVtx );
				}
			}
			else if ( p_position.x() + l_charSize.x() > p_size.x() )
			{
				// A part of the character is beyond X limits
				if ( m_wrappingMode == eTEXT_WRAPPING_MODE_NONE )
				{
					l_charSize.x() = p_size.x() - p_position.x();
				}
				else if ( m_wrappingMode == eTEXT_WRAPPING_MODE_BREAK )
				{
					p_position.x() = 0;
					p_position.y() += l_maxHeight;
					DoAlignHorizontally( p_size.x(), p_lineWidth, p_lineVtx, p_linesVtx );
				}
			}

			if ( p_position.y() > int32_t( p_size.y() ) )
			{
				// The character is beyond Y limits
				l_charSize.y() = 0;
			}
			else if ( p_position.y() + l_maxHeight > p_size.y() )
			{
				// A part of the character is beyond Y limits
				l_charCrop = p_position.y() + l_maxHeight - p_size.y();
			}

			if ( l_charSize.x() > 0 && l_charSize.y() > 0 && l_charCrop < int32_t( l_charSize.y() ) )
			{
				Position l_position( p_position.x(), p_position.y() + l_maxHeight - l_glyph.GetPosition().y() );
				uint32_t l_width = l_charSize.x();
				uint32_t l_height = l_charSize.y() - l_charCrop;
				Position l_uvPosition = GetGlyphPosition( l_character );
				Size l_texDim = m_glyphsTexture.GetBuffer().GetDimensions();
				double l_uvX = double( l_uvPosition.x() ) / l_texDim.x();
				double l_uvY = double( l_uvPosition.y() ) / l_texDim.y();
				double l_uvStepX = double( l_charSize.x() ) / l_texDim.y();
				double l_uvStepY = double( l_charSize.y() ) / l_texDim.y();
				double l_uvCrop = double( l_charCrop ) / l_texDim.y();
				p_lineWidth += l_width;

				VertexI l_vertexTR( CONSTRUCT_ANONYMOUS( VertexI::TPoint, int32_t( l_ovPosition.x() + l_position.x() + l_width ), int32_t( l_ovPosition.y() + l_position.y() ) ), CONSTRUCT_ANONYMOUS( Point2d, l_uvX + l_uvStepX, l_uvY + l_uvStepY ) );
				VertexI l_vertexTL( CONSTRUCT_ANONYMOUS( VertexI::TPoint, int32_t( l_ovPosition.x() + l_position.x() ), int32_t( l_ovPosition.y() + l_position.y() ) ), CONSTRUCT_ANONYMOUS( Point2d, l_uvX, l_uvY + l_uvStepY ) );
				VertexI l_vertexBL( CONSTRUCT_ANONYMOUS( VertexI::TPoint, int32_t( l_ovPosition.x() + l_position.x() ), int32_t( l_ovPosition.y() + l_position.y() + l_height ) ), CONSTRUCT_ANONYMOUS( Point2d, l_uvX, l_uvY + l_uvCrop ) );
				VertexI l_vertexBR( CONSTRUCT_ANONYMOUS( VertexI::TPoint, int32_t( l_ovPosition.x() + l_position.x() + l_width ), int32_t( l_ovPosition.y() + l_position.y() + l_height ) ), CONSTRUCT_ANONYMOUS( Point2d, l_uvX + l_uvStepX, l_uvY + l_uvCrop ) );

				p_lineVtx.push_back( l_vertexBL );
				p_lineVtx.push_back( l_vertexBR );
				p_lineVtx.push_back( l_vertexTL );

				p_lineVtx.push_back( l_vertexTR );
				p_lineVtx.push_back( l_vertexTL );
				p_lineVtx.push_back( l_vertexBR );

				m_characters.push_back( Character( l_glyph.GetCharacter(), l_vertexTL.GetPosition(), Size( l_width, l_height ) ) );
			}

			p_position.x() += l_charSize.x();
		}
	}

	void TextOverlay::DoAlignHorizontally( int32_t p_width, int32_t & p_lineWidth, std::vector< VertexI > & p_lineVtx, std::vector< std::vector< VertexI > > & p_linesVtx )
	{
		if ( m_hAlign != eHALIGN_LEFT )
		{
			int32_t l_offset = p_width - p_lineWidth;

			if ( m_hAlign == eHALIGN_CENTER )
			{
				l_offset /= 2;
			}

			for ( auto && l_vertex : p_lineVtx )
			{
				VertexI::TPoint & l_position = l_vertex.GetPosition();
				l_position[0] += l_offset;
			}
		}

		p_linesVtx.push_back( p_lineVtx );
		p_lineVtx.clear();
		p_lineWidth = 0;
	}

	void TextOverlay::DoAlignVertically( int32_t p_height, int32_t p_linesHeight, std::vector< std::vector< VertexI > > & p_linesVtx )
	{
		if ( m_vAlign != eVALIGN_TOP )
		{
			int32_t l_offset = 0;

			if ( m_vAlign == eVALIGN_CENTER )
			{
				l_offset = ( p_height - p_linesHeight ) / 2;
			}
			else
			{
				l_offset = p_height - p_linesHeight;
			}

			for ( auto && l_lineVtx : p_linesVtx )
			{
				for ( auto && l_vertex : l_lineVtx )
				{
					VertexI::TPoint & l_position = l_vertex.GetPosition();
					l_position[1] += l_offset;
				}
			}
		}
	}
}
