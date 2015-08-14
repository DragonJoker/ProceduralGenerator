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
#ifndef ___GENERATOR_GL_SHADER_PROGRAM_H___
#define ___GENERATOR_GL_SHADER_PROGRAM_H___

#include "GlObject.h"
#include <memory>

namespace ProceduralTextures
{
namespace gl
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		14/02/2010
	@brief
		Shader program implementation
	*/
	class GeneratorAPI ShaderProgram
		: public Object< std::function< uint32_t() >, std::function< bool( uint32_t ) > >
		, public std::enable_shared_from_this< ShaderProgram >
	{
	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		*/
		ShaderProgram( OpenGl & p_openGl );

		/** Destructor
		*/
		~ShaderProgram();

		/** Initialises the program
		@return
			true if compiled and linked successfully
		*/
		bool Initialise();

		/** Cleans the program up
		*/
		void Cleanup();

		/** Binds the object on GPU
		@return
			false if not bound successfully
		*/
		virtual bool Bind();

		/** Unbinds the object on GPU
		*/
		virtual void Unbind();

		/** Creates a shader object
		@param[in] p_eType
			The object type
		*/
		void CreateObject( eSHADER_OBJECT_TYPE p_eType );

		/** Destroys a shader object
		@param[in] p_eType
			The object type
		*/
		void DestroyObject( eSHADER_OBJECT_TYPE p_eType );

		/** Activates the program
		@return
			true the program and all its variables are activated successfully
		*/
		bool Activate();

		/** Deactivates the program
		*/
		void Deactivate();

		/** Sets the file for the given object
		@param[in] p_strFile
			The file
		@param[in] p_eType
			The object type
		*/
		void SetProgramFile( String const & p_strFile, eSHADER_OBJECT_TYPE p_eType );

		/** Sets the text for the given object
		@param[in] p_strSource
			The source
		@param[in] p_eType
			The object type
		*/
		void SetProgramText( String const & p_strSource, eSHADER_OBJECT_TYPE p_eType );

		/** Retrieves the wanted shader object compiler log
		@param[in] p_eType
			The object type
		@return
			The compiler log
		*/
		String GetCompilerLog( eSHADER_OBJECT_TYPE p_eType )const;

		/** Links all Shaders held by the program
		@return
			true if linked successfully
		*/
		bool Link();

		/** Resets compilation variables to be able to compile again
		*/
		void ResetToCompile();

		/** Pass all the variables to the shader objects
		@return
			true if all the variables are applied successfully
		*/
		bool ApplyAllVariables();

		/** Retrieves a vertex attribute location from the program
		@param[in] p_strName
			The attribute name
		@return
			The attribute location
		*/
		uint32_t GetAttributeLocation( String const & p_strName )const;

		/** Retrieves an uniform variable location from the program
		@param[in] p_strName
			The variable name
		@return
			The variable location
		*/
		uint32_t GetUniformLocation( String const & p_strName )const;

		/** Creates a single float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< FrameVariable< float > > CreateFloatFrameVariable( String const & p_name );

		/** Creates a single int frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< FrameVariable< int > > CreateIntFrameVariable( String const & p_name );

		/** Creates a vec2 float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec2FrameVariable< float > > CreateVec2FloatFrameVariable( String const & p_name );

		/** Creates a vec2 int frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec2FrameVariable< int > > CreateVec2IntFrameVariable( String const & p_name );

		/** Creates a vec3 float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec3FrameVariable< float > > CreateVec3FloatFrameVariable( String const & p_name );

		/** Creates a vec3 int frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec3FrameVariable< int > > CreateVec3IntFrameVariable( String const & p_name );

		/** Creates a vec4 float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec4FrameVariable< float > > CreateVec4FloatFrameVariable( String const & p_name );

		/** Creates a vec4 int frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec4FrameVariable< int > > CreateVec4IntFrameVariable( String const & p_name );

		/** Creates a mat4 float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Mat4FrameVariable< float > > CreateMat4FloatFrameVariable( String const & p_name );

		/** Creates a single float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< FrameVariable< float > > GetFloatFrameVariable( String const & p_name );

		/** Creates a single int frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< FrameVariable< int > > GetIntFrameVariable( String const & p_name );

		/** Creates a vec2 float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec2FrameVariable< float > > GetVec2FloatFrameVariable( String const & p_name );

		/** Creates a vec2 int frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec2FrameVariable< int > > GetVec2IntFrameVariable( String const & p_name );

		/** Creates a vec3 float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec3FrameVariable< float > > GetVec3FloatFrameVariable( String const & p_name );

		/** Creates a vec3 int frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec3FrameVariable< int > > GetVec3IntFrameVariable( String const & p_name );

		/** Creates a vec4 float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec4FrameVariable< float > > GetVec4FloatFrameVariable( String const & p_name );

		/** Creates a vec4 int frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Vec4FrameVariable< int > > GetVec4IntFrameVariable( String const & p_name );

		/** Creates a mat4 float frame variable with given name
		@param[in] p_name
			The variable name
		*/
		std::shared_ptr< Mat4FrameVariable< float > > GetMat4FloatFrameVariable( String const & p_name );

		/** Retrieves the Link status for the program
		@return
			The value
		*/
		inline bool IsLinked()const
		{
			return m_bLinked;
		}

		/** Retrieves The Link messages log
		@return
			The value
		*/
		inline String const & GetLinkerLog()const
		{
			return m_linkerLog;
		}

		/** Retrieves a constant iterator to the beginning of the frame variables list
		@return
			The value
		*/
		FrameVariablePtrList::const_iterator Begin()const
		{
			return m_listFrameVariables.begin();
		}

		/** Retrieves an iterator to the beginning of the frame variables list
		@return
			The value
		*/
		FrameVariablePtrList::iterator Begin()
		{
			return m_listFrameVariables.begin();
		}

		/** Retrieves a constant iterator to the end of the frame variables list
		@return
			The value
		*/
		FrameVariablePtrList::const_iterator End()const
		{
			return m_listFrameVariables.end();
		}

		/** Retrieves an iterator to the end of the frame variables list
		@return
			The value
		*/
		FrameVariablePtrList::iterator End()
		{
			return m_listFrameVariables.end();
		}

	protected:
		/** Get linker messages
		@return
				The messages
		*/
		String RetrieveLinkerLog();

		//! Tells if the program is linked
		bool m_bLinked;
		//! Tells if the program is in error
		bool m_bError;
		//! The shader objects
		std::vector< std::shared_ptr< ShaderObject > > m_shaders;
		//! The frame variables list
		FrameVariablePtrList m_listFrameVariables;
		//! The float frame variables map, sorted by name
		std::map< String, std::weak_ptr< FrameVariable< float > > > m_floatFrameVariables;
		//! The int frame variables list, sorted by name
		std::map< String, std::weak_ptr< FrameVariable< int > > > m_intFrameVariables;
		//! The vec2f frame variables list, sorted by name
		std::map< String, std::weak_ptr< Vec2FrameVariable< float > > > m_vec2fFrameVariables;
		//! The vec2i frame variables list, sorted by name
		std::map< String, std::weak_ptr< Vec2FrameVariable< int > > > m_vec2iFrameVariables;
		//! The vec3f frame variables list, sorted by name
		std::map< String, std::weak_ptr< Vec3FrameVariable< float > > > m_vec3fFrameVariables;
		//! The vec3i frame variables list, sorted by name
		std::map< String, std::weak_ptr< Vec3FrameVariable< int > > > m_vec3iFrameVariables;
		//! The vec4f frame variables list, sorted by name
		std::map< String, std::weak_ptr< Vec4FrameVariable< float > > > m_vec4fFrameVariables;
		//! The vec4i frame variables list, sorted by name
		std::map< String, std::weak_ptr< Vec4FrameVariable< int > > > m_vec4iFrameVariables;
		//! The mat4 frame variables list, sorted by name
		std::map< String, std::weak_ptr< Mat4FrameVariable< float > > > m_mat4FrameVariables;
		//! The linker log messages
		String m_linkerLog;
	};
}
}

//*************************************************************************************************

#endif
