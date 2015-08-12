#include "GlShaderObject.h"
#include "GlShaderProgram.h"
#include "OpenGl.h"
#include "GeneratorUtils.h"

#include <fstream>

namespace ProceduralTextures
{
	namespace gl
	{
		GLenum ShaderObject::ShaderTypes[eSHADER_OBJECT_TYPE_COUNT] =
		{
			GL_VERTEX_SHADER,
			GL_FRAGMENT_SHADER,
		};

		ShaderObject::ShaderObject( std::shared_ptr< OpenGl > p_pOpenGl, eSHADER_OBJECT_TYPE p_eType )
			: Object( p_pOpenGl,
					  std::bind( &OpenGl::CreateShader, p_pOpenGl, ShaderTypes[p_eType] ),
					  std::bind( &OpenGl::DeleteShader, p_pOpenGl, std::placeholders::_1 ),
					  std::bind( &OpenGl::IsShader, p_pOpenGl, std::placeholders::_1 )
					)
			, m_bCompiled( false )
			, m_eType( p_eType )
			, m_bError( false )
			, m_bAttached( false )
		{
		}

		ShaderObject::~ShaderObject()
		{
		}

		void ShaderObject::SetFile( String const & p_filename )
		{
			m_bError = false;
			m_pathFile.clear();
			m_strSource.clear();

			if ( !p_filename.empty() )
			{
				std::basic_ifstream< char > l_file;
				l_file.open( StringUtils::ToStdString( p_filename ).c_str() );

				if ( l_file.is_open() )
				{
					m_pathFile = p_filename;

					while ( !l_file.eof() )
					{
						String l_line;
						std::getline( l_file, l_line );
						m_strSource += l_line + _T( "\n" );
					}
				}
			}
		}

		void ShaderObject::SetSource( String const & p_strSource )
		{
			m_bError = false;
			m_strSource = p_strSource;
		}

		void ShaderObject::Destroy()
		{
			Detach();

			if ( m_bCompiled && IsValid() )
			{
				ParentClass::Destroy();
				m_bCompiled = false;
			}
		}

		bool ShaderObject::Compile()
		{
			bool l_bReturn = false;

			if ( !m_bError && !m_strSource.empty() )
			{
				m_bCompiled = false;
				GLint l_iCompiled = 0;
				GLint l_iLength = GLint( m_strSource.size() );
				std::string l_tmp = StringUtils::ToStdString( m_strSource ).c_str();
				char const * l_pTmp = l_tmp.c_str();
				GetOpenGl()->ShaderSource( GetGlName(), 1, &l_pTmp, &l_iLength );
				GetOpenGl()->CompileShader( GetGlName() );
				GetOpenGl()->GetShaderParameter( GetGlName(), GL_COMPILE_STATUS, &l_iCompiled );

				if ( l_iCompiled )
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
				else
				{
					std::cerr << m_compilerLog << std::endl;
				}

				l_bReturn = m_bCompiled;
			}

			return l_bReturn;
		}

		void ShaderObject::AttachTo( std::shared_ptr< ShaderProgram > p_pProgram )
		{
			if ( m_bCompiled && !m_bError )
			{
				m_parent = p_pProgram;
				GetOpenGl()->AttachShader( GetParent()->GetGlName(), GetGlName() );
				m_bAttached = true;
			}
		}

		void ShaderObject::Detach()
		{
			std::shared_ptr< ShaderProgram > l_parent = GetParent();

			if ( m_bCompiled && l_parent && !m_bError && m_bAttached )
			{
				try
				{
					GetOpenGl()->DetachShader( l_parent->GetGlName(), GetGlName() );
					m_bAttached = false;
				}
				catch ( ... )
				{
					std::cerr << "GlShaderObject::Detach - glDetachShader - Exception" << std::endl;
				}

				m_parent.reset();
				// if you get an error here, you deleted the Program object first and then
				// the ShaderObject! Always delete ShaderObjects last!
			}
		}

		String ShaderObject::RetrieveCompilerLog()
		{
			String l_log;
			int l_iInfologLength = 0;
			int charsWritten  = 0;
			GetOpenGl()->GetShaderParameter( GetGlName(), GL_INFO_LOG_LENGTH, &l_iInfologLength );

			if ( l_iInfologLength > 0 )
			{
				std::vector< char > l_szInfoLog( l_iInfologLength + 1 );
				GetOpenGl()->GetShaderInfoLog( GetGlName(), l_iInfologLength, &charsWritten, l_szInfoLog.data() );
				l_log = StringUtils::FromStdString( std::string( l_szInfoLog.data() ) );
			}

			if ( !l_log.empty() )
			{
				l_log = l_log.substr( 0, l_log.size() - 1 );
			}

			return l_log;
		}
	}
}
