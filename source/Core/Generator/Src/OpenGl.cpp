#include "OpenGl.h"
#include "GeneratorUtils.h"

#ifndef NDEBUG
#	define glCheckError( x ) CheckGlError( *this, _T( x ) )
#else
#	define glCheckError( x ) true
#endif

namespace ProceduralTextures
{
	namespace gl
	{
		bool CheckGlError( OpenGl & p_gl, String const & p_strMsg )
		{
			static const String l_strErrors[] =
			{
				_T( "GL_INVALID_ENUM" ),
				_T( "GL_INVALID_VALUE" ),
				_T( "GL_INVALID_OPERATION" ),
				_T( "GL_STACK_OVERFLOW" ),
				_T( "GL_STACK_UNDERFLOW" ),
				_T( "GL_OUT_OF_MEMORY" ),
			};
			GLenum l_error = p_gl.GetError();
			bool l_bReturn = true;

			if ( l_error != GL_NO_ERROR )
			{
				l_bReturn = false;
				TRACE( p_strMsg + _T( " - " ) + l_strErrors[l_error - GL_INVALID_ENUM] + _T( "\n" ) );
			}

			return l_bReturn;
		}

		//*************************************************************************************************

		OpenGl::OpenGl()
		{
			Cleanup();
		}

		OpenGl::~OpenGl()
		{
		}

		void OpenGl::Initialise()
		{
			glewInit();

			if ( glGenerateMipmap != NULL )
			{
				m_pfnGenerateMipmap			= glGenerateMipmap;
			}
			else
			{
				m_pfnGenerateMipmap			= glGenerateMipmapEXT;
			}

			m_pfnGetError					= &glGetError;
			m_pfnActiveTexture				= glActiveTexture;
			m_pfnClientActiveTexture		= glClientActiveTexture;
			m_pfnGenBuffers					= glGenBuffers;
			m_pfnBindBuffer					= glBindBuffer;
			m_pfnDeleteBuffers				= glDeleteBuffers;
			m_pfnIsBuffer					= glIsBuffer;
			m_pfnBufferData					= glBufferData;
			m_pfnBufferSubData				= glBufferSubData;
			m_pfnGetBufferParameteriv		= glGetBufferParameteriv;
			m_pfnMapBuffer					= glMapBuffer;
			m_pfnUnmapBuffer				= glUnmapBuffer;
			m_pfnGenFramebuffers			= glGenFramebuffers;
			m_pfnDeleteFramebuffers			= glDeleteFramebuffers;
			m_pfnIsFramebuffer				= glIsFramebuffer;
			m_pfnBindFramebuffer			= glBindFramebuffer;
			m_pfnFramebufferTexture2D		= glFramebufferTexture2D;
			m_pfnCheckFramebufferStatus		= glCheckFramebufferStatus;
			m_pfnGetUniformLocation			= glGetUniformLocation;
			m_pfnUniform1i					= glUniform1i;
			m_pfnUniform1f					= glUniform1f;
			m_pfnUniform2iv					= glUniform2iv;
			m_pfnUniform2fv					= glUniform2fv;
			m_pfnUniform3iv					= glUniform3iv;
			m_pfnUniform3fv					= glUniform3fv;
			m_pfnUniform4iv					= glUniform4iv;
			m_pfnUniform4fv					= glUniform4fv;
			m_pfnUniformMatrix4fv			= glUniformMatrix4fv;
			m_pfnCreateShader				= glCreateShader;
			m_pfnDeleteShader				= glDeleteShader;
			m_pfnIsShader					= glIsShader;
			m_pfnAttachShader				= glAttachShader;
			m_pfnDetachShader				= glDetachShader;
			m_pfnCompileShader				= glCompileShader;
			m_pfnGetShaderiv				= glGetShaderiv;
			m_pfnGetShaderInfoLog			= glGetShaderInfoLog;
			m_pfnShaderSource				= glShaderSource;
			m_pfnCreateProgram				= glCreateProgram;
			m_pfnDeleteProgram				= glDeleteProgram;
			m_pfnIsProgram					= glIsProgram;
			m_pfnLinkProgram				= glLinkProgram;
			m_pfnUseProgram					= glUseProgram;
			m_pfnGetProgramiv				= glGetProgramiv;
			m_pfnGetProgramInfoLog			= glGetProgramInfoLog;
			m_pfnGetAttribLocation			= glGetAttribLocation;
			m_pfnEnableVertexAttribArray	= glEnableVertexAttribArray;
			m_pfnVertexAttribPointer		= glVertexAttribPointer;
			m_pfnDisableVertexAttribArray	= glDisableVertexAttribArray;
			m_pfnClearColor					= &glClearColor;
			m_pfnViewport					= &glViewport;
			m_pfnClear						= &glClear;
			m_pfnDrawElements				= &glDrawElements;
			m_pfnDrawArrays					= &glDrawArrays;
			m_pfnEnable						= &glEnable;
			m_pfnDisable					= &glDisable;
			m_pfnGenTextures				= &glGenTextures;
			m_pfnDeleteTextures				= &glDeleteTextures;
			m_pfnIsTexture					= &glIsTexture;
			m_pfnBindTexture				= &glBindTexture;
			m_pfnTexSubImage2D				= &glTexSubImage2D;
			m_pfnTexImage2D					= &glTexImage2D;
			m_pfnTexParameteri				= &glTexParameteri;
			m_pfnGetTexImage				= &glGetTexImage;
			m_pfnReadBuffer					= &glReadBuffer;
			m_pfnReadPixels					= &glReadPixels;
			m_pfnBlendFunc					= &glBlendFunc;
			m_pfnBlendFuncSeparate			= glBlendFuncSeparate;

			m_initialised = true;
		}

