/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_OPEN_GL_H___
#define ___GENERATOR_OPEN_GL_H___

#include "glew.h"

//*************************************************************************************************

extern bool CheckGlError( const wxString & p_strMsg );

typedef void	( wxCALLBACK * PFNGLCLEARCOLORPROC )( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
typedef void	( wxCALLBACK * PFNGLSHADEMODELPROC )( GLenum mode );
typedef void	( wxCALLBACK * PFNGLVIEWPORTPROC )( GLint x, GLint y, GLsizei width, GLsizei height );
typedef void	( wxCALLBACK * PFNGLMATRIXMODEPROC )( GLenum mode );
typedef void	( wxCALLBACK * PFNGLTRANSLATEPROC )( GLfloat x, GLfloat y, GLfloat z );
typedef void	( wxCALLBACK * PFNGLSCALEPROC )( GLfloat x, GLfloat y, GLfloat z );
typedef void	( wxCALLBACK * PFNGLMULTMATRIXPROC )( GLfloat const * matrix );
typedef void	( wxCALLBACK * PFNGLLOADIDENTITYPROC )( void );
typedef void	( wxCALLBACK * PFNGLORTHOPROC )( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
typedef void	( wxCALLBACK * PFNGLCLEARPROC )( GLbitfield mask );
typedef void	( wxCALLBACK * PFNGLDRAWELEMENTSPROC )( GLenum mode, GLsizei count, GLenum type, const GLvoid * indices );
typedef void	( wxCALLBACK * PFNGLENABLEPROC )( GLenum mode );
typedef void	( wxCALLBACK * PFNGLDISABLEPROC )( GLenum mode );
typedef void	( wxCALLBACK * PFNGLGENTEXTURESPROC )( GLsizei n, GLuint * textures );
typedef void	( wxCALLBACK * PFNGLDELETETEXTURESPROC )( GLsizei n, const GLuint * textures );
typedef void	( wxCALLBACK * PFNGLBINDTEXTUREPROC )( GLenum mode, GLuint texture );
typedef void	( wxCALLBACK * PFNGLTEXSUBIMAGE2DPROC )( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * data );
typedef void	( wxCALLBACK * PFNGLTEXIMAGE2DPROC )( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data );
typedef void	( wxCALLBACK * PFNGLTEXPARAMETERIPROC )( GLenum target, GLenum pname, GLint param );
typedef void	( wxCALLBACK * PFNGLGETTEXIMAGEPROC )( GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels );
typedef void	( wxCALLBACK * PFNGLENABLECLIENTSTATEPROC )( GLenum array );
typedef void	( wxCALLBACK * PFNGLDISABLECLIENTSTATEPROC )( GLenum array );
typedef void	( wxCALLBACK * PFNGLVERTEXPOINTERPROC )( GLint size, GLenum type, GLsizei stride, const GLvoid * pointer );
typedef void	( wxCALLBACK * PFNGLNORMALPOINTERPROC )( GLenum type, GLsizei stride, const GLvoid * pointer );
typedef void	( wxCALLBACK * PFNGLTEXCOORDPOINTERPROC )( GLint size, GLenum type, GLsizei stride, const GLvoid * pointer );
typedef void	( wxCALLBACK * PFNGLTEXENVIPROC )( GLenum target, GLenum pname, GLint param );
typedef void	( wxCALLBACK * PFNGLTEXENVIVPROC )( GLenum target, GLenum pname, const GLint * param );
typedef void	( wxCALLBACK * PFNGLTEXENVFPROC )( GLenum target, GLenum pname, GLfloat param );
typedef void	( wxCALLBACK * PFNGLTEXENVFVPROC )( GLenum target, GLenum pname, const GLfloat * param );
typedef void	( wxCALLBACK * PFNGLBLENDFUNCPROC )( GLenum sfactor, GLenum dfactor );
typedef void	( wxCALLBACK * PFNGLALPHAFUNCPROC )( GLenum func, GLclampf ref );
typedef GLenum( wxCALLBACK * PFNGLGETERRORPROC )( void );
typedef void	( wxCALLBACK * PFNGLREADBUFFERPROC )( GLenum mode );
typedef void	( wxCALLBACK * PFNGLREADPIXELSPROC )( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );

//*************************************************************************************************

namespace ProceduralTextures
{
	class GeneratorAPI OpenGl
	{
	public:
		/**@name Construction / Destruction */
		//@{
		OpenGl();
		~OpenGl();
		//@}

		void Initialise();
		void Cleanup();

		/**@name General Functions */
		//@{
		GLenum GetError();
		bool ClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
		bool ShadeModel( GLenum mode );
		bool Viewport( GLint x, GLint y, GLsizei width, GLsizei height );
		bool MatrixMode( GLenum mode );
		bool Translate( GLfloat x, GLfloat y, GLfloat z );
		bool Scale( GLfloat x, GLfloat y, GLfloat z );
		bool MultMatrix( GLfloat const * matrix );
		bool LoadIdentity();
		bool Ortho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
		bool Clear( GLbitfield mask );
		bool DrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid * indices );
		bool Enable( GLenum mode );
		bool Disable( GLenum mode );
		//@}

		/**@name Texture functions */
		//@{
		bool GenTextures( GLsizei n, GLuint * textures );
		bool DeleteTextures( GLsizei n, const GLuint * textures );
		bool GenerateMipmap( GLenum target );
		bool ActiveTexture( GLint texture );
		bool ClientActiveTexture( GLenum texture );
		bool AttachTexture( GLenum mode, GLuint texture );
		bool TexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * data );
		bool TexImage2D( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data );
		bool TexParameteri( GLenum target, GLenum pname, GLint param );
		bool GetTexImage( GLenum target, GLint level, GLenum format, GLenum type, GLvoid * img );
		bool TexEnvi( GLenum target, GLenum pname, GLint param );
		bool TexEnviv( GLenum target, GLenum pname, const GLint * param );
		bool TexEnvf( GLenum target, GLenum pname, GLfloat param );
		bool TexEnvfv( GLenum target, GLenum pname, const GLfloat * param );
		bool BlendFunc( GLenum sfactor, GLenum dfactor );
		bool AlphaFunc( GLenum func, GLclampf ref );
		bool ReadBuffer( GLenum mode );
		bool ReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );
		//@}

		/**@name Buffer objects functions */
		//@{
		bool EnableClientState( GLenum array );
		bool DisableClientState( GLenum array );
		bool GenBuffers( GLsizei n, GLuint * buffers );
		bool BindBuffer( GLenum target, GLuint buffer );
		bool DeleteBuffers( GLsizei n, const GLuint * buffers );
		bool IsBuffer( GLuint buffer );
		bool BufferData( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );
		bool BufferSubData( GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid * data );
		GLvoid * MapBuffer( GLenum target, GLenum access );
		bool UnmapBuffer( GLenum target );
		bool GetBufferParameteriv( GLenum target, GLenum pname, GLint * params );
		bool VertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid * pointer );
		bool NormalPointer( GLenum type, GLsizei stride, const GLvoid * pointer );
		bool TexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid * pointer );
		//@}

		/**@name FBO functions */
		//@{
		bool GenFramebuffers( GLsizei n, GLuint * framebuffers );
		bool DeleteFramebuffers( GLsizei n, const GLuint * framebuffers );
		bool BindFramebuffer( GLenum target, GLuint framebuffer );
		bool FramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
		GLenum CheckFramebufferStatus( GLenum target );
		//@}

		/**@name Uniform variable Functions */
		//@{
		GLint GetUniformLocation( GLuint program, const GLchar * name );
		bool Uniform1i( GLint location, GLint v0 );
		bool Uniform1f( GLint location, GLfloat v0 );
		//@}

		/**@name Shader object Functions */
		//@{
		GLuint CreateShader( GLenum type );
		bool DeleteShader( GLuint shader );
		bool AttachShader( GLuint program, GLuint shader );
		bool DetachShader( GLuint program, GLuint shader );
		bool CompileShader( GLuint shader );
		bool GetShaderiv( GLuint shader, GLenum pname, GLint * param );
		bool GetShaderInfoLog( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
		bool ShaderSource( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * lengths );
		//@}

		/**@name Shader program Functions */
		//@{
		GLuint CreateProgram();
		bool DeleteProgram( GLuint program );
		bool LinkProgram( GLuint program );
		bool UseProgram( GLuint program );
		bool GetProgramiv( GLuint program, GLenum pname, GLint * param );
		bool GetProgramInfoLog( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
		GLint GetAttribLocation( GLuint program, const GLchar * name );
		bool IsProgram( GLuint program );
		//@}

	private:
		/**@name General */
		//@{
		PFNGLGETERRORPROC m_pfnGetError;
		PFNGLCLEARCOLORPROC m_pfnClearColor;
		PFNGLSHADEMODELPROC m_pfnShadeModel;
		PFNGLVIEWPORTPROC m_pfnViewport;
		PFNGLMATRIXMODEPROC m_pfnMatrixMode;
		PFNGLLOADIDENTITYPROC m_pfnLoadIdentity;
		PFNGLTRANSLATEPROC m_pfnTranslate;
		PFNGLSCALEPROC m_pfnScale;
		PFNGLMULTMATRIXPROC m_pfnMultMatrix;
		PFNGLORTHOPROC m_pfnOrtho;
		PFNGLCLEARPROC m_pfnClear;
		PFNGLDRAWELEMENTSPROC m_pfnDrawElements;
		PFNGLENABLEPROC m_pfnEnable;
		PFNGLDISABLEPROC m_pfnDisable;
		//@}

		/**@name Texture */
		//@{
		PFNGLGENTEXTURESPROC m_pfnGenTextures;
		PFNGLDELETETEXTURESPROC m_pfnDeleteTextures;
		PFNGLBINDTEXTUREPROC m_pfnBindTexture;
		PFNGLTEXSUBIMAGE2DPROC m_pfnTexSubImage2D;
		PFNGLTEXIMAGE2DPROC m_pfnTexImage2D;
		PFNGLTEXPARAMETERIPROC m_pfnTexParameteri;
		PFNGLGENERATEMIPMAPPROC m_pfnGenerateMipmap;
		PFNGLACTIVETEXTUREPROC m_pfnActiveTexture;
		PFNGLCLIENTACTIVETEXTUREPROC m_pfnClientActiveTexture;
		PFNGLGETTEXIMAGEPROC m_pfnGetTexImage;
		PFNGLTEXENVIPROC m_pfnTexEnvi;
		PFNGLTEXENVIVPROC m_pfnTexEnviv;
		PFNGLTEXENVFPROC m_pfnTexEnvf;
		PFNGLTEXENVFVPROC m_pfnTexEnvfv;
		PFNGLBLENDFUNCPROC m_pfnBlendFunc;
		PFNGLALPHAFUNCPROC m_pfnAlphaFunc;
		PFNGLREADBUFFERPROC m_pfnReadBuffer;
		PFNGLREADPIXELSPROC m_pfnReadPixels;
		//@}

		/**@name Buffer Objects */
		//@{
		PFNGLENABLECLIENTSTATEPROC m_pfnEnableClientState;
		PFNGLDISABLECLIENTSTATEPROC m_pfnDisableClientState;
		PFNGLGENBUFFERSPROC m_pfnGenBuffers;
		PFNGLBINDBUFFERPROC m_pfnBindBuffer;
		PFNGLDELETEBUFFERSPROC m_pfnDeleteBuffers;
		PFNGLISBUFFERPROC m_pfnIsBuffer;
		PFNGLBUFFERDATAPROC m_pfnBufferData;
		PFNGLBUFFERSUBDATAARBPROC m_pfnBufferSubData;
		PFNGLGETBUFFERPARAMETERIVPROC m_pfnGetBufferParameteriv;
		PFNGLMAPBUFFERPROC m_pfnMapBuffer;
		PFNGLUNMAPBUFFERPROC m_pfnUnmapBuffer;
		PFNGLVERTEXPOINTERPROC m_pfnVertexPointer;
		PFNGLNORMALPOINTERPROC m_pfnNormalPointer;
		PFNGLTEXCOORDPOINTERPROC m_pfnTexCoordPointer;
		//@}

		/**@name FBO */
		//@{
		PFNGLGENFRAMEBUFFERSPROC m_pfnGenFramebuffers;
		PFNGLDELETEFRAMEBUFFERSPROC m_pfnDeleteFramebuffers;
		PFNGLBINDFRAMEBUFFERPROC m_pfnBindFramebuffer;
		PFNGLFRAMEBUFFERTEXTURE2DPROC m_pfnFramebufferTexture2D;
		PFNGLCHECKFRAMEBUFFERSTATUSPROC m_pfnCheckFramebufferStatus;
		//@}

		/**@name Uniform variables */
		//@{
		PFNGLGETUNIFORMLOCATIONPROC m_pfnGetUniformLocation;
		PFNGLUNIFORM1IPROC m_pfnUniform1i;
		PFNGLUNIFORM1FPROC m_pfnUniform1f;
		//@}

		/**@name Shader object */
		//@{
		PFNGLCREATESHADERPROC m_pfnCreateShader;
		PFNGLDELETESHADERPROC m_pfnDeleteShader;
		PFNGLATTACHSHADERPROC m_pfnAttachShader;
		PFNGLDETACHSHADERPROC m_pfnDetachShader;
		PFNGLCOMPILESHADERPROC m_pfnCompileShader;
		PFNGLGETSHADERIVPROC m_pfnGetShaderiv;
		PFNGLGETSHADERINFOLOGPROC m_pfnGetShaderInfoLog;
		PFNGLSHADERSOURCEPROC m_pfnShaderSource;
		//@}

		/**@name Shader program */
		//@{
		PFNGLCREATEPROGRAMPROC m_pfnCreateProgram;
		PFNGLDELETEPROGRAMPROC m_pfnDeleteProgram;
		PFNGLLINKPROGRAMPROC m_pfnLinkProgram;
		PFNGLUSEPROGRAMPROC m_pfnUseProgram;
		PFNGLGETPROGRAMIVPROC m_pfnGetProgramiv;
		PFNGLGETPROGRAMINFOLOGPROC m_pfnGetProgramInfoLog;
		PFNGLGETATTRIBLOCATIONPROC m_pfnGetAttribLocation;
		PFNGLISPROGRAMPROC m_pfnIsProgram;
		//@}
	};
}

#ifndef NDEBUG
#	define glCheckError( x )	CheckGlError( this, wxT( x ) )
#else
#	define glCheckError( x ) true
#endif

//*************************************************************************************************

#endif
