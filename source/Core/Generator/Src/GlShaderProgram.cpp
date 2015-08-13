#include "GlShaderProgram.h"
#include "GlShaderObject.h"
#include "GlFrameVariable.h"
#include "GlVec2FrameVariable.h"
#include "GlVec3FrameVariable.h"
#include "GlVec4FrameVariable.h"
#include "GlMat4FrameVariable.h"
#include "OpenGl.h"
#include "GeneratorUtils.h"

namespace ProceduralTextures
{
	namespace gl
	{
		namespace
		{
			template< typename Variable >
			std::shared_ptr< Variable > CreateFrameVariable( String const & p_name, OpenGl & p_openGl, ShaderProgram & p_program, FrameVariablePtrList & p_list, std::map< String, std::weak_ptr< Variable > > & p_map )
			{
				std::shared_ptr< Variable > l_pReturn;
				auto l_it = p_map.find( p_name );

				if ( l_it == p_map.end() )
				{
					l_pReturn = std::make_shared< Variable >( p_openGl, p_program );
					l_pReturn->SetName( p_name );
					p_list.push_back( l_pReturn );
					p_map.insert( std::make_pair( p_name, l_pReturn ) );
				}
				else
				{
					l_pReturn = l_it->second.lock();
				}

				return l_pReturn;
			}
			template< typename Variable >
			std::shared_ptr< Variable > GetFrameVariable( String const & p_name, std::map< String, std::weak_ptr< Variable > > & p_map )
			{
				std::shared_ptr< Variable > l_pReturn;
				auto l_it = p_map.find( p_name );

				if ( l_it != p_map.end() )
				{
					l_pReturn = l_it->second.lock();
				}

				return l_pReturn;
			}
		}

		ShaderProgram::ShaderProgram( OpenGl & p_openGl )
			: Object( p_openGl,
					  std::bind( &OpenGl::CreateProgram, std::ref( p_openGl ) ),
					  std::bind( &OpenGl::DeleteProgram, std::ref( p_openGl ), std::placeholders::_1 ),
					  std::bind( &OpenGl::IsProgram, std::ref( p_openGl ), std::placeholders::_1 )
					)
			, m_bLinked( false )
			, m_bError( false )
			, m_shaders( eSHADER_OBJECT_TYPE_COUNT )
		{
		}

		ShaderProgram::~ShaderProgram()
		{
			m_shaders.clear();
			m_listFrameVariables.clear();
		}

		bool ShaderProgram::Initialise()
		{
			bool l_bReturn = false;

			if ( !m_bLinked && !m_bError )
			{
				Cleanup();
				Create();

				for ( auto l_shader : m_shaders )
				{
					if ( !m_bError && l_shader && !l_shader->GetSource().empty() )
					{
						l_shader->Create();

						if ( !l_shader->Compile() )
						{
							m_bError = true;
						}
					}
				}

				if ( m_bError || !Link() )
				{
					Cleanup();
					m_bError = true;
				}

				l_bReturn = !m_bError;
			}

			return l_bReturn;
		}

		void ShaderProgram::Cleanup()
		{
			for ( auto l_shader : m_shaders )
			{
				if ( l_shader && !l_shader->GetSource().empty() )
				{
					l_shader->Destroy();
				}
			}

			Destroy();
		}

		bool ShaderProgram::Bind()
		{
			return GetOpenGl().UseProgram( GetGlName() );
		}

		void ShaderProgram::Unbind()
		{
			GetOpenGl().UseProgram( 0 );
		}

		void ShaderProgram::CreateObject( eSHADER_OBJECT_TYPE p_eType )
		{
			if ( !m_shaders[p_eType] )
			{
				m_shaders[p_eType] = std::make_shared< ShaderObject >( GetOpenGl(), p_eType );
			}
		}

		void ShaderProgram::DestroyObject( eSHADER_OBJECT_TYPE p_eType )
		{
			if ( m_shaders[p_eType] )
			{
				m_shaders[p_eType]->Destroy();
				m_shaders[p_eType].reset();
			}
		}

		bool ShaderProgram::Link()
		{
			bool l_bReturn = false;

			if ( !m_bError && IsValid() )
			{
				for ( auto l_shader : m_shaders )
				{
					if ( l_shader )
					{
						l_shader->AttachTo( shared_from_this() );
					}
				}

				GLint l_iLinked = 0;
				GetOpenGl().LinkProgram( GetGlName() );
				GetOpenGl().GetProgramParameter( GetGlName(), GL_LINK_STATUS, &l_iLinked );
				m_linkerLog = RetrieveLinkerLog();

				if ( l_iLinked && m_linkerLog.find( _T( "ERROR" ) ) == String::npos )
				{
					m_bLinked = true;
					m_linkerLog.clear();
				}
				else
				{
					std::cerr << m_linkerLog << std::endl;
				}

				l_bReturn = m_bLinked;
			}

			return l_bReturn;
		}

		String ShaderProgram::RetrieveLinkerLog()
		{
			String l_log;
			GLint l_iLength = 0;
			GLsizei l_uiLength = 0;

			if ( IsValid() )
			{
				GetOpenGl().GetProgramParameter( GetGlName(), GL_INFO_LOG_LENGTH , &l_iLength );

				if ( l_iLength > 1 )
				{
					std::vector< char > l_pTmp( l_iLength + 1 );
					GetOpenGl().GetProgramInfoLog( GetGlName(), l_iLength, &l_uiLength, l_pTmp.data() );
					l_log = StringUtils::ToString( l_pTmp.data() );
				}
			}

			return l_log;
		}