		void OpenGl::Cleanup()
		{
			m_initialised = false;

			m_pfnGenerateMipmap				= NULL;
			m_pfnGetError					= NULL;
			m_pfnActiveTexture				= NULL;
			m_pfnClientActiveTexture		= NULL;
			m_pfnGenBuffers					= NULL;
			m_pfnBindBuffer					= NULL;
			m_pfnDeleteBuffers				= NULL;
			m_pfnIsBuffer					= NULL;
			m_pfnBufferData					= NULL;
			m_pfnBufferSubData				= NULL;
			m_pfnGetBufferParameteriv		= NULL;
			m_pfnMapBuffer					= NULL;
			m_pfnUnmapBuffer				= NULL;
			m_pfnGenFramebuffers			= NULL;
			m_pfnDeleteFramebuffers			= NULL;
			m_pfnIsFramebuffer				= NULL;
			m_pfnBindFramebuffer			= NULL;
			m_pfnFramebufferTexture2D		= NULL;
			m_pfnCheckFramebufferStatus		= NULL;
			m_pfnGetUniformLocation			= NULL;
			m_pfnUniform1i					= NULL;
			m_pfnUniform1f					= NULL;
			m_pfnUniform2iv					= NULL;
			m_pfnUniform2fv					= NULL;
			m_pfnUniform3iv					= NULL;
			m_pfnUniform3fv					= NULL;
			m_pfnUniform4iv					= NULL;
			m_pfnUniform4fv					= NULL;
			m_pfnUniformMatrix4fv			= NULL;
			m_pfnCreateShader				= NULL;
			m_pfnDeleteShader				= NULL;
			m_pfnIsShader					= NULL;
			m_pfnAttachShader				= NULL;
			m_pfnDetachShader				= NULL;
			m_pfnCompileShader				= NULL;
			m_pfnGetShaderiv				= NULL;
			m_pfnGetShaderInfoLog			= NULL;
			m_pfnShaderSource				= NULL;
			m_pfnCreateProgram				= NULL;
			m_pfnDeleteProgram				= NULL;
			m_pfnIsProgram					= NULL;
			m_pfnLinkProgram				= NULL;
			m_pfnUseProgram					= NULL;
			m_pfnGetProgramiv				= NULL;
			m_pfnGetProgramInfoLog			= NULL;
			m_pfnGetAttribLocation			= NULL;
			m_pfnEnableVertexAttribArray	= NULL;
			m_pfnVertexAttribPointer		= NULL;
			m_pfnDisableVertexAttribArray	= NULL;
			m_pfnClearColor					= NULL;
			m_pfnViewport					= NULL;
			m_pfnClear						= NULL;
			m_pfnDrawElements				= NULL;
			m_pfnDrawArrays					= NULL;
			m_pfnEnable						= NULL;
			m_pfnDisable					= NULL;
			m_pfnGenTextures				= NULL;
			m_pfnDeleteTextures				= NULL;
			m_pfnIsTexture					= NULL;
			m_pfnBindTexture				= NULL;
			m_pfnTexSubImage2D				= NULL;
			m_pfnTexImage2D					= NULL;
			m_pfnTexParameteri				= NULL;
			m_pfnGetTexImage				= NULL;
			m_pfnReadBuffer					= NULL;
			m_pfnReadPixels					= NULL;
			m_pfnBlendFunc					= NULL;
			m_pfnBlendFuncSeparate			= NULL;
		}

		GLenum OpenGl::GetError()
		{
			return m_pfnGetError();
		}

