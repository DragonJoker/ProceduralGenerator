#include "SfxEffect.h"

#include <GlFrameVariable.h>
#include <GlShaderObject.h>
#include <GlShaderProgram.h>
#include <Buffer.h>

using namespace ProceduralTextures;

namespace ShaderEffects
{
	Effect::Effect( gl::OpenGl & p_openGl, std::shared_ptr< PixelBuffer > p_buffer )
		: gl::Holder( p_openGl )
		, m_strVertexFile()
		, m_strFragmentFile()
		, m_bNewShader( false )
		, m_size( p_buffer->GetDimensions() )
		, m_buffer( p_buffer )
		, m_vertexAttribLocation( GL_INVALID_INDEX )
		, m_textureAttribLocation( GL_INVALID_INDEX )
	{
	}

	Effect::~Effect()
	{
		m_pShaderProgram.reset();
	}

	void Effect::SetVertexFile( String const & p_strPath )
	{
		m_strVertexFile = p_strPath;
	}

	void Effect::SetFragmentFile( String const & p_strPath )
	{
		m_strFragmentFile = p_strPath;
	}

	void Effect::Compile()
	{
		m_bNewShader = true;
	}

	void Effect::Initialise()
	{
		if ( m_bNewShader )
		{
			if ( m_pShaderProgram )
			{
				m_pShaderProgram->Cleanup();
			}
			else
			{
				m_pShaderProgram = std::make_shared< gl::ShaderProgram >( GetOpenGl() );
			}

			m_pShaderProgram->CreateObject( gl::eSHADER_OBJECT_TYPE_VERTEX );
			m_pShaderProgram->CreateObject( gl::eSHADER_OBJECT_TYPE_PIXEL );

			if ( !m_strVertexFile.empty() )
			{
				m_pShaderProgram->SetProgramFile( m_strVertexFile, gl::eSHADER_OBJECT_TYPE_VERTEX );
			}

			if ( !m_strFragmentFile.empty() )
			{
				m_pShaderProgram->SetProgramFile( m_strFragmentFile, gl::eSHADER_OBJECT_TYPE_PIXEL );
			}

			m_pShaderProgram->Initialise();
			m_vertexAttribLocation = m_pShaderProgram->GetAttributeLocation( _T( "vertex" ) );
			m_textureAttribLocation = m_pShaderProgram->GetAttributeLocation( _T( "texture" ) );
			m_uniformWidth = m_pShaderProgram->CreateIntFrameVariable( _T( "pg_width" ) );
			m_uniformHeight = m_pShaderProgram->CreateIntFrameVariable( _T( "pg_height" ) );
			m_uniformTime = m_pShaderProgram->CreateIntFrameVariable( _T( "pg_time" ) );
			m_uniformSepType = m_pShaderProgram->CreateIntFrameVariable( _T( "pg_sep_type" ) );
			m_uniformSepOffset = m_pShaderProgram->CreateIntFrameVariable( _T( "pg_sep_offset" ) );
			m_pShaderProgram->CreateIntFrameVariable( _T( "pg_texture" ) );
			m_pShaderProgram->CreateIntFrameVariable( _T( "pg_modif1" ) )->SetValue( 1 );
			m_pShaderProgram->CreateIntFrameVariable( _T( "pg_modif2" ) )->SetValue( 2 );
			m_pShaderProgram->CreateIntFrameVariable( _T( "pg_modif3" ) )->SetValue( 3 );
			m_llStartTime = Clock::now();
			m_bNewShader = false;
		}
	}

	void Effect::Cleanup()
	{
		if ( m_pShaderProgram )
		{
			m_pShaderProgram->Cleanup();
		}
	}

	bool Effect::Activate( eSEPARATION p_eSeparationType, int p_iSeparationOffset )
	{
		bool l_bReturn = true;

		if ( m_pShaderProgram )
		{
			m_uniformWidth.lock()->SetValue( m_size.x() );
			m_uniformHeight.lock()->SetValue( m_size.y() );
			m_uniformSepType.lock()->SetValue( int( p_eSeparationType ) );
			m_uniformSepOffset.lock()->SetValue( p_iSeparationOffset );
			m_uniformTime.lock()->SetValue( int( std::chrono::duration_cast< std::chrono::milliseconds >( Clock::now() - m_llStartTime ).count() ) );
			m_pShaderProgram->Activate();
		}

		return l_bReturn;
	}

	void Effect::Deactivate()
	{
		if ( m_pShaderProgram )
		{
			m_pShaderProgram->Deactivate();
		}
	}

	String Effect::GetCompilerLog( gl::eSHADER_OBJECT_TYPE p_eType )
	{
		String l_strReturn;

		if ( m_pShaderProgram )
		{
			l_strReturn = m_pShaderProgram->GetCompilerLog( p_eType );
		}

		return l_strReturn;
	}

	String Effect::GetLinkerLog()
	{
		String l_strReturn;

		if ( m_pShaderProgram )
		{
			l_strReturn = m_pShaderProgram->GetLinkerLog();
		}

		return l_strReturn;
	}

	bool Effect::IsInitialised()const
	{
		return m_pShaderProgram && m_pShaderProgram->IsLinked() && !m_bNewShader;
	}

	void Effect::UpdateImages()
	{
		m_size = m_buffer.lock()->GetDimensions();
	}
}