		bool ShaderProgram::Activate()
		{
			bool l_bReturn = true;

			if ( IsValid() && m_bLinked && !m_bError )
			{
				if ( Bind() )
				{
					l_bReturn = ApplyAllVariables();
				}
			}

			return l_bReturn;
		}

		void ShaderProgram::Deactivate()
		{
			if ( GetGlName() != 0 && m_bLinked && !m_bError )
			{
				Unbind();
			}
		}

		void ShaderProgram::SetProgramFile( String const & p_strFile, eSHADER_OBJECT_TYPE p_eType )
		{
			if ( m_shaders[p_eType] )
			{
				m_shaders[p_eType]->SetFile( p_strFile );
				ResetToCompile();
			}
		}

		String ShaderProgram::GetCompilerLog( eSHADER_OBJECT_TYPE p_eType )const
		{
			String l_log;

			if ( m_shaders[p_eType] )
			{
				l_log = m_shaders[p_eType]->GetCompilerLog();
			}

			return l_log;
		}

		uint32_t ShaderProgram::GetAttributeLocation( String const & p_strName )const
		{
			uint32_t l_iReturn = GL_INVALID_INDEX;

			if ( IsValid() )
			{
				l_iReturn = GetOpenGl().GetAttribLocation( GetGlName(), StringUtils::ToStdString( p_strName ).c_str() );
			}

			return l_iReturn;
		}

		uint32_t ShaderProgram::GetUniformLocation( String const & p_strName )const
		{
			uint32_t l_iReturn = GL_INVALID_INDEX;

			if ( IsValid() )
			{
				l_iReturn = GetOpenGl().GetUniformLocation( GetGlName(), StringUtils::ToStdString( p_strName ).c_str() );
			}

			return l_iReturn;
		}

		void ShaderProgram::SetProgramText( String const & p_strFile, eSHADER_OBJECT_TYPE p_eType )
		{
			if ( m_shaders[p_eType] )
			{
				m_shaders[p_eType]->SetFile( String() );
				m_shaders[p_eType]->SetSource( p_strFile );
				ResetToCompile();
			}
		}

		void ShaderProgram::ResetToCompile()
		{
			m_bLinked = false;
			m_bError = false;
		}

		bool ShaderProgram::ApplyAllVariables()
		{
			bool l_bReturn = true;

			for ( auto l_variable: m_listFrameVariables )
			{
				l_bReturn &= l_variable->Apply();
			}

			return true;
		}

		std::shared_ptr< FrameVariable< float > > ShaderProgram::CreateFloatFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_floatFrameVariables );
		}

		std::shared_ptr< FrameVariable< int > > ShaderProgram::CreateIntFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_intFrameVariables );
		}

		std::shared_ptr< Vec2FrameVariable< float > > ShaderProgram::CreateVec2FloatFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_vec2fFrameVariables );
		}

		std::shared_ptr< Vec2FrameVariable< int > > ShaderProgram::CreateVec2IntFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_vec2iFrameVariables );
		}

		std::shared_ptr< Vec3FrameVariable< float > > ShaderProgram::CreateVec3FloatFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_vec3fFrameVariables );
		}

		std::shared_ptr< Vec3FrameVariable< int > > ShaderProgram::CreateVec3IntFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_vec3iFrameVariables );
		}

		std::shared_ptr< Vec4FrameVariable< float > > ShaderProgram::CreateVec4FloatFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_vec4fFrameVariables );
		}

		std::shared_ptr< Vec4FrameVariable< int > > ShaderProgram::CreateVec4IntFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_vec4iFrameVariables );
		}

		std::shared_ptr< Mat4FrameVariable< float > > ShaderProgram::CreateMat4FloatFrameVariable( String const & p_strName )
		{
			return CreateFrameVariable( p_strName, GetOpenGl(), *this, m_listFrameVariables, m_mat4FrameVariables );
		}

		std::shared_ptr< FrameVariable< float > > ShaderProgram::GetFloatFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_floatFrameVariables );
		}

		std::shared_ptr< FrameVariable< int > > ShaderProgram::GetIntFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_intFrameVariables );
		}

		std::shared_ptr< Vec2FrameVariable< float > > ShaderProgram::GetVec2FloatFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_vec2fFrameVariables );
		}

		std::shared_ptr< Vec2FrameVariable< int > > ShaderProgram::GetVec2IntFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_vec2iFrameVariables );
		}

		std::shared_ptr< Vec3FrameVariable< float > > ShaderProgram::GetVec3FloatFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_vec3fFrameVariables );
		}

		std::shared_ptr< Vec3FrameVariable< int > > ShaderProgram::GetVec3IntFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_vec3iFrameVariables );
		}

		std::shared_ptr< Vec4FrameVariable< float > > ShaderProgram::GetVec4FloatFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_vec4fFrameVariables );
		}

		std::shared_ptr< Vec4FrameVariable< int > > ShaderProgram::GetVec4IntFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_vec4iFrameVariables );
		}

		std::shared_ptr< Mat4FrameVariable< float > > ShaderProgram::GetMat4FloatFrameVariable( String const & p_strName )
		{
			return GetFrameVariable( p_strName, m_mat4FrameVariables );
		}
	}
}
