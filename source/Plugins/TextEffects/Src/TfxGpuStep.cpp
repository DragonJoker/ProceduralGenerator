#include "TextEffects.h"

#include <Font.h>
#include <GlFrameBuffer.h>
#include <GlFrameVariable.h>
#include <GlTexture.h>
#include <GlVec4FrameVariable.h>

#include <ft2build.h>

FT_BEGIN_HEADER
#	include FT_FREETYPE_H
#	include FT_GLYPH_H
#	include FT_OUTLINE_H
#	include FT_TRIGONOMETRY_H
#	include FT_TYPES_H
FT_END_HEADER

namespace TextEffects
{
	const ProceduralTextures::String VertexShader =
		_T( "attribute vec2 vertex;\n" )
		_T( "attribute vec2 texture;\n" )
		_T( "varying vec2 pxl_texture;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "	pxl_texture = texture;\n" )
		_T( "	gl_Position = vec4( vertex.xy, 0.0, 1.0 );\n" )
		_T( "}\n" );

	const ProceduralTextures::String PixelShaderColour =
		_T( "varying vec2 pxl_texture;\n" )
		_T( "uniform sampler2D pxl_mapBackground;\n" )
		_T( "uniform sampler2D pxl_mapTextMask;\n" )
		_T( "uniform vec4 pxl_colourForeground;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "	float alpha = texture2D( pxl_mapTextMask, pxl_texture ).a;\n" )
		_T( "	gl_FragColor = ( 1.0 - alpha ) * texture2D( pxl_mapBackground, pxl_texture ) + alpha * pxl_colourForeground;\n" )
		_T( "}\n" );

	const ProceduralTextures::String PixelShaderTexture =
		_T( "varying vec2 pxl_texture;\n" )
		_T( "uniform sampler2D pxl_mapBackground;\n" )
		_T( "uniform sampler2D pxl_mapTextMask;\n" )
		_T( "uniform sampler2D pxl_mapForeground;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "	float alpha = texture2D( pxl_mapTextMask, pxl_texture ).a;\n" )
		_T( "	gl_FragColor = ( 1.0 - alpha ) * texture2D( pxl_mapBackground, pxl_texture ) + alpha * texture2D( pxl_mapForeground, pxl_texture );\n" )
		_T( "}\n" );

	Program::Program( ProceduralTextures::gl::OpenGl & p_gl )
		:	m_program( std::make_shared< ProceduralTextures::gl::ShaderProgram >( p_gl ) )
	{
	}

	void Program::Initialise( ProceduralTextures::String const & p_vertex, ProceduralTextures::String const & p_pixel )
	{
		m_program->CreateObject( ProceduralTextures::gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_program->CreateObject( ProceduralTextures::gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_program->SetProgramText( p_vertex, ProceduralTextures::gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_program->SetProgramText( p_pixel, ProceduralTextures::gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_program->Initialise();
		m_uniformBackground = m_program->CreateIntFrameVariable( _T( "pxl_mapBackground" ) );
		m_uniformBackground.lock()->SetValue( 0 );
		m_uniformTextMask = m_program->CreateIntFrameVariable( _T( "pxl_mapTextMask" ) );
		m_uniformTextMask.lock()->SetValue( 1 );
		m_vertex = m_program->GetAttributeLocation( _T( "vertex" ) );
		m_texture = m_program->GetAttributeLocation( _T( "texture" ) );
	}

	void Program::Cleanup()
	{
	}

	//*************************************************************************************************

	GpuStep::GpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize )
		: ProceduralTextures::GpuStep( p_generator, p_size, p_bordersSize )
		, m_bFontModified( false )
		, m_bTextModified( false )
		, m_newBackgroundTexture( false )
		, m_newForegroundTexture( false )
		, m_foregroundType( ProceduralTextures::eMATERIAL_TYPE_COLOUR )
		, m_textureBackground( std::make_shared< ProceduralTextures::gl::Texture >( *m_openGl ) )
		, m_textureTextMask( std::make_shared< ProceduralTextures::gl::Texture >( *m_openGl ) )
		, m_textureForeground( std::make_shared< ProceduralTextures::gl::Texture >( *m_openGl ) )
		, m_programColour( std::make_shared< Program >( *m_openGl ) )
		, m_programTexture( std::make_shared< Program >( *m_openGl ) )
	{
		m_textureBackground->Resize( p_size );
		m_textureTextMask->Resize( p_size );
		m_textureForeground->Resize( p_size );

		if ( m_foregroundType == ProceduralTextures::eMATERIAL_TYPE_COLOUR )
		{
			m_currentProgram = m_programColour;
		}
		else
		{
			m_currentProgram = m_programTexture;
		}
	}

	GpuStep::~GpuStep()
	{
		DoCleanupGlyphs();
	}

	void GpuStep::SetForegroundType( int p_value )
	{
		m_foregroundType = ProceduralTextures::eMATERIAL_TYPE( p_value );

		if ( m_foregroundType == ProceduralTextures::eMATERIAL_TYPE_COLOUR )
		{
			m_currentProgram = m_programColour;
		}
		else
		{
			m_currentProgram = m_programTexture;
		}
	}

	void GpuStep::SetForegroundColour( uint32_t p_colour )
	{
		m_colourForeground = ProceduralTextures::Utils::FromBGRColour( p_colour );
	}

	void GpuStep::SetForegroundImage( ProceduralTextures::PixelBuffer const & p_image )
	{
		m_textureForeground->GetBuffer() = p_image;
		m_textureForeground->Resize( p_image.GetDimensions() );
		m_newForegroundTexture = true;
	}

	void GpuStep::SetBackgroundImage( ProceduralTextures::PixelBuffer const & p_image )
	{
		m_initialised = false;
		m_textureBackground->GetBuffer() = p_image;
		m_finalBuffer->Initialise( p_image.GetDimensions() );
		m_newBackgroundTexture = true;
		m_initialised = true;
	}

	void GpuStep::DoInitialise()
	{
		m_textureBackground->Create();
		m_textureTextMask->Create();
		m_textureForeground->Create();
		m_textureBackground->Initialise( m_sizeImage );
		m_textureTextMask->Initialise( m_sizeImage );
		m_textureForeground->Initialise( m_sizeImage );
		m_programColour->Initialise( VertexShader, PixelShaderColour );
		m_uniformColourForeground = m_programColour->m_program->CreateVec4FloatFrameVariable( _T( "pxl_colourForeground" ) );
		m_programTexture->Initialise( VertexShader, PixelShaderTexture );
		m_uniformTextureForeground = m_programTexture->m_program->CreateIntFrameVariable( _T( "pxl_mapForeground" ) );
		m_uniformTextureForeground.lock()->SetValue( 2 );
	}

	void GpuStep::DoCleanup()
	{
		m_textureBackground->Cleanup();
		m_textureTextMask->Cleanup();
		m_textureForeground->Cleanup();
		m_textureForeground->Destroy();
		m_textureTextMask->Destroy();
		m_textureBackground->Destroy();
	}

	void GpuStep::DoResize()
	{
		m_textureBackground->Resize( m_sizeImage );
		m_textureTextMask->Resize( m_sizeImage );
	}

	void GpuStep::DoPreRender()
	{
		m_textureTextMask->Initialise( m_sizeImage );

		if ( m_bFontModified )
		{
			m_bFontModified = false;
			m_bTextModified = true;
			DoCleanupGlyphs();
			DoLoadFont();
		}

		ProceduralTextures::String l_strText = m_strText;

		if ( m_bTextModified && !m_font.expired() && m_strPreviousText != l_strText )
		{
			m_strPreviousText = l_strText;
			DoUpdateText();
		}

		if ( m_newBackgroundTexture )
		{
			m_textureBackground->Activate();
			m_textureBackground->UploadSync();
			m_textureBackground->Deactivate();
			m_newBackgroundTexture = false;
		}

		if ( m_newForegroundTexture )
		{
			m_textureForeground->Activate();
			m_textureForeground->UploadSync();
			m_textureForeground->Deactivate();
			m_newForegroundTexture = false;
		}
	}

	void GpuStep::DoRender( bool & p_bChanged )
	{
		m_openGl->Viewport( 0, 0, m_sizeImage.x(), m_sizeImage.y() );
		m_frameBuffer->Bind();
		m_openGl->Clear( GL_COLOR_BUFFER_BIT );
		std::shared_ptr< Program > l_program = m_currentProgram.lock();
		l_program->m_program->Activate();
		l_program->m_uniformBackground.lock()->Apply();
		m_textureBackground->Activate( GL_TEXTURE0 );
		l_program->m_uniformTextMask.lock()->Apply();
		m_textureTextMask->Activate( GL_TEXTURE1 );

		if ( m_foregroundType == ProceduralTextures::eMATERIAL_TYPE_TEXTURE )
		{
			m_uniformTextureForeground.lock()->Apply();
			m_textureForeground->Activate( GL_TEXTURE2 );
		}
		else
		{
			m_uniformColourForeground.lock()->SetValue( m_colourForeground.r / 255.0f, m_colourForeground.g / 255.0f, m_colourForeground.b / 255.0f, m_colourForeground.a / 255.0f );
			m_uniformColourForeground.lock()->Apply();
		}

		DoSubRender( l_program->m_vertex, l_program->m_texture );
		l_program->m_program->Deactivate();

		if ( m_foregroundType == ProceduralTextures::eMATERIAL_TYPE_TEXTURE )
		{
			m_textureForeground->Deactivate( GL_TEXTURE2 );
		}

		m_textureTextMask->Deactivate( GL_TEXTURE1 );
		m_textureBackground->Deactivate( GL_TEXTURE0 );
		m_frameBuffer->Unbind();
		p_bChanged = false;
	}

	void GpuStep::DoPostRender()
	{
	}

	void GpuStep::DoLoadFont()
	{
		ProceduralTextures::String l_name = m_strFontFile.substr( m_strFontFile.rfind( ProceduralTextures::FOLDER_SEPARATOR ) + 1 );
		l_name = l_name.substr( 0, l_name.rfind( _T( '.' ) ) );
		m_font = m_generator.lock()->LoadFont( l_name, 40, m_strFontFile );
	}

	void GpuStep::DoCleanupGlyphs()
	{
		m_font.reset();
	}

	void GpuStep::DoUpdateText()
	{
		std::shared_ptr< ProceduralTextures::Font > l_font = m_font.lock();

		if ( l_font )
		{
			ProceduralTextures::Position l_ptPos( 0, 0 );
			m_textureTextMask->GetBuffer().Initialise();
			uint32_t l_maxHeight = l_font->GetMaxHeight();

			for ( ProceduralTextures::StringUtils::Utf8Iterator l_it = ProceduralTextures::StringUtils::Utf8Iterator( m_strText.begin() ); l_it != ProceduralTextures::StringUtils::Utf8Iterator( m_strText.end() ); ++l_it )
			{
				char const & l_character = *l_it;
				ProceduralTextures::Glyph const & l_glyph = l_font->GetGlyphAt( l_character );
				ProceduralTextures::Size const & l_sizeChar = l_glyph.GetSize();
				ProceduralTextures::Position const & l_ptCharPos = l_glyph.GetPosition();
				std::vector< ProceduralTextures::UbPixel > const & l_bitmap = l_glyph.GetBitmap();
				uint32_t l_charWidth = std::max( l_glyph.GetSize().x(), l_glyph.GetAdvance().x() );
				size_t l_uiIndex = l_bitmap.size() - l_sizeChar.x();

				if ( l_character == _T( '\n' ) )
				{
					l_ptPos = ProceduralTextures::Position( 0, l_ptPos.y() + l_maxHeight );
				}
				else if ( l_character == _T( '\r' ) )
				{
					l_ptPos = ProceduralTextures::Position( 0, l_ptPos.y() );
				}
				else if ( l_character == _T( ' ' ) )
				{
					l_ptPos += ProceduralTextures::Position( l_charWidth, 0 );
				}
				else
				{
					if ( l_ptPos.x() + l_ptCharPos.x() + l_sizeChar.x() >= m_sizeImage.x() )
					{
						l_ptPos = ProceduralTextures::Position( 0, l_ptPos.y() + l_maxHeight );
					}

					if ( l_ptPos.y() + l_ptCharPos.y() + l_sizeChar.y() < m_sizeImage.y() )
					{
						for ( uint32_t i = 0; i < l_sizeChar.y(); i++ )
						{
							ProceduralTextures::UbPixel const * l_lineStart = &l_bitmap[l_uiIndex];
							size_t l_lineIndex = 0;

							for ( uint32_t j = 0; j < l_sizeChar.x(); j++ )
							{
								ProceduralTextures::UbPixel const & l_r = l_lineStart[l_lineIndex++];

								if ( l_r.a )
								{
									if ( l_ptPos.x() + j + l_ptCharPos.x() >= 0 )
									{
										m_textureTextMask->GetBuffer()[( m_sizeImage.y() - 1 ) - ( ( l_maxHeight - l_ptCharPos.y() ) + l_ptPos.y() + i )][l_ptPos.x() + j + l_ptCharPos.x()].Set( l_r );
									}
									else
									{
										m_textureTextMask->GetBuffer()[( m_sizeImage.y() - 1 ) - ( ( l_maxHeight - l_ptCharPos.y() ) + l_ptPos.y() + i )][l_ptPos.x() + j].Set( l_r );
									}
								}
							}

							l_uiIndex -= l_sizeChar.x();
						}
					}

					l_ptPos.x() += l_sizeChar.x() + l_ptCharPos.x();
				}
			}

			m_textureTextMask->Activate();
			m_textureTextMask->UploadSync();
			m_textureTextMask->Deactivate();
			m_bTextModified = false;
		}
	}
}
