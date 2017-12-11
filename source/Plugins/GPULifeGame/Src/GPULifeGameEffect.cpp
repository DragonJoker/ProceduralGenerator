#include "GpuLifeGameEffect.h"

#include <GlFrameVariable.h>
#include <GlVec2FrameVariable.h>
#include <GlShaderObject.h>
#include <GlShaderProgram.h>
#include <Position.h>

#include <cmath>

using namespace ProceduralTextures;

namespace GpuLifeGame
{
	static const String VertexProgram =
		_T( "attribute vec2 vertex;\n" )
		_T( "attribute vec2 texture;\n" )
		_T( "varying vec2 pxl_texture;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "	pxl_texture = texture;\n" )
		_T( "	gl_Position = vec4( vertex, 0.0, 1.0 );\n" )
		_T( "}\n" );

	static const String FragmentProgram =
		_T( "uniform int pg_width;\n" )
		_T( "uniform int pg_height;\n" )
		_T( "\n" )
		_T( "varying vec2 pxl_texture;\n" )
		_T( "\n" )
		_T( "uniform sampler2D pg_texture;\n" )
		_T( "\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "	float stepX = 1.0 / float( pg_width );\n" )
		_T( "	float stepY = 1.0 / float( pg_height );\n" )
		_T( "	float neighbours\n" )
		_T( "		= texture2D( pg_texture, pxl_texture + vec2( stepX, -stepY ) ).x\n" )
		_T( "		+ texture2D( pg_texture, pxl_texture + vec2( stepX, 0.0 ) ).x\n" )
		_T( "		+ texture2D( pg_texture, pxl_texture + vec2( stepX, stepY ) ).x\n" )
		_T( "		+ texture2D( pg_texture, pxl_texture + vec2( -stepX, -stepY ) ).x\n" )
		_T( "		+ texture2D( pg_texture, pxl_texture + vec2( -stepX, 0.0 ) ).x\n" )
		_T( "		+ texture2D( pg_texture, pxl_texture + vec2( -stepX, stepY ) ).x\n" )
		_T( "		+ texture2D( pg_texture, pxl_texture + vec2( 0.0, -stepY ) ).x\n" )
		_T( "		+ texture2D( pg_texture, pxl_texture + vec2( 0.0, stepY ) ).x;\n" )
		_T( "\n" )
		_T( "	if ( neighbours == 3.0 )\n" )
		_T( "	{\n" )
		_T( "		gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );\n" )
		_T( "	}\n" )
		_T( "	else if ( neighbours != 2.0 )\n" )
		_T( "	{\n" )
		_T( "		gl_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );\n" )
		_T( "	}\n" )
		_T( "	else\n" )
		_T( "	{\n" )
		_T( "		gl_FragColor = texture2D( pg_texture, pxl_texture );\n" )
		_T( "	}\n" )
		_T( "}\n" );

	Effect::Effect( gl::OpenGl & p_openGl, int p_iWidth, int p_iHeight )
		: gl::Holder( p_openGl )
		, m_iWidth( p_iWidth )
		, m_iHeight( p_iHeight )
		, m_vertexAttribLocation( GL_INVALID_INDEX )
		, m_textureAttribLocation( GL_INVALID_INDEX )
	{
		Compile();
	}

	Effect::~Effect()
	{
		m_pShaderProgram.reset();
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
			m_pShaderProgram->SetProgramText( VertexProgram, gl::eSHADER_OBJECT_TYPE_VERTEX );
			m_pShaderProgram->SetProgramText( FragmentProgram, gl::eSHADER_OBJECT_TYPE_PIXEL );

			m_pShaderProgram->Initialise();
			m_vertexAttribLocation = m_pShaderProgram->GetAttributeLocation( _T( "vertex" ) );
			m_textureAttribLocation = m_pShaderProgram->GetAttributeLocation( _T( "texture" ) );
			m_uniformWidth = m_pShaderProgram->CreateIntFrameVariable( _T( "pg_width" ) );
			m_uniformHeight = m_pShaderProgram->CreateIntFrameVariable( _T( "pg_height" ) );
			m_uniformTime = m_pShaderProgram->CreateIntFrameVariable( _T( "pg_time" ) );
			m_pShaderProgram->CreateIntFrameVariable( _T( "pg_texture" ) );
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
