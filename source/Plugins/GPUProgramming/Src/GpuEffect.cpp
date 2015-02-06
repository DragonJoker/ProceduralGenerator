#include "GpuEffect.h"

#include <GlShaderProgram.h>
#include <GlShaderObject.h>
#include <GlFrameVariable.h>

#include <cmath>

namespace ProceduralTextures
{
	static const wxString VertexProgram =
		wxT( "void main()\n" )
		wxT( "{\n" )
		wxT( "	gl_TexCoord[0] = gl_MultiTexCoord0;\n" )
		wxT( "	gl_Position = ftransform();\n" )
		wxT( "}\n" );

	Effect::Effect( OpenGl * p_pOpenGl, int p_iWidth, int p_iHeight )
		:	m_pShaderProgram( NULL )
		,	m_strVertexFile()
		,	m_strFragmentFile()
		,	m_bNewShader( false )
		,	m_llStartTime( 0 )
		,	m_iWidth( p_iWidth )
		,	m_iHeight( p_iHeight )
		,	m_pOpenGl( p_pOpenGl )
	{
	}

	Effect::~Effect()
	{
		delete m_pShaderProgram;
	}

	void Effect::SetVertexFile( const wxString & p_strPath )
	{
		m_strVertexFile = p_strPath;
	}

	void Effect::SetFragmentFile( const wxString & p_strPath )
	{
		m_strFragmentFile = p_strPath;
	}

	void Effect::Compile()
	{
		m_bNewShader = true;
	}

	void Effect::Initialise()
	{
		if ( m_pShaderProgram )
		{
			m_pShaderProgram->Cleanup();
		}
		else
		{
			m_pShaderProgram = new GlShaderProgram( m_pOpenGl );
		}

		m_pShaderProgram->CreateObject( eSHADER_OBJECT_TYPE_VERTEX );

		if ( !m_strVertexFile.empty() )
		{
			m_pShaderProgram->SetProgramFile( m_strVertexFile, eSHADER_OBJECT_TYPE_VERTEX );
		}
		else
		{
			m_pShaderProgram->SetProgramText( VertexProgram, eSHADER_OBJECT_TYPE_VERTEX );
		}

		if ( !m_strFragmentFile.empty() )
		{
			m_pShaderProgram->CreateObject( eSHADER_OBJECT_TYPE_PIXEL );
			m_pShaderProgram->SetProgramFile( m_strFragmentFile, eSHADER_OBJECT_TYPE_PIXEL );
		}

		m_pShaderProgram->Initialise();
		m_uniformWidth = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_width" ) );
		m_uniformHeight = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_height" ) );
		m_uniformTime = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_time" ) );
		m_uniformMouseX = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_mousex" ) );
		m_uniformMouseY = m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_mousey" ) );
		m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_texture" ) );
		m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_modif1" ) )->SetValue( 1 );
		m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_modif2" ) )->SetValue( 2 );
		m_pShaderProgram->CreateIntFrameVariable( wxT( "pg_modif3" ) )->SetValue( 3 );
		m_llStartTime = wxGetLocalTimeMillis();
		m_bNewShader = false;
	}

	void Effect::Cleanup()
	{
		if ( m_pShaderProgram )
		{
			m_pShaderProgram->Cleanup();
		}
	}

	bool Effect::Activate( wxPoint const & p_ptMousePosition )
	{
		bool l_bReturn = true;

		if ( m_pShaderProgram )
		{
			m_uniformWidth->SetValue( m_iWidth );
			m_uniformHeight->SetValue( m_iHeight );
			m_uniformMouseX->SetValue( std::max( 0, std::min( p_ptMousePosition.x, m_iWidth ) ) );
			m_uniformMouseY->SetValue( std::max( 0, std::min( p_ptMousePosition.y, m_iHeight ) ) );
			m_uniformTime->SetValue( int( ( wxGetLocalTimeMillis() - m_llStartTime ).ToLong() ) );
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

	wxString Effect::GetCompilerLog( eSHADER_OBJECT_TYPE p_eType )
	{
		wxString l_strReturn;

		if ( m_pShaderProgram )
		{
			GlShaderObject * l_pShader = m_pShaderProgram->GetObject( p_eType );

			if ( l_pShader )
			{
				l_strReturn = l_pShader->GetCompilerLog();
			}
		}

		return l_strReturn;
	}

	wxString Effect::GetLinkerLog()
	{
		wxString l_strReturn;

		if ( m_pShaderProgram )
		{
			l_strReturn = m_pShaderProgram->GetLinkerLog();
		}

		return l_strReturn;
	}
}
