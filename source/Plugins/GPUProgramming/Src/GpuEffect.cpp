#include "GpuEffect.h"

#include <GlFrameVariable.h>
#include <GlVec2FrameVariable.h>
#include <GlShaderObject.h>
#include <GlShaderProgram.h>
#include <Position.h>

#include <cmath>

using namespace ProceduralTextures;

namespace GpuProgramming
{
	static const String VertexProgram =
		_T( "attribute vec2 vertex;" )
		_T( "attribute vec2 texture;" )
		_T( "varying vec2 pxl_texture;" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "	pxl_texture = texture;\n" )
		_T( "	gl_Position = vec4( vertex, 0.0, 1.0 );\n" )
		_T( "}\n" );

	Effect::Effect( std::shared_ptr< gl::OpenGl > p_pOpenGl, int p_iWidth, int p_iHeight )
		: gl::Holder( p_pOpenGl )
		, m_strVertexFile()
		, m_strFragmentFile()
		, m_bNewShader( false )
		, m_iWidth( p_iWidth )
		, m_iHeight( p_iHeight )
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
			else
			{
				m_pShaderProgram->SetProgramText( VertexProgram, gl::eSHADER_OBJECT_TYPE_VERTEX );
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
			m_uniformMouse = m_pShaderProgram->CreateVec2IntFrameVariable( _T( "pg_mouse" ) );
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

	bool Effect::Activate( Position const & p_ptMousePosition )
	{
		bool l_bReturn = true;

		if ( m_pShaderProgram )
		{
			m_uniformWidth.lock()->SetValue( m_iWidth );
			m_uniformHeight.lock()->SetValue( m_iHeight );
			m_uniformMouse.lock()->SetValue( std::max( 0, std::min( p_ptMousePosition.x(), m_iWidth ) ), std::max( 0, std::min( p_ptMousePosition.y(), m_iHeight ) ) );
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
}
