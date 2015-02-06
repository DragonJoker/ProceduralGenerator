#include "GlShaderObject.h"
#include "GlShaderProgram.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	GLenum GlShaderObject::ShaderTypes[eSHADER_OBJECT_TYPE_COUNT] =
	{
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
	};

	GlShaderObject::GlShaderObject( OpenGl * p_pOpenGl, GlShaderProgram * p_pParent, eSHADER_OBJECT_TYPE p_eType )
		:	m_bCompiled( false )
		,	m_eType( p_eType )
		,	m_bError( false )
		,	m_pParent( p_pParent )
		,	m_shaderObject( GL_INVALID_INDEX )
		,	m_bAttached( false )
		,	m_pOpenGl( p_pOpenGl )
	{
	}

	GlShaderObject::~GlShaderObject()
	{
	}

	void GlShaderObject::SetFile( const wxString & p_filename )
	{
		m_bError = false;
		m_pathFile.clear();
		m_strSource.clear();

		if ( ! p_filename.empty() )
		{
			wxTextFile l_file( p_filename );
			l_file.Open();

			if ( ! l_file.Eof() )
			{
				m_pathFile = p_filename;

				for ( size_t i = 0 ; i < l_file.GetLineCount() ; i++ )
				{
					m_strSource += l_file.GetLine( i ) + wxT( "\n" );
				}
			}
		}
	}

	void GlShaderObject::SetSource( const wxString & p_strSource )
	{
		m_bError = false;
		m_strSource = p_strSource;
	}

	void GlShaderObject::CreateProgram()
	{
		m_shaderObject = m_pOpenGl->CreateShader( ShaderTypes[m_eType] );
	}

	void GlShaderObject::DestroyProgram()
	{
		Detach();

		if ( m_bCompiled && m_shaderObject != GL_INVALID_INDEX )
		{
			m_pOpenGl->DeleteShader( m_shaderObject );
			m_shaderObject = GL_INVALID_INDEX;
			m_bCompiled = false;
		}
	}

	bool GlShaderObject::Compile()
	{
		bool l_bReturn = false;

		if ( ! m_bError && ! m_strSource.empty() )
		{
			m_bCompiled = false;
			GLint l_iCompiled = 0;
			GLint l_iLength = GLint( m_strSource.size() );
			char * l_pTmp = new char[l_iLength + 1];
			memcpy( l_pTmp, m_strSource.char_str(), l_iLength );
			l_pTmp[l_iLength] = 0;
			m_pOpenGl->ShaderSource( m_shaderObject, 1, const_cast< const char ** >( & l_pTmp ), & l_iLength );
			m_pOpenGl->CompileShader( m_shaderObject );
			m_pOpenGl->GetShaderiv( m_shaderObject, GL_COMPILE_STATUS, & l_iCompiled );
			delete [] l_pTmp;

			if ( l_iCompiled != 0 )
			{
				m_bCompiled = true;
			}
			else
			{
				m_bError = true;
			}

			m_compilerLog = RetrieveCompilerLog();

			if ( m_bCompiled )
			{
				m_compilerLog.clear();
			}

			l_bReturn = m_bCompiled;
		}

		return l_bReturn;
	}

	void GlShaderObject::AttachTo( GlShaderProgram * p_pProgram )
	{
		if ( m_bCompiled && ! m_bError )
		{
			m_pParent = p_pProgram;
			m_pOpenGl->AttachShader( m_pParent->GetProgramObject(), m_shaderObject );
			m_bAttached = true;
		}
	}

	void GlShaderObject::Detach()
	{
		if ( m_bCompiled && m_pParent != NULL && ! m_bError && m_bAttached )
		{
			try
			{
				m_pOpenGl->DetachShader( m_pParent->GetProgramObject(), m_shaderObject );
				m_bAttached = false;
			}
			catch ( ... )
			{
//				wxMessageBox( wxT( "GlShaderObject::Detach - glDetachShader - Exception"));
			}

			m_pParent = NULL;
			// if you get an error here, you deleted the Program object first and then
			// the ShaderObject! Always delete ShaderObjects last!
		}
	}

	wxString GlShaderObject::RetrieveCompilerLog()
	{
		wxString l_log;
		int l_iInfologLength = 0;
		int charsWritten  = 0;
		m_pOpenGl->GetShaderiv( m_shaderObject, GL_INFO_LOG_LENGTH, & l_iInfologLength );

		if ( l_iInfologLength > 0 )
		{
			char * l_szInfoLog = new char[l_iInfologLength];
			m_pOpenGl->GetShaderInfoLog( m_shaderObject, l_iInfologLength, & charsWritten, l_szInfoLog );
			l_log = wxString( l_szInfoLog, wxConvLibc );
			delete [] l_szInfoLog;
		}

		if ( !l_log.empty() )
		{
			l_log = l_log.substr( 0, l_log.size() - 1 );
		}

		return l_log;
	}
}
