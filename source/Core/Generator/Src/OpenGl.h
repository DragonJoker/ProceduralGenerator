/*
This source file is part of ProceduralGenerator (https://sourceforge.net/projects/proceduralgene/ )

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (At your option ) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERATOR_OPENGL_H___
#define ___GENERATOR_OPENGL_H___

#include "GeneratorPrerequisites.h"
#include "glew.h"

typedef GLenum( PGEN_CALLBACK * PFNGLGETERRORPROC )( GLvoid );
typedef GLvoid ( PGEN_CALLBACK * PFNGLCLEARCOLORPROC )( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
typedef GLvoid ( PGEN_CALLBACK * PFNGLVIEWPORTPROC )( GLint x, GLint y, GLsizei width, GLsizei height );
typedef GLvoid ( PGEN_CALLBACK * PFNGLCLEARPROC )( GLbitfield mask );
typedef GLvoid ( PGEN_CALLBACK * PFNGLDRAWELEMENTSPROC )( GLenum mode, GLsizei count, GLenum type, const GLvoid * indices );
typedef GLvoid ( PGEN_CALLBACK * PFNGLENABLEPROC )( GLenum cap );
typedef GLvoid ( PGEN_CALLBACK * PFNGLDISABLEPROC )( GLenum cap );
typedef GLvoid ( PGEN_CALLBACK * PFNGLGENTEXTURESPROC )( GLsizei n, GLuint * textures );
typedef GLvoid ( PGEN_CALLBACK * PFNGLDELETETEXTURESPROC )( GLsizei n, const GLuint * textures );
typedef GLboolean( PGEN_CALLBACK * PFNGLISTEXTUREPROC )( GLuint texture );
typedef GLvoid ( PGEN_CALLBACK * PFNGLBINDTEXTUREPROC )( GLenum target, GLuint texture );
typedef GLvoid ( PGEN_CALLBACK * PFNGLTEXSUBIMAGE2DPROC )( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels );
typedef GLvoid ( PGEN_CALLBACK * PFNGLTEXIMAGE2DPROC )( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels );
typedef GLvoid ( PGEN_CALLBACK * PFNGLTEXPARAMETERIPROC )( GLenum target, GLenum pname, GLint param );
typedef GLvoid ( PGEN_CALLBACK * PFNGLGETTEXIMAGEPROC )( GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels );
typedef GLvoid ( PGEN_CALLBACK * PFNGLREADBUFFERPROC )( GLenum mode );
typedef GLvoid ( PGEN_CALLBACK * PFNGLREADPIXELSPROC )( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );
typedef GLvoid ( PGEN_CALLBACK * PFNGLBLENDFUNCPROC )( GLenum sfactor, GLenum dfactor );
typedef GLvoid ( PGEN_CALLBACK * PFNGLBLENDFUNCSEPARATEPROC )( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );
typedef GLvoid ( PGEN_CALLBACK * PFNGLDRAWARRAYSPROC )( GLenum mode, GLint first, GLsizei count );


namespace ProceduralTextures
{
	namespace gl
	{
		bool CheckGlError( OpenGl const & p_gl, String const & p_strMsg );

		/*!
		@author
			Sylvain DOREMUS
		@date
			03/01/2011
		@version
				1.0.0.0
		@brief
			OpenGL API wrapper
		*/
		class GeneratorAPI OpenGl
		{
		public:
			/**@name Construction / Destruction */
			//@{

			/** Constructor
			*/
			OpenGl();

			/** Destructor
			*/
			~OpenGl();

			//@}
			/**@name Context binding */
			//@{

			/** Retrieves the API functions
			*/
			GLvoid Initialise();

			/** Releases the API functions
			*/
			GLvoid Cleanup();

			/** Retrieves the initialisation status
			*/
			inline bool IsInitialised()const
			{
				return m_initialised;
			}

			//@}
			/**@name General Functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml
			*/
			GLenum GetError()const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClearColor.xml
			*/
			bool ClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glViewport.xml
			*/
			bool Viewport( GLint x, GLint y, GLsizei width, GLsizei height );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClear.xml
			*/
			bool Clear( GLbitfield mask );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawElements.xml
			*/
			bool DrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid * indices );

			/** see https://www.opengl.org/sdk/docs/man/html/glDrawArrays.xhtml
			*/
			bool DrawArrays( GLenum mode, GLint first, GLsizei count );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnable.xml
			*/
			bool Enable( GLenum mode );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnable.xml
			*/
			bool Disable( GLenum mode );

			/** see https://www.opengl.org/sdk/docs/man2/xhtml/glBlendFunc.xml
			*/
			bool BlendFunc( GLenum sfactor, GLenum dfactor );

			/** see https://www.opengl.org/sdk/docs/man2/xhtml/glBlendFuncSeparate.xml
			*/
			bool BlendFuncSeparate( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );

			//@}
			/**@name Texture functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenTextures.xml
			*/
			bool GenTextures( GLsizei n, GLuint * textures );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteTextures.xml
			*/
			bool DeleteTextures( GLsizei n, const GLuint * textures );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsTexture.xml
			*/
			bool IsTexture( GLuint texture )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenerateMipmap.xml
			*/
			bool GenerateMipmap( GLenum target );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glActiveTexture.xml
			*/
			bool ActiveTexture( GLint texture );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClientActiveTexture.xml
			*/
			bool ClientActiveTexture( GLenum texture );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindTexture.xml
			*/
			bool BindTexture( GLenum mode, GLuint texture );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexSubImage2D.xml
			*/
			bool TexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * data );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexImage2D.xml
			*/
			bool TexImage2D( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml
			*/
			bool TexParameter( GLenum target, GLenum pname, GLint param );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetTexImage.xml
			*/
			bool GetTexImage( GLenum target, GLint level, GLenum format, GLenum type, GLvoid * img )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glReadBuffer.xml
			*/
			bool ReadBuffer( GLenum mode );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glReadPixels.xml
			*/
			bool ReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels )const;

			//@}
			/**@name Buffer objects functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenBuffers.xml
			*/
			bool GenBuffers( GLsizei n, GLuint * buffers );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindBuffer.xml
			*/
			bool BindBuffer( GLenum target, GLuint buffer );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteBuffers.xml
			*/
			bool DeleteBuffers( GLsizei n, const GLuint * buffers );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsBuffer.xml
			*/
			bool IsBuffer( GLuint buffer )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBufferData.xml
			*/
			bool BufferData( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBufferSubData.xml
			*/
			bool BufferSubData( GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid * data );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glMapBuffer.xml
			*/
			GLvoid * MapBuffer( GLenum target, GLenum access );

			/** see https://www.opengl.org/sdk/docs/man/html/glUnmapBuffer.xhtml
			*/
			bool UnmapBuffer( GLenum target );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetBufferParameter.xml
			*/
			bool GetBufferParameter( GLenum target, GLenum pname, GLint * params );

			//@}
			/**@name FBO functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenFramebuffers.xml
			*/
			bool GenFramebuffers( GLsizei n, GLuint * framebuffers );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteFramebuffers.xml
			*/
			bool DeleteFramebuffers( GLsizei n, const GLuint * framebuffers );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsFramebuffer.xml
			*/
			bool IsFramebuffer( GLuint framebuffer )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindFramebuffer.xml
			*/
			bool BindFramebuffer( GLenum target, GLuint framebuffer );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glFramebufferTexture.xml
			*/
			bool FramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCheckFramebufferStatus.xml
			*/
			GLenum CheckFramebufferStatus( GLenum target )const;

			//@}
			/**@name Uniform variable Functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetUniformLocation.xml
			*/
			int GetUniformLocation( GLuint program, const GLchar * name )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool Uniform( GLint location, GLint v0 );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool Uniform( GLint location, GLfloat v0 );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool UniformVec2( GLint location, GLint const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool UniformVec2( GLint location, GLfloat const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool UniformVec3( GLint location, GLint const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool UniformVec3( GLint location, GLfloat const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool UniformVec4( GLint location, GLint const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool UniformVec4( GLint location, GLfloat const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			*/
			bool UniformMat4( GLint location, GLfloat const * v );

			//@}
			/**@name Shader object Functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCreateShader.xml
			*/
			GLenum CreateShader( GLenum type );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteShader.xml
			*/
			bool DeleteShader( GLuint shader );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsShader.xml
			*/
			bool IsShader( GLuint shader )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glAttachShader.xml
			*/
			bool AttachShader( GLuint program, GLuint shader );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDetachShader.xml
			*/
			bool DetachShader( GLuint program, GLuint shader );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCompileShader.xml
			*/
			bool CompileShader( GLuint shader );

			/** see https://www.opengl.org/sdk/docs/man/html/glGetShader.xhtml
			*/
			bool GetShaderParameter( GLuint shader, GLenum pname, GLint * param )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetShaderInfoLog.xml
			*/
			bool GetShaderInfoLog( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glShaderSource.xml
			*/
			bool ShaderSource( GLuint shader, GLsizei count, const GLchar ** strings, const GLint * lengths );

			//@}
			/**@name Shader program Functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCreateProgram.xml
			*/
			GLenum CreateProgram();

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteProgram.xml
			*/
			bool DeleteProgram( GLuint program );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsProgram.xml
			*/
			bool IsProgram( GLuint program )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glLinkProgram.xml
			*/
			bool LinkProgram( GLuint program );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUseProgram.xml
			*/
			bool UseProgram( GLuint program );

			/** see https://www.opengl.org/sdk/docs/man/html/glGetProgram.xhtml
			*/
			bool GetProgramParameter( GLuint program, GLenum pname, GLint * param )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetProgramInfoLog.xml
			*/
			bool GetProgramInfoLog( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetAttribLocation.xml
			*/
			int GetAttribLocation( GLuint program, const GLchar * name )const;

			//@}
			/**@name Vertex Attribute Pointer functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnableVertexAttribArray.xml
			*/
			bool EnableVertexAttribArray( GLuint index );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glVertexAttribPointer.xml
			*/
			bool VertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid const * pointer );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnableVertexAttribArray.xml
			*/
			bool DisableVertexAttribArray( GLuint index );

			//@}

		private:
			/**@name General */
			//@{

			bool m_initialised;

			PFNGLGETERRORPROC m_pfnGetError;
			PFNGLCLEARCOLORPROC m_pfnClearColor;
			PFNGLVIEWPORTPROC m_pfnViewport;
			PFNGLCLEARPROC m_pfnClear;
			PFNGLDRAWELEMENTSPROC m_pfnDrawElements;
			PFNGLDRAWARRAYSPROC m_pfnDrawArrays;
			PFNGLENABLEPROC m_pfnEnable;
			PFNGLDISABLEPROC m_pfnDisable;
			PFNGLBLENDFUNCPROC m_pfnBlendFunc;
			PFNGLBLENDFUNCSEPARATEPROC m_pfnBlendFuncSeparate;

			//@}
			/**@name Texture */
			//@{

			PFNGLGENTEXTURESPROC m_pfnGenTextures;
			PFNGLDELETETEXTURESPROC m_pfnDeleteTextures;
			PFNGLISTEXTUREPROC m_pfnIsTexture;
			PFNGLBINDTEXTUREPROC m_pfnBindTexture;
			PFNGLTEXSUBIMAGE2DPROC m_pfnTexSubImage2D;
			PFNGLTEXIMAGE2DPROC m_pfnTexImage2D;
			PFNGLTEXPARAMETERIPROC m_pfnTexParameteri;
			PFNGLGENERATEMIPMAPPROC m_pfnGenerateMipmap;
			PFNGLACTIVETEXTUREPROC m_pfnActiveTexture;
			PFNGLCLIENTACTIVETEXTUREPROC m_pfnClientActiveTexture;
			PFNGLGETTEXIMAGEPROC m_pfnGetTexImage;
			PFNGLREADBUFFERPROC m_pfnReadBuffer;
			PFNGLREADPIXELSPROC m_pfnReadPixels;

			//@}
			/**@name Buffer Objects */
			//@{

			PFNGLGENBUFFERSPROC m_pfnGenBuffers;
			PFNGLDELETEBUFFERSPROC m_pfnDeleteBuffers;
			PFNGLISBUFFERPROC m_pfnIsBuffer;
			PFNGLBINDBUFFERPROC m_pfnBindBuffer;
			PFNGLBUFFERDATAPROC m_pfnBufferData;
			PFNGLBUFFERSUBDATAPROC m_pfnBufferSubData;
			PFNGLGETBUFFERPARAMETERIVPROC m_pfnGetBufferParameteriv;
			PFNGLMAPBUFFERPROC m_pfnMapBuffer;
			PFNGLUNMAPBUFFERPROC m_pfnUnmapBuffer;

			//@}
			/**@name FBO */
			//@{

			PFNGLGENFRAMEBUFFERSPROC m_pfnGenFramebuffers;
			PFNGLDELETEFRAMEBUFFERSPROC m_pfnDeleteFramebuffers;
			PFNGLISFRAMEBUFFERPROC m_pfnIsFramebuffer;
			PFNGLBINDFRAMEBUFFERPROC m_pfnBindFramebuffer;
			PFNGLFRAMEBUFFERTEXTURE2DPROC m_pfnFramebufferTexture2D;
			PFNGLCHECKFRAMEBUFFERSTATUSPROC m_pfnCheckFramebufferStatus;

			//@}
			/**@name Uniform variables */
			//@{

			PFNGLGETUNIFORMLOCATIONPROC m_pfnGetUniformLocation;
			PFNGLUNIFORM1IPROC m_pfnUniform1i;
			PFNGLUNIFORM1FPROC m_pfnUniform1f;
			PFNGLUNIFORM2IVPROC m_pfnUniform2iv;
			PFNGLUNIFORM2FVPROC m_pfnUniform2fv;
			PFNGLUNIFORM3IVPROC m_pfnUniform3iv;
			PFNGLUNIFORM3FVPROC m_pfnUniform3fv;
			PFNGLUNIFORM4IVPROC m_pfnUniform4iv;
			PFNGLUNIFORM4FVPROC m_pfnUniform4fv;
			PFNGLUNIFORMMATRIX4FVPROC m_pfnUniformMatrix4fv;
			//@}
			/**@name Shader object */
			//@{

			PFNGLCREATESHADERPROC m_pfnCreateShader;
			PFNGLDELETESHADERPROC m_pfnDeleteShader;
			PFNGLISSHADERPROC m_pfnIsShader;
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
			PFNGLISPROGRAMPROC m_pfnIsProgram;
			PFNGLLINKPROGRAMPROC m_pfnLinkProgram;
			PFNGLUSEPROGRAMPROC m_pfnUseProgram;
			PFNGLGETPROGRAMIVPROC m_pfnGetProgramiv;
			PFNGLGETPROGRAMINFOLOGPROC m_pfnGetProgramInfoLog;
			PFNGLGETATTRIBLOCATIONPROC m_pfnGetAttribLocation;

			//@}
			/**@name Vertex attribute pointer */
			//@{

			PFNGLENABLEVERTEXATTRIBARRAYPROC m_pfnEnableVertexAttribArray;
			PFNGLVERTEXATTRIBPOINTERPROC m_pfnVertexAttribPointer;
			PFNGLDISABLEVERTEXATTRIBARRAYPROC m_pfnDisableVertexAttribArray;

			//@}
		};
	}
}

#endif