		bool OpenGl::ClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
		{
			m_pfnClearColor( red, green, blue, alpha );
			return glCheckError( "glClearColor" );
		}

		bool OpenGl::Viewport( GLint x, GLint y, GLsizei width, GLsizei height )
		{
			m_pfnViewport( x, y, width, height );
			return glCheckError( "glViewport" );
		}

		bool OpenGl::Clear( GLbitfield mask )
		{
			m_pfnClear( mask );
			return glCheckError( "glClear" );
		}

		bool OpenGl::DrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid * indices )
		{
			m_pfnDrawElements( mode, count, type, indices );
			return glCheckError( "glDrawElements" );
		}

		bool OpenGl::DrawArrays( GLenum mode, GLint first, GLsizei count )
		{
			m_pfnDrawArrays( mode, first, count );
			return glCheckError( "glDrawArrays" );
		}

		bool OpenGl::Enable( GLenum mode )
		{
			m_pfnEnable( mode );
			return glCheckError( "glEnable" );
		}

		bool OpenGl::Disable( GLenum mode )
		{
			m_pfnDisable( mode );
			return glCheckError( "glDisable" );
		}

		bool OpenGl::BlendFunc(	unsigned int sfactor, unsigned int dfactor )
		{
			m_pfnBlendFunc( sfactor, dfactor );
			return glCheckError( "glBlendFunc" );
		}

		bool OpenGl::BlendFuncSeparate(	unsigned int srcRGB, unsigned int dstRGB, unsigned int srcAlpha, unsigned int dstAlpha )
		{
			m_pfnBlendFuncSeparate( srcRGB, dstRGB, srcAlpha, dstAlpha );
			return glCheckError( "glBlendFuncSeparate" );
		}

		bool OpenGl::GenTextures( GLsizei n, GLuint * textures )
		{
			m_pfnGenTextures( n, textures );
			return glCheckError( "glGenTextures" );
		}

		bool OpenGl::DeleteTextures( GLsizei n, const GLuint * textures )
		{
			m_pfnDeleteTextures( n, textures );
			return glCheckError( "glDeleteTextures" );
		}

		bool OpenGl::IsTexture( GLuint texture )
		{
			bool l_return = m_pfnIsTexture( texture ) == GL_TRUE;
			return glCheckError( "glIsTexture" ) && l_return;
		}

		bool OpenGl::GenerateMipmap( GLenum target )
		{
			m_pfnGenerateMipmap( target );
			return glCheckError( "glGenerateMipmap" );
		}

		bool OpenGl::ActiveTexture( GLint texture )
		{
			m_pfnActiveTexture( texture );
			return glCheckError( "glActiveTexture" );
		}

		bool OpenGl::ClientActiveTexture( GLenum texture )
		{
			m_pfnClientActiveTexture( texture );
			return glCheckError( "glClientActiveTexture" );
		}

		bool OpenGl::BindTexture( GLenum mode, GLuint texture )
		{
			m_pfnBindTexture( mode, texture );
			return glCheckError( "glBindTexture" );
		}

