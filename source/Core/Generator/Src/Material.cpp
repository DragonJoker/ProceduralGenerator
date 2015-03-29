#include "Material.h"

#include "GlFrameVariable.h"
#include "GlShaderProgram.h"
#include "GlTexture.h"
#include "GlVec4FrameVariable.h"

namespace ProceduralTextures
{
	Material::Material()
		: m_type( eMATERIAL_TYPE_COLOUR )
		, m_vertexAttribute( GL_INVALID_INDEX )
		, m_textureAttribute( GL_INVALID_INDEX )
	{
	}

	Material::Material( Colour const & p_colour )
		: m_colour( p_colour )
		, m_type( eMATERIAL_TYPE_COLOUR )
		, m_vertexAttribute( GL_INVALID_INDEX )
		, m_textureAttribute( GL_INVALID_INDEX )
	{
	}

	Material::Material( std::shared_ptr< gl::Texture > p_texture )
		: m_texture( p_texture )
		, m_type( eMATERIAL_TYPE_TEXTURE )
		, m_vertexAttribute( GL_INVALID_INDEX )
		, m_textureAttribute( GL_INVALID_INDEX )
	{
	}

	void Material::Activate()
	{
		std::shared_ptr< gl::ShaderProgram > l_program = m_program.lock();

		if ( l_program )
		{
			if ( m_vertexAttribute == GL_INVALID_INDEX )
			{
				m_vertexAttribute = l_program->GetAttributeLocation( _T( "vertex" ) );
				m_textureAttribute = l_program->GetAttributeLocation( _T( "texture" ) );
			}

			if ( m_type == eMATERIAL_TYPE_TEXTURE )
			{
				m_uniformTexture.lock()->SetValue( 0 );
				l_program->Activate();
				m_texture->Activate( GL_TEXTURE0 );
			}
			else
			{
				m_uniformColour.lock()->SetValue( m_colour.r, m_colour.g, m_colour.b, m_colour.a );
				l_program->Activate();
			}
		}
	}

	void Material::Deactivate()
	{
		std::shared_ptr< gl::ShaderProgram > l_program = m_program.lock();

		if ( l_program )
		{
			if ( m_type == eMATERIAL_TYPE_TEXTURE )
			{
				m_texture->Deactivate( GL_TEXTURE0 );
				l_program->Deactivate();
			}
			else
			{
				l_program->Deactivate();
			}
		}
	}

	void Material::SetType( eMATERIAL_TYPE p_type, std::shared_ptr< gl::ShaderProgram > p_program )
	{
		m_type = p_type;
		m_program = p_program;
		m_vertexAttribute = p_program->GetAttributeLocation( _T( "vertex" ) );
		m_textureAttribute = p_program->GetAttributeLocation( _T( "texture" ) );
		m_uniformColour.reset();
		m_uniformTexture.reset();

		switch ( p_type )
		{
		case eMATERIAL_TYPE_COLOUR:
			m_uniformColour = p_program->GetVec4FloatFrameVariable( _T( "pxl_colour" ) );
			break;

		case eMATERIAL_TYPE_TEXTURE:
			m_uniformTexture = p_program->GetIntFrameVariable( _T( "pxl_mapTexture" ) );
			break;
		}
	}

	void Material::SetColour( Colour const & p_colour )
	{
		m_colour = p_colour;
		std::shared_ptr< gl::Vec4FrameVariable< float > > l_uniform = m_uniformColour.lock();

		if ( l_uniform )
		{
			l_uniform->SetValue( m_colour.r, m_colour.g, m_colour.b, m_colour.a );
		}
	}

	void Material::SetTexture( std::shared_ptr< gl::Texture > p_texture )
	{
		m_texture = p_texture;
	}
}
