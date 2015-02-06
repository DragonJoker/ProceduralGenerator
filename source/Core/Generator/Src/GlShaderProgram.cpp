#include "GlShaderProgram.h"
#include "GlShaderObject.h"
#include "GlFrameVariable.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	GlShaderProgram::GlShaderProgram( OpenGl * p_pOpenGl, const wxArrayString & p_shaderFiles )
		:	m_bLinked( false )
		,	m_bError( false )
		,	m_programObject( 0 )
		,	m_pOpenGl( p_pOpenGl )
	{
		for ( int i = eSHADER_OBJECT_TYPE_VERTEX ; i < eSHADER_OBJECT_TYPE_COUNT && i < static_cast< int >( p_shaderFiles.size() ) ; i++ )
		{
			if ( ! p_shaderFiles[i].empty() )
			{
				CreateObject( eSHADER_OBJECT_TYPE( i ) );
				SetProgramFile( p_shaderFiles[i], eSHADER_OBJECT_TYPE( i ) );
			}
		}
	}

	GlShaderProgram::GlShaderProgram( OpenGl * p_pOpenGl )
		:	m_bLinked( false )
		,	m_bError( false )
		,	m_programObject( 0 )
		,	m_pOpenGl( p_pOpenGl )
	{
		for ( int i = eSHADER_OBJECT_TYPE_VERTEX ; i < eSHADER_OBJECT_TYPE_COUNT ; i++ )
		{
			m_pShaders[i] = NULL;
		}
	}

	GlShaderProgram::~GlShaderProgram()
	{
		for ( int i = eSHADER_OBJECT_TYPE_VERTEX ; i < eSHADER_OBJECT_TYPE_COUNT ; i++ )
		{
			if ( m_pShaders[i] )
			{
				delete m_pShaders[i];
				m_pShaders[i] = NULL;
			}
		}

		for ( FrameVariablePtrList::iterator it = m_listFrameVariables.begin() ; it != m_listFrameVariables.end() ; ++it )
		{
			delete( * it );
		}

		m_listFrameVariables.clear();
	}

	bool GlShaderProgram::Initialise()
	{
		bool l_bReturn = false;

		if ( ! m_bLinked && ! m_bError )
		{
			Cleanup();
			m_programObject = m_pOpenGl->CreateProgram();

			for ( int i = eSHADER_OBJECT_TYPE_VERTEX ; i < eSHADER_OBJECT_TYPE_COUNT ; i++ )
			{
				if ( m_pShaders[i] && ! m_pShaders[i]->GetSource().empty() )
				{
					m_pShaders[i]->CreateProgram();
				}
			}

			for ( int i = eSHADER_OBJECT_TYPE_VERTEX ; i < eSHADER_OBJECT_TYPE_COUNT ; i++ )
			{
				if ( m_pShaders[i] && ! m_pShaders[i]->GetSource().empty() )
				{
					if ( ! m_pShaders[i]->Compile() )
					{
						Cleanup();
						m_bError = true;
						return false;
					}
				}
			}

			if ( ! Link() )
			{
				Cleanup();
				m_bError = true;
				return false;
			}

			l_bReturn = true;
		}

		return l_bReturn;
	}

	void GlShaderProgram::Cleanup()
	{
		for ( FrameVariablePtrList::iterator it = m_listFrameVariables.begin() ; it != m_listFrameVariables.end() ; ++it )
		{
			delete( * it );
		}

		m_listFrameVariables.clear();

		for ( int i = eSHADER_OBJECT_TYPE_VERTEX ; i < eSHADER_OBJECT_TYPE_COUNT ; i++ )
		{
			if ( m_pShaders[i] && ! m_pShaders[i]->GetSource().empty() )
			{
				m_pShaders[i]->DestroyProgram();
			}
		}

		if ( m_programObject != 0 )
		{
			m_pOpenGl->DeleteProgram( m_programObject );
		}
	}

	void GlShaderProgram::CreateObject( eSHADER_OBJECT_TYPE p_eType )
	{
		if ( !m_pShaders[p_eType] )
		{
			m_pShaders[p_eType] = new GlShaderObject( m_pOpenGl, this, p_eType );
		}
	}

	void GlShaderProgram::DestroyObject( eSHADER_OBJECT_TYPE p_eType )
	{
		if ( m_pShaders[p_eType] )
		{
			m_pShaders[p_eType]->DestroyProgram();
			delete m_pShaders[p_eType];
			m_pShaders[p_eType] = NULL;
		}
	}

	bool GlShaderProgram::Link()
	{
		bool l_bReturn = false;

		if ( ! m_bError && m_pOpenGl->IsProgram( m_programObject ) )
		{
			for ( int i = eSHADER_OBJECT_TYPE_VERTEX ; i < eSHADER_OBJECT_TYPE_COUNT ; i++ )
			{
				if ( m_pShaders[i] != NULL )
				{
					m_pShaders[i]->AttachTo( this );
				}
			}

			GLint l_iLinked = 0;
			GLint l_iValidated = 0;
			m_pOpenGl->LinkProgram( m_programObject );
			m_pOpenGl->GetProgramiv( m_programObject, GL_LINK_STATUS, & l_iLinked );
			m_pOpenGl->GetProgramiv( m_programObject, GL_VALIDATE_STATUS, & l_iValidated );
			m_linkerLog = RetrieveLinkerLog();

			if ( l_iLinked && l_iValidated && m_linkerLog.find( wxT( "ERROR" ) ) == wxString::npos )
			{
				m_bLinked = true;
				m_linkerLog.clear();
			}

			l_bReturn = m_bLinked;
		}

		return l_bReturn;
	}

	wxString GlShaderProgram::RetrieveLinkerLog()
	{
		wxString l_log;
		GLint l_iLength = 0;
		GLsizei l_uiLength = 0;

		if ( m_programObject != 0 )
		{
			m_pOpenGl->GetProgramiv( m_programObject, GL_INFO_LOG_LENGTH , & l_iLength );

			if ( l_iLength > 1 )
			{
				char * l_pTmp = new char[l_iLength];
				m_pOpenGl->GetProgramInfoLog( m_programObject, l_iLength, & l_uiLength, l_pTmp );
				l_log = wxString( l_pTmp, wxConvLibc );
				delete [] l_pTmp;
			}
		}

		return l_log;
	}

	bool GlShaderProgram::Activate()
	{
		bool l_bReturn = true;

		if ( m_programObject != 0 && m_bLinked && ! m_bError )
		{
			if ( m_pOpenGl->UseProgram( m_programObject ) )
			{
				l_bReturn = ApplyAllVariables();
			}
		}

		return l_bReturn;
	}

	void GlShaderProgram::Deactivate()
	{
		if ( m_programObject != 0 && m_bLinked && ! m_bError )
		{
			m_pOpenGl->UseProgram( 0 );
		}
	}

	void GlShaderProgram::SetProgramFile( const wxString & p_strFile, eSHADER_OBJECT_TYPE p_eType )
	{
		if ( m_pShaders[p_eType] != NULL )
		{
			m_pShaders[p_eType]->SetFile( p_strFile );
			ResetToCompile();
		}
	}

	GlShaderObject * GlShaderProgram::GetObject( eSHADER_OBJECT_TYPE p_eType )
	{
		return m_pShaders[p_eType];
	}

	int GlShaderProgram::GetAttributeLocation( const wxString & p_strName )const
	{
		int l_iReturn = GL_INVALID_INDEX;

		if ( m_programObject != GL_INVALID_INDEX && m_pOpenGl->IsProgram( m_programObject ) )
		{
			l_iReturn = m_pOpenGl->GetAttribLocation( m_programObject, p_strName.char_str() );
		}

		return l_iReturn;
	}

	void GlShaderProgram::SetProgramText( const wxString & p_strFile, eSHADER_OBJECT_TYPE p_eType )
	{
		if ( m_pShaders[p_eType] != NULL )
		{
			m_pShaders[p_eType]->SetFile( wxEmptyString );
			m_pShaders[p_eType]->SetSource( p_strFile );
			ResetToCompile();
		}
	}

	void GlShaderProgram::ResetToCompile()
	{
		m_bLinked = false;
		m_bError = false;
	}

	bool GlShaderProgram::ApplyAllVariables()
	{
		bool l_bReturn = true;

		for ( FrameVariablePtrList::iterator it = m_listFrameVariables.begin() ; it != m_listFrameVariables.end() && l_bReturn ; ++it )
		{
			l_bReturn = ( * it )->Apply();
		}

		return l_bReturn;
	}

	GlFrameVariable< float > * GlShaderProgram::CreateFloatFrameVariable( const wxString & p_strName )
	{
		GlFrameVariable< float > * l_pReturn = new GlFrameVariable< float >( m_pOpenGl, this );
		l_pReturn->SetName( p_strName );
		m_listFrameVariables.push_back( l_pReturn );
		return l_pReturn;
	}

	GlFrameVariable< int > * GlShaderProgram::CreateIntFrameVariable( const wxString & p_strName )
	{
		GlFrameVariable< int > * l_pReturn = new GlFrameVariable< int >( m_pOpenGl, this );
		l_pReturn->SetName( p_strName );
		m_listFrameVariables.push_back( l_pReturn );
		return l_pReturn;
	}
}