		bool OpenGl::TexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * data )
		{
			m_pfnTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, data );
			return glCheckError( "glTexSubImage2D" );
		}

		bool OpenGl::TexImage2D( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data )
		{
			m_pfnTexImage2D( target, level, internalFormat, width, height, border, format, type, data );
			return glCheckError( "glTexImage2D" );
		}

		bool OpenGl::TexParameter( GLenum target, GLenum pname, GLint param )
		{
			m_pfnTexParameteri( target, pname, param );
			return glCheckError( "glTexParameteri" );
		}

		bool OpenGl::GetTexImage( GLenum target, GLint level, GLenum format, GLenum type, GLvoid * img )
		{
			m_pfnGetTexImage( target, level, format, type, img );
			return glCheckError( "glGetTexImage" );
		}

		bool OpenGl::ReadBuffer( GLenum mode )
		{
			m_pfnReadBuffer( mode );
			return glCheckError( "glReadBuffer" );
		}

		bool OpenGl::ReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels )
		{
			m_pfnReadPixels( x, y, width, height, format, type, pixels );
			return glCheckError( "glReadPixels" );
		}

		bool OpenGl::GenBuffers( GLsizei n, GLuint * buffers )
		{
			m_pfnGenBuffers( n, buffers );
			return glCheckError( "glGenBuffers" );
		}

		bool OpenGl::BindBuffer( GLenum target, GLuint buffer )
		{
			m_pfnBindBuffer( target, buffer );
			return glCheckError( "glBindBuffer" );
		}

		bool OpenGl::DeleteBuffers( GLsizei n, const GLuint * buffers )
		{
			m_pfnDeleteBuffers( n, buffers );
			return glCheckError( "glDeleteBuffers" );
		}

		bool OpenGl::IsBuffer( GLuint buffer )
		{
			bool l_return = m_pfnIsBuffer( buffer ) == GL_TRUE;
			return glCheckError( "glIsBuffer" ) && l_return;
		}

		bool OpenGl::BufferData( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage )
		{
			m_pfnBufferData( target, size, data, usage );
			return glCheckError( "glBufferData" );
		}

		bool OpenGl::BufferSubData( GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid * data )
		{
			m_pfnBufferSubData( target, offset, size, data );
			return glCheckError( "glBufferSubData" );
		}

		bool OpenGl::GetBufferParameter( GLenum target, GLenum pname, GLint * params )
		{
			m_pfnGetBufferParameteriv( target, pname, params );
			return glCheckError( "glGetBufferParameteriv" );
		}

		GLvoid * OpenGl::MapBuffer( GLenum target, GLenum access )
		{
			GLvoid * l_pReturn = m_pfnMapBuffer( target, access );

			if ( ! glCheckError( "glMapBuffer" ) )
			{
				l_pReturn = NULL;
			}

			return l_pReturn;
		}

		bool OpenGl::UnmapBuffer( GLenum target )
		{
			m_pfnUnmapBuffer( target );
			return glCheckError( "glUnmapBuffer" );
		}

		bool OpenGl::GenFramebuffers( GLsizei n, GLuint * framebuffers )
		{
			m_pfnGenFramebuffers( n, framebuffers );
			return glCheckError( "glGenFramebuffers" );
		}

		bool OpenGl::DeleteFramebuffers( GLsizei n, const GLuint * framebuffers )
		{
			m_pfnDeleteFramebuffers( n, framebuffers );
			return glCheckError( "glDeleteFramebuffers" );
		}

		bool OpenGl::IsFramebuffer( GLuint framebuffer )
		{
			bool l_return = m_pfnIsFramebuffer( framebuffer ) == GL_TRUE;
			return glCheckError( "glIsFramebuffer" ) && l_return;
		}

		bool OpenGl::BindFramebuffer( GLenum target, GLuint framebuffer )
		{
			m_pfnBindFramebuffer( target, framebuffer );
			return glCheckError( "glBindFramebuffer" );
		}

		bool OpenGl::FramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
		{
			m_pfnFramebufferTexture2D( target, attachment, textarget, texture, level );
			return glCheckError( "glFramebufferTexture2D" );
		}

		GLenum OpenGl::CheckFramebufferStatus( GLenum target )
		{
			GLenum l_eReturn = m_pfnCheckFramebufferStatus( target );

			if ( ! glCheckError( "glCheckFramebufferStatus" ) )
			{
				l_eReturn = GL_INVALID_INDEX;
			}

			return l_eReturn;
		}

		GLint OpenGl::GetUniformLocation( GLuint program, const GLchar * name )
		{
			GLint l_iReturn = m_pfnGetUniformLocation( program, name );

			if ( ! glCheckError( "glGetUniformLocation" ) )
			{
				l_iReturn = GL_INVALID_INDEX;
			}

			return l_iReturn;
		}

		bool OpenGl::Uniform( GLint location, GLint v0 )
		{
			m_pfnUniform1i( location, v0 );
			return glCheckError( "glUniform1i" );
		}

		bool OpenGl::Uniform( GLint location, GLfloat v0 )
		{
			m_pfnUniform1f( location, v0 );
			return glCheckError( "glUniform1f" );
		}

		bool OpenGl::UniformVec2( GLint location, GLint const * v )
		{
			m_pfnUniform2iv( location, 1, v );
			return glCheckError( "glUniform2iv" );
		}

		bool OpenGl::UniformVec2( GLint location, GLfloat const * v )
		{
			m_pfnUniform2fv( location, 1, v );
			return glCheckError( "glUniform2fv" );
		}

		bool OpenGl::UniformVec3( GLint location, GLint const * v )
		{
			m_pfnUniform3iv( location, 1, v );
			return glCheckError( "glUniform3iv" );
		}

		bool OpenGl::UniformVec3( GLint location, GLfloat const * v )
		{
			m_pfnUniform3fv( location, 1, v );
			return glCheckError( "glUniform3fv" );
		}

		bool OpenGl::UniformVec4( GLint location, GLint const * v )
		{
			m_pfnUniform4iv( location, 1, v );
			return glCheckError( "glUniform4iv" );
		}

		bool OpenGl::UniformVec4( GLint location, GLfloat const * v )
		{
			m_pfnUniform4fv( location, 1, v );
			return glCheckError( "glUniform4fv" );
		}

		bool OpenGl::UniformMat4( GLint location, GLfloat const * v )
		{
			m_pfnUniformMatrix4fv( location, 1, GL_FALSE, v );
			return glCheckError( "glUniformMatrix4fv" );
		}

		GLuint OpenGl::CreateShader( GLenum type )
		{
			GLuint l_uiReturn = m_pfnCreateShader( type );

			if ( ! glCheckError( "glCreateShader" ) )
			{
				l_uiReturn = GL_INVALID_INDEX;
			}

			return l_uiReturn;
		}

		bool OpenGl::DeleteShader( GLuint shader )
		{
			m_pfnDeleteShader( shader );
			return glCheckError( "glDeleteShader" );
		}

		bool OpenGl::IsShader( GLuint shader )
		{
			bool l_return = m_pfnIsShader( shader ) == GL_TRUE;
			return glCheckError( "glIsShader" ) && l_return;
		}

		bool OpenGl::AttachShader( GLuint program, GLuint shader )
		{
			m_pfnAttachShader( program, shader );
			return glCheckError( "glAttachShader" );
		}

		bool OpenGl::DetachShader( GLuint program, GLuint shader )
		{
			m_pfnDetachShader( program, shader );
			return glCheckError( "glDetachShader" );
		}

		bool OpenGl::CompileShader( GLuint shader )
		{
			m_pfnCompileShader( shader );
			return glCheckError( "glCompileShader" );
		}

		bool OpenGl::GetShaderParameter( GLuint shader, GLenum pname, GLint * param )
		{
			m_pfnGetShaderiv( shader, pname, param );
			return glCheckError( "glGetShaderiv" );
		}

		bool OpenGl::GetShaderInfoLog( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
		{
			m_pfnGetShaderInfoLog( shader, bufSize, length, infoLog );
			return glCheckError( "glGetShaderInfoLog" );
		}

		bool OpenGl::ShaderSource( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * lengths )
		{
			m_pfnShaderSource( shader, count, strings, lengths );
			return glCheckError( "glShaderSource" );
		}

		GLuint OpenGl::CreateProgram()
		{
			GLuint l_uiReturn = m_pfnCreateProgram();

			if ( ! glCheckError( "glCreateProgram" ) )
			{
				l_uiReturn = GL_INVALID_INDEX;
			}

			return l_uiReturn;
		}

		bool OpenGl::DeleteProgram( GLuint program )
		{
			m_pfnDeleteProgram( program );
			return glCheckError( "glDeleteProgram" );
		}

		bool OpenGl::IsProgram( GLuint program )
		{
			bool l_return = m_pfnIsProgram( program ) == GL_TRUE;
			return glCheckError( "glIsProgram" ) && l_return;
		}

		bool OpenGl::LinkProgram( GLuint program )
		{
			m_pfnLinkProgram( program );
			return glCheckError( "glLinkProgram" );
		}

		bool OpenGl::UseProgram( GLuint program )
		{
			m_pfnUseProgram( program );
			return glCheckError( "glUseProgram" );
		}

		bool OpenGl::GetProgramParameter( GLuint program, GLenum pname, GLint * param )
		{
			m_pfnGetProgramiv( program, pname, param );
			return glCheckError( "glGetProgramiv" );
		}

		bool OpenGl::GetProgramInfoLog( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog )
		{
			m_pfnGetProgramInfoLog( program, bufSize, length, infoLog );
			return glCheckError( "glGetProgramInfoLog" );
		}

		GLint OpenGl::GetAttribLocation( GLuint program, const GLchar * name )
		{
			GLint l_iReturn = m_pfnGetAttribLocation( program, name );

			if ( ! glCheckError( "glGetAttribLocation" ) )
			{
				l_iReturn = GL_INVALID_INDEX;
			}

			return l_iReturn;
		}

		bool OpenGl::EnableVertexAttribArray( uint32_t index )
		{
			m_pfnEnableVertexAttribArray( index );
			return glCheckError( "glEnableVertexAttribArray" );
		}

		bool OpenGl::VertexAttribPointer( uint32_t index, int size, GLenum type, bool normalized, int stride, void const * pointer )
		{
			m_pfnVertexAttribPointer( index, size, type, normalized, stride, pointer );
			return glCheckError( "glVertexAttribPointer" );
		}

		bool OpenGl::DisableVertexAttribArray( uint32_t index )
		{
			m_pfnDisableVertexAttribArray( index );
			return glCheckError( "glDisableVertexAttribArray" );
		}
	}
}
