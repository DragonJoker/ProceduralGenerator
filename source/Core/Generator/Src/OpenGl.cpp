#include "OpenGl.h"
#include "GeneratorUtils.h"

namespace ProceduralTextures
{
	bool CheckGlError( OpenGl * p_pOpenGl, const wxString & p_strMsg )
	{
		static const wxString l_strErrors[] =
		{
			wxT( "GL_INVALID_ENUM" ),
			wxT( "GL_INVALID_VALUE" ),
			wxT( "GL_INVALID_OPERATION" ),
			wxT( "GL_STACK_OVERFLOW" ),
			wxT( "GL_STACK_UNDERFLOW" ),
			wxT( "GL_OUT_OF_MEMORY" ),
		};
		GLenum l_error = p_pOpenGl->GetError();
		bool l_bReturn = true;

		if ( l_error != GL_NO_ERROR )
		{
			l_bReturn = false;
			utils::Trace( p_strMsg + wxT( " - " ) + l_strErrors[l_error - GL_INVALID_ENUM] + wxT( "\n" ) );
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

		m_pfnGetError					= & glGetError;
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
		m_pfnBindFramebuffer			= glBindFramebuffer;
		m_pfnFramebufferTexture2D		= glFramebufferTexture2D;
		m_pfnCheckFramebufferStatus		= glCheckFramebufferStatus;
		m_pfnGetUniformLocation			= glGetUniformLocation;
		m_pfnUniform1i					= glUniform1i;
		m_pfnUniform1f					= glUniform1f;
		m_pfnCreateShader				= glCreateShader;
		m_pfnDeleteShader				= glDeleteShader;
		m_pfnAttachShader				= glAttachShader;
		m_pfnDetachShader				= glDetachShader;
		m_pfnCompileShader				= glCompileShader;
		m_pfnGetShaderiv				= glGetShaderiv;
		m_pfnGetShaderInfoLog			= glGetShaderInfoLog;
		m_pfnShaderSource				= glShaderSource;
		m_pfnCreateProgram				= glCreateProgram;
		m_pfnDeleteProgram				= glDeleteProgram;
		m_pfnLinkProgram				= glLinkProgram;
		m_pfnUseProgram					= glUseProgram;
		m_pfnGetProgramiv				= glGetProgramiv;
		m_pfnGetProgramInfoLog			= glGetProgramInfoLog;
		m_pfnGetAttribLocation			= glGetAttribLocation;
		m_pfnIsProgram					= glIsProgram;
		m_pfnClearColor					= &glClearColor;
		m_pfnShadeModel					= &glShadeModel;
		m_pfnViewport					= &glViewport;
		m_pfnMatrixMode					= &glMatrixMode;
		m_pfnLoadIdentity				= &glLoadIdentity;
		m_pfnTranslate					= &glTranslatef;
		m_pfnScale						= &glScalef;
		m_pfnMultMatrix					= &glMultMatrixf;
		m_pfnOrtho						= &glOrtho;
		m_pfnClear						= &glClear;
		m_pfnDrawElements				= &glDrawElements;
		m_pfnEnable						= &glEnable;
		m_pfnDisable					= &glDisable;
		m_pfnGenTextures				= &glGenTextures;
		m_pfnDeleteTextures				= &glDeleteTextures;
		m_pfnBindTexture				= &glBindTexture;
		m_pfnTexSubImage2D				= &glTexSubImage2D;
		m_pfnTexImage2D					= &glTexImage2D;
		m_pfnTexParameteri				= &glTexParameteri;
		m_pfnEnableClientState			= &glEnableClientState;
		m_pfnDisableClientState			= &glDisableClientState;
		m_pfnVertexPointer				= &glVertexPointer;
		m_pfnNormalPointer				= &glNormalPointer;
		m_pfnGetTexImage				= &glGetTexImage;
		m_pfnTexCoordPointer			= &glTexCoordPointer;
		m_pfnTexEnvi					= &glTexEnvi;
		m_pfnTexEnviv					= &glTexEnviv;
		m_pfnTexEnvf					= &glTexEnvf;
		m_pfnTexEnvfv					= &glTexEnvfv;
		m_pfnBlendFunc					= &glBlendFunc;
		m_pfnAlphaFunc					= &glAlphaFunc;
		m_pfnReadBuffer					= &glReadBuffer;
		m_pfnReadPixels					= &glReadPixels;
	}

	void OpenGl::Cleanup()
	{
		m_pfnGetError					= NULL;
		m_pfnGenerateMipmap				= NULL;
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
		m_pfnBindFramebuffer			= NULL;
		m_pfnFramebufferTexture2D		= NULL;
		m_pfnCheckFramebufferStatus		= NULL;
		m_pfnGetUniformLocation			= NULL;
		m_pfnUniform1i					= NULL;
		m_pfnUniform1f					= NULL;
		m_pfnCreateShader				= NULL;
		m_pfnDeleteShader				= NULL;
		m_pfnAttachShader				= NULL;
		m_pfnDetachShader				= NULL;
		m_pfnCompileShader				= NULL;
		m_pfnGetShaderiv				= NULL;
		m_pfnGetShaderInfoLog			= NULL;
		m_pfnShaderSource				= NULL;
		m_pfnCreateProgram				= NULL;
		m_pfnDeleteProgram				= NULL;
		m_pfnLinkProgram				= NULL;
		m_pfnUseProgram					= NULL;
		m_pfnGetProgramiv				= NULL;
		m_pfnGetProgramInfoLog			= NULL;
		m_pfnGetAttribLocation			= NULL;
		m_pfnIsProgram					= NULL;
		m_pfnClearColor					= NULL;
		m_pfnShadeModel					= NULL;
		m_pfnViewport					= NULL;
		m_pfnMatrixMode					= NULL;
		m_pfnLoadIdentity				= NULL;
		m_pfnTranslate					= NULL;
		m_pfnScale						= NULL;
		m_pfnOrtho						= NULL;
		m_pfnClear						= NULL;
		m_pfnDrawElements				= NULL;
		m_pfnEnable						= NULL;
		m_pfnDisable					= NULL;
		m_pfnGenTextures				= NULL;
		m_pfnDeleteTextures				= NULL;
		m_pfnBindTexture				= NULL;
		m_pfnTexSubImage2D				= NULL;
		m_pfnTexImage2D					= NULL;
		m_pfnTexParameteri				= NULL;
		m_pfnEnableClientState			= NULL;
		m_pfnDisableClientState			= NULL;
		m_pfnVertexPointer				= NULL;
		m_pfnNormalPointer				= NULL;
		m_pfnGetTexImage				= NULL;
		m_pfnTexCoordPointer			= NULL;
		m_pfnTexEnvi					= NULL;
		m_pfnBlendFunc					= NULL;
		m_pfnAlphaFunc					= NULL;
		m_pfnReadBuffer					= NULL;
		m_pfnReadPixels					= NULL;
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

	bool OpenGl::ShadeModel( GLenum mode )
	{
		m_pfnShadeModel( mode );
		return glCheckError( "glShadeModel" );
	}

	bool OpenGl::Viewport( GLint x, GLint y, GLsizei width, GLsizei height )
	{
		m_pfnViewport( x, y, width, height );
		return glCheckError( "glViewport" );
	}

	bool OpenGl::MatrixMode( GLenum mode )
	{
		m_pfnMatrixMode( mode );
		return glCheckError( "glMatrixMode" );
	}

	bool OpenGl::Translate( GLfloat x, GLfloat y, GLfloat z )
	{
		m_pfnTranslate( x, y, z );
		return glCheckError( "glTranslatef" );
	}

	bool OpenGl::Scale( GLfloat x, GLfloat y, GLfloat z )
	{
		m_pfnScale( x, y, z );
		return glCheckError( "glScalef" );
	}

	bool OpenGl::MultMatrix( GLfloat const * matrix )
	{
		m_pfnMultMatrix( matrix );
		return glCheckError( "glMultMatrixf" );
	}

	bool OpenGl::LoadIdentity()
	{
		m_pfnLoadIdentity();
		return glCheckError( "glLoadIdentity" );
	}

	bool OpenGl::Ortho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
	{
		m_pfnOrtho( left, right, bottom, top, zNear, zFar );
		return glCheckError( "glOrtho" );
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

	bool OpenGl::AttachTexture( GLenum mode, GLuint texture )
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

	bool OpenGl::TexParameteri( GLenum target, GLenum pname, GLint param )
	{
		m_pfnTexParameteri( target, pname, param );
		return glCheckError( "glTexParameteri" );
	}

	bool OpenGl::GetTexImage( GLenum target, GLint level, GLenum format, GLenum type, GLvoid * img )
	{
		m_pfnGetTexImage( target, level, format, type, img );
		return glCheckError( "glGetTexImage" );
	}

	bool OpenGl::TexEnvi( GLenum target, GLenum pname, GLint param )
	{
		m_pfnTexEnvi( target, pname, param );
		return glCheckError( "glTexEnvi" );
	}

	bool OpenGl::TexEnviv( GLenum target, GLenum pname, const GLint * param )
	{
		m_pfnTexEnviv( target, pname, param );
		return glCheckError( "glTexEnviv" );
	}

	bool OpenGl::TexEnvf( GLenum target, GLenum pname, GLfloat param )
	{
		m_pfnTexEnvf( target, pname, param );
		return glCheckError( "glTexEnvf" );
	}

	bool OpenGl::TexEnvfv( GLenum target, GLenum pname, const GLfloat * param )
	{
		m_pfnTexEnvfv( target, pname, param );
		return glCheckError( "glTexEnvfv" );
	}

	bool OpenGl::BlendFunc( GLenum sfactor, GLenum dfactor )
	{
		m_pfnBlendFunc( sfactor, dfactor );
		return glCheckError( "glBlendFunc" );
	}

	bool OpenGl::AlphaFunc( GLenum func, GLclampf ref )
	{
		m_pfnAlphaFunc( func, ref );
		return glCheckError( "glAlphaFunc" );
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

	bool OpenGl::EnableClientState( GLenum array )
	{
		m_pfnEnableClientState( array );
		return glCheckError( "glEnableClientState" );
	}

	bool OpenGl::DisableClientState( GLenum array )
	{
		m_pfnDisableClientState( array );
		return glCheckError( "glDisableClientState" );
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
		return m_pfnIsBuffer( buffer ) && glCheckError( "glIsBuffer" );
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

	bool OpenGl::GetBufferParameteriv( GLenum target, GLenum pname, GLint * params )
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

	bool OpenGl::VertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid * pointer )
	{
		m_pfnVertexPointer( size, type, stride, pointer );
		return glCheckError( "glVertexPointer" );
	}

	bool OpenGl::NormalPointer( GLenum type, GLsizei stride, const GLvoid * pointer )
	{
		m_pfnNormalPointer( type, stride, pointer );
		return glCheckError( "glNormalPointer" );
	}

	bool OpenGl::TexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid * pointer )
	{
		m_pfnTexCoordPointer( size, type, stride, pointer );
		return glCheckError( "glTexCoordPointer" );
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

	bool OpenGl::Uniform1i( GLint location, GLint v0 )
	{
		m_pfnUniform1i( location, v0 );
		return glCheckError( "glUniform1i" );
	}

	bool OpenGl::Uniform1f( GLint location, GLfloat v0 )
	{
		m_pfnUniform1f( location, v0 );
		return glCheckError( "glUniform1f" );
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

	bool OpenGl::GetShaderiv( GLuint shader, GLenum pname, GLint * param )
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

	bool OpenGl::GetProgramiv( GLuint program, GLenum pname, GLint * param )
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

	bool OpenGl::IsProgram( GLuint program )
	{
		m_pfnIsProgram( program );
		return glCheckError( "glIsProgram" );
	}
}
