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

typedef GLenum( PGEN_CALLBACK * PFNGLGETERRORPROC )( void );
typedef void ( PGEN_CALLBACK * PFNGLCLEARCOLORPROC )( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
typedef void ( PGEN_CALLBACK * PFNGLVIEWPORTPROC )( GLint x, GLint y, GLsizei width, GLsizei height );
typedef void ( PGEN_CALLBACK * PFNGLCLEARPROC )( GLbitfield mask );
typedef void ( PGEN_CALLBACK * PFNGLDRAWELEMENTSPROC )( GLenum mode, GLsizei count, GLenum type, const GLvoid * indices );
typedef void ( PGEN_CALLBACK * PFNGLENABLEPROC )( GLenum cap );
typedef void ( PGEN_CALLBACK * PFNGLDISABLEPROC )( GLenum cap );
typedef void ( PGEN_CALLBACK * PFNGLGENTEXTURESPROC )( GLsizei n, GLuint * textures );
typedef void ( PGEN_CALLBACK * PFNGLDELETETEXTURESPROC )( GLsizei n, const GLuint * textures );
typedef GLboolean ( PGEN_CALLBACK * PFNGLISTEXTUREPROC )( GLuint texture );
typedef void ( PGEN_CALLBACK * PFNGLBINDTEXTUREPROC )( GLenum target, GLuint texture );
typedef void ( PGEN_CALLBACK * PFNGLTEXSUBIMAGE2DPROC )( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels );
typedef void ( PGEN_CALLBACK * PFNGLTEXIMAGE2DPROC )( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * pixels );
typedef void ( PGEN_CALLBACK * PFNGLTEXPARAMETERIPROC )( GLenum target, GLenum pname, GLint param );
typedef void ( PGEN_CALLBACK * PFNGLGETTEXIMAGEPROC )( GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels );
typedef void ( PGEN_CALLBACK * PFNGLREADBUFFERPROC )( GLenum mode );
typedef void ( PGEN_CALLBACK * PFNGLREADPIXELSPROC )( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * pixels );
typedef void ( PGEN_CALLBACK * PFNGLBLENDFUNCPROC )( GLenum sfactor, GLenum dfactor );
typedef void ( PGEN_CALLBACK * PFNGLBLENDFUNCSEPARATEPROC )( GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );
typedef void ( PGEN_CALLBACK * PFNGLDRAWARRAYSPROC )( GLenum mode, GLint first, GLsizei count );


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
			void Initialise();

			/** Releases the API functions
			 */
			void Cleanup();

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
			unsigned int GetError()const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClearColor.xml
			 */
			bool ClearColor( float red, float green, float blue, float alpha );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glViewport.xml
			 */
			bool Viewport( int x, int y, int width, int height );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClear.xml
			 */
			bool Clear( unsigned int mask );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawElements.xml
			 */
			bool DrawElements( unsigned int mode, int count, unsigned int type, const void * indices );

			/** see https://www.opengl.org/sdk/docs/man/html/glDrawArrays.xhtml
			 */
			bool DrawArrays( unsigned int mode, int first, int count );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnable.xml
			 */
			bool Enable( unsigned int mode );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnable.xml
			 */
			bool Disable( unsigned int mode );

			/** see https://www.opengl.org/sdk/docs/man2/xhtml/glBlendFunc.xml
			 */
			bool BlendFunc(	unsigned int sfactor, unsigned int dfactor );

			/** see https://www.opengl.org/sdk/docs/man2/xhtml/glBlendFuncSeparate.xml
			 */
			bool BlendFuncSeparate(	unsigned int srcRGB, unsigned int dstRGB, unsigned int srcAlpha, unsigned int dstAlpha );

			//@}
			/**@name Texture functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenTextures.xml
			 */
			bool GenTextures( int n, unsigned int * textures );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteTextures.xml
			 */
			bool DeleteTextures( int n, const unsigned int * textures );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsTexture.xml
			 */
			bool IsTexture( unsigned int texture )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenerateMipmap.xml
			 */
			bool GenerateMipmap( unsigned int target );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glActiveTexture.xml
			 */
			bool ActiveTexture( int texture );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glClientActiveTexture.xml
			 */
			bool ClientActiveTexture( unsigned int texture );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindTexture.xml
			 */
			bool BindTexture( unsigned int mode, unsigned int texture );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexSubImage2D.xml
			 */
			bool TexSubImage2D( unsigned int target, int level, int xoffset, int yoffset, int width, int height, unsigned int format, unsigned int type, const void * data );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexImage2D.xml
			 */
			bool TexImage2D( unsigned int target, int level, int internalFormat, int width, int height, int border, unsigned int format, unsigned int type, const void * data );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml
			 */
			bool TexParameter( unsigned int target, unsigned int pname, int param );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetTexImage.xml
			 */
			bool GetTexImage( unsigned int target, int level, unsigned int format, unsigned int type, void * img )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glReadBuffer.xml
			 */
			bool ReadBuffer( unsigned int mode );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glReadPixels.xml
			 */
			bool ReadPixels( int x, int y, int width, int height, unsigned int format, unsigned int type, void * pixels )const;

			//@}
			/**@name Buffer objects functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenBuffers.xml
			 */
			bool GenBuffers( int n, unsigned int * buffers );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindBuffer.xml
			 */
			bool BindBuffer( unsigned int target, unsigned int buffer );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteBuffers.xml
			 */
			bool DeleteBuffers( int n, const unsigned int * buffers );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsBuffer.xml
			 */
			bool IsBuffer( unsigned int buffer )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBufferData.xml
			 */
			bool BufferData( unsigned int target, ptrdiff_t size, const void * data, unsigned int usage );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBufferSubData.xml
			 */
			bool BufferSubData( unsigned int target, ptrdiff_t offset, ptrdiff_t size, const void * data );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glMapBuffer.xml
			 */
			void * MapBuffer( unsigned int target, unsigned int access );

			/** see https://www.opengl.org/sdk/docs/man/html/glUnmapBuffer.xhtml
			 */
			bool UnmapBuffer( unsigned int target );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetBufferParameter.xml
			 */
			bool GetBufferParameter( unsigned int target, unsigned int pname, int * params );

			//@}
			/**@name FBO functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGenFramebuffers.xml
			 */
			bool GenFramebuffers( int n, unsigned int * framebuffers );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteFramebuffers.xml
			 */
			bool DeleteFramebuffers( int n, const unsigned int * framebuffers );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsFramebuffer.xml
			 */
			bool IsFramebuffer( unsigned int framebuffer )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindFramebuffer.xml
			 */
			bool BindFramebuffer( unsigned int target, unsigned int framebuffer );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glFramebufferTexture.xml
			 */
			bool FramebufferTexture2D( unsigned int target, unsigned int attachment, unsigned int textarget, unsigned int texture, int level );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCheckFramebufferStatus.xml
			 */
			unsigned int CheckFramebufferStatus( unsigned int target )const;

			//@}
			/**@name Uniform variable Functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetUniformLocation.xml
			 */
			int GetUniformLocation( unsigned int program, const char * name )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool Uniform( int location, int v0 );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool Uniform( int location, float v0 );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool UniformVec2( int location, int const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool UniformVec2( int location, float const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool UniformVec3( int location, int const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool UniformVec3( int location, float const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool UniformVec4( int location, int const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool UniformVec4( int location, float const * v );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUniform.xml
			 */
			bool UniformMat4( int location, float const * v );

			//@}
			/**@name Shader object Functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCreateShader.xml
			 */
			unsigned int CreateShader( unsigned int type );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteShader.xml
			 */
			bool DeleteShader( unsigned int shader );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsShader.xml
			 */
			bool IsShader( unsigned int shader )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glAttachShader.xml
			 */
			bool AttachShader( unsigned int program, unsigned int shader );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDetachShader.xml
			 */
			bool DetachShader( unsigned int program, unsigned int shader );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCompileShader.xml
			 */
			bool CompileShader( unsigned int shader );

			/** see https://www.opengl.org/sdk/docs/man/html/glGetShader.xhtml
			 */
			bool GetShaderParameter( unsigned int shader, unsigned int pname, int * param )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetShaderInfoLog.xml
			 */
			bool GetShaderInfoLog( unsigned int shader, int bufSize, int * length, char * infoLog )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glShaderSource.xml
			 */
			bool ShaderSource( unsigned int shader, int count, const char ** strings, const int * lengths );

			//@}
			/**@name Shader program Functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glCreateProgram.xml
			 */
			unsigned int CreateProgram();

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDeleteProgram.xml
			 */
			bool DeleteProgram( unsigned int program );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glIsProgram.xml
			 */
			bool IsProgram( unsigned int program )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glLinkProgram.xml
			 */
			bool LinkProgram( unsigned int program );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glUseProgram.xml
			 */
			bool UseProgram( unsigned int program );

			/** see https://www.opengl.org/sdk/docs/man/html/glGetProgram.xhtml
			 */
			bool GetProgramParameter( unsigned int program, unsigned int pname, int * param )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetProgramInfoLog.xml
			 */
			bool GetProgramInfoLog( unsigned int program, int bufSize, int * length, char * infoLog )const;

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetAttribLocation.xml
			 */
			int GetAttribLocation( unsigned int program, const char * name )const;

			//@}
			/**@name Vertex Attribute Pointer functions */
			//@{

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnableVertexAttribArray.xml
			 */
			bool EnableVertexAttribArray( uint32_t index );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glVertexAttribPointer.xml
			 */
			bool VertexAttribPointer( uint32_t index, int size, unsigned int type, bool normalized, int stride, void const * pointer );

			/** see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glEnableVertexAttribArray.xml
			 */
			bool DisableVertexAttribArray( uint32_t index );

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
