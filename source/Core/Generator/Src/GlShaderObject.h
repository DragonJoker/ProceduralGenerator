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
#ifndef ___GENERATOR_GL_SHADER_OBJECT_H___
#define ___GENERATOR_GL_SHADER_OBJECT_H___

#include "GeneratorPrerequisites.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Shader object implementation
	*/
	class GeneratorAPI GlShaderObject
	{
	protected:
		friend class GlShaderProgram;

	protected:
		/**
		 *\brief		Constructor
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 *\param[in]	p_pParent	The parent program
		 *\param[in]	p_eType		The object type
		 */
		GlShaderObject( OpenGl * p_pOpenGl, GlShaderProgram * p_pParent, eSHADER_OBJECT_TYPE p_eType );
		/**
		 * Destructor
		 */
		~GlShaderObject();

	public:
		/**
		 *\brief		Loads a shader file.
		 *\param[in]	p_filename	The name of the ASCII file containing the shader.
		 */
		void SetFile( const wxString & p_filename );
		/**
		 *\brief		Load program from text
		 *\param[in]	p_source	The object string source
		 */
		void SetSource( const wxString & p_source );
		/**
		 *\brief		Creates the program
		 */
		void CreateProgram();
		/**
		 *\brief		Destroys the program
		 */
		void DestroyProgram();
		/**
		 *\brief		Compiles program
		 *\return		true if compiled successfully
		 */
		bool Compile();
		/**
		 *\brief		Attaches this shader to the given program
		 *\param[in]	p_pParent	The parent program
		 */
		void AttachTo( GlShaderProgram * p_pProgram );
		/**
		 *\brief		Detaches this shader from it's program
		 */
		void Detach();
		/**
		 *\brief		Retrieves the shader type
		 *\return		The value
		 */
		inline eSHADER_OBJECT_TYPE GetShaderType()const
		{
			return m_eType;
		}
		/**
		 *\brief		Retrieves the compiled status
		 *\return		The value
		 */
		inline bool IsCompiled()const
		{
			return m_bCompiled;
		}
		/**
		 *\brief		Retrieves the shader source
		 *\return		The value
		 */
		inline const wxString & GetSource()const
		{
			return m_strSource;
		}
		/**
		 *\brief		Retrieves the shader source file
		 *\return		The value
		 */
		inline const wxString & GetFile()const
		{
			return m_pathFile;
		}
		/**
		 *\brief		Retrieves the compiler log
		 *\return		The value
		 */
		inline const wxString & GetCompilerLog()const
		{
			return m_compilerLog;
		}

	protected:
		/**
		 *\brief		Get compiler messages
		 *\return		The messages
		 */
		wxString RetrieveCompilerLog();

		//! The OpenGL instance
		OpenGl * m_pOpenGl;
		//! The mapping of eSHADER_OBJECT_TYPE to OpenGL corresponding values
		static GLenum ShaderTypes[eSHADER_OBJECT_TYPE_COUNT];
		//! The parent program
		GlShaderProgram * m_pParent;
		//! The program type
		eSHADER_OBJECT_TYPE m_eType;
		//! ASCII Source-Code
		wxString m_strSource;
		//! File containing Source-Code
		wxString m_pathFile;
		//! Tells if the program is compiled
		bool m_bCompiled;
		//! Tells if the program is in error after compilation
		bool m_bError;
		//! Shader Object
		GLuint m_shaderObject;
		//! The compiler messages
		wxString m_compilerLog;
		//! Tells whether or not this object is attached to a program
		bool m_bAttached;
	};
}

//*************************************************************************************************

#endif
