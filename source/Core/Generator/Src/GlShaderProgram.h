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
#ifndef ___GENERATOR_GL_SHADER_PROGRAM_H___
#define ___GENERATOR_GL_SHADER_PROGRAM_H___

#include "GeneratorPrerequisites.h"

namespace ProceduralTextures
{
	/*!
	\author		Sylvain DOREMUS
	\date		14/02/2010
	\brief		Shader program implementation
	*/
	class GeneratorAPI GlShaderProgram
	{
	protected:
		template< class Ty > friend struct FrameVariableCreator;

	public:
		/**
		 *\brief		Constructor
		 *\param[in]	p_pOpenGl		The OpenGL instance
		 *\param[in]	p_shaderFiles	The shader file for each shader object
		 */
		GlShaderProgram( OpenGl * p_pOpenGl, const wxArrayString & p_shaderFiles );
		/**
		 *\brief		Constructor
		 *\param[in]	p_pOpenGl	The OpenGL instance
		 */
		GlShaderProgram( OpenGl * p_pOpenGl );
		/**
		 *\brief		Destructor
		 */
		~GlShaderProgram();
		/**
		 *\brief		Initialises the program
		 *\return		true if compiled and linked successfully
		 */
		bool Initialise();
		/**
		 *\brief		Cleans the program up
		 */
		void Cleanup();
		/**
		 *\brief		Creates a shader object
		 *\param[in]	p_eType	The object type
		 */
		void CreateObject( eSHADER_OBJECT_TYPE p_eType );
		/**
		 *\brief		Destroys a shader object
		 *\param[in]	p_eType	The object type
		 */
		void DestroyObject( eSHADER_OBJECT_TYPE p_eType );
		/**
		 *\brief		Activates the program
		 *\return		true the program and all its variables are activated successfully
		 */
		bool Activate();
		/**
		 *\brief		Deactivates the program
		 */
		void Deactivate();
		/**
		 *\brief		Sets the file for the given object
		 *\param[in]	p_strFile	The file
		 *\param[in]	p_eType		The object type
		 */
		void SetProgramFile( const wxString & p_strFile, eSHADER_OBJECT_TYPE p_eType );
		/**
		 *\brief		Sets the text for the given object
		 *\param[in]	p_strSource	The source
		 *\param[in]	p_eType	The object type
		 */
		void SetProgramText( const wxString & p_strSource, eSHADER_OBJECT_TYPE p_eType );
		/**
		 *\brief		Retrieves the wanted shader object
		 *\param[in]	p_eType	The object type
		 *\return		The shader object
		 */
		GlShaderObject * GetObject( eSHADER_OBJECT_TYPE p_eType );
		/**
		 *\brief		Links all Shaders held by the program
		 *\return		true if linked successfully
		 */
		bool Link();
		/**
		 *\brief		Resets compilation variables to be able to compile again
		 */
		void ResetToCompile();
		/**
		 *\brief		Pass all the variables to the shader objects
		 *\return		true if all the variables are applied successfully
		 */
		bool ApplyAllVariables();
		/**
		 *\brief		Retrieves Vertex Attribute from the program
		 *\param[in]	p_strName	The attribute name
		 *\return		The attribute location
		 */
		int GetAttributeLocation( const wxString & p_strName )const;
		/**
		 *\brief		Creates a single float frame variable with given name
		 *\param[in]	p_name	The variable name
		 *\param[in]	p_value	The variable value
		 */
		GlFrameVariable< float > * CreateFloatFrameVariable( const wxString & p_name );
		/**
		 *\brief		Creates a single int frame variable with given name
		 *\param[in]	p_name	The variable name
		 *\param[in]	p_value	The variable value
		 */
		GlFrameVariable< int > * CreateIntFrameVariable( const wxString & p_name );
		/**
		 *\brief		Sets a frame variable value
		 *\param[in]	p_name	The variable name
		 *\param[in]	p_value	The variable value
		 */
		void SetFrameVariableValue( const wxString & p_name, float p_value );
		/**
		 *\brief		Sets a frame variable value
		 *\param[in]	p_name	The variable name
		 *\param[in]	p_value	The variable value
		 */
		void SetFrameVariableValue( const wxString & p_name, int p_value );
		/**
		 *\brief		Retrieves the link status for the program
		 *\return		The value
		 */
		inline bool IsLinked()const
		{
			return m_bLinked;
		}
		/**
		 *\brief		Retrieves the OpenGL program index
		 *\return		The value
		 */
		inline GLuint GetProgramObject()const
		{
			return m_programObject;
		}
		/**
		 *\brief		Retrieves The link messages log
		 *\return		The value
		 */
		inline const wxString & GetLinkerLog()const
		{
			return m_linkerLog;
		}
		/**
		 *\brief		Retrieves a constant iterator to the beginning of the frame variables list
		 *\return		The value
		 */
		FrameVariablePtrList::const_iterator Begin()const
		{
			return m_listFrameVariables.begin();
		}
		/**
		 *\brief		Retrieves an iterator to the beginning of the frame variables list
		 *\return		The value
		 */
		FrameVariablePtrList::iterator Begin()
		{
			return m_listFrameVariables.begin();
		}
		/**
		 *\brief		Retrieves a constant iterator to the end of the frame variables list
		 *\return		The value
		 */
		FrameVariablePtrList::const_iterator End()const
		{
			return m_listFrameVariables.end();
		}
		/**
		 *\brief		Retrieves an iterator to the end of the frame variables list
		 *\return		The value
		 */
		FrameVariablePtrList::iterator End()
		{
			return m_listFrameVariables.end();
		}

	protected:
		/**
		 *\brief		Get linker messages
		 *\return		The messages
		 */
		wxString RetrieveLinkerLog();

		//! The OpenGL instance
		OpenGl * m_pOpenGl;
		//! Tells if the program is linked
		bool m_bLinked;
		//! Tells if the program is in error
		bool m_bError;
		//! The shader objects
		GlShaderObject * m_pShaders[eSHADER_OBJECT_TYPE_COUNT];
		//! The frame variables list
		FrameVariablePtrList m_listFrameVariables;
		//! The OpenGL index for the program
		GLuint m_programObject;
		//! The linker log messages
		wxString m_linkerLog;
	};
}

//*************************************************************************************************

#endif
