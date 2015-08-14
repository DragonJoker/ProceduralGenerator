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
#ifndef ___GENERATOR_GL_SHADER_OBJECT_H___
#define ___GENERATOR_GL_SHADER_OBJECT_H___

#include "GlObject.h"

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
		Shader object implementation
	*/
	class GeneratorAPI ShaderObject
		: public Object< std::function< uint32_t() >, std::function< bool( uint32_t ) > >
	{
	protected:
		typedef Object< std::function< uint32_t() >, std::function< bool( uint32_t ) > > ParentClass;

	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGL instance
		@param[in] p_type
			The object type
		*/
		ShaderObject( OpenGl & p_openGl, eSHADER_OBJECT_TYPE p_type );

		/**
		 * Destructor
		*/
		~ShaderObject();

		/** Loads a shader file.
		@param[in] p_filename
			The name of the ASCII file containing the shader.
		*/
		void SetFile( String const & p_filename );

		/** Load program from text
		@param[in] p_source
			The object string source
		*/
		void SetSource( String const & p_source );

		/** Destroys the program
		*/
		virtual void Destroy();

		/** Compiles program
		@return
				true if compiled successfully
		*/
		bool Compile();

		/** Attaches this shader to the given program
		@param[in] p_parent
			The parent program
		*/
		void AttachTo( std::shared_ptr< ShaderProgram > p_parent );

		/** Detaches this shader from it's program
		*/
		void Detach();

		/** Retrieves the shader type
		@return
			The value
		*/
		inline eSHADER_OBJECT_TYPE GetShaderType()const
		{
			return m_eType;
		}

		/** Retrieves the compiled status
		@return
			The value
		*/
		inline bool IsCompiled()const
		{
			return m_bCompiled;
		}

		/** Retrieves the shader source
		@return
			The value
		*/
		inline String const & GetSource()const
		{
			return m_strSource;
		}

		/** Retrieves the shader source file
		@return
			The value
		*/
		inline String const & GetFile()const
		{
			return m_pathFile;
		}

		/** Retrieves the compiler log
		@return
			The value
		*/
		inline String const & GetCompilerLog()const
		{
			return m_compilerLog;
		}

		/** Retrieves the parent program
		@return
			The value
		*/
		inline std::shared_ptr< ShaderProgram > GetParent()
		{
			return m_parent.lock();
		}

	protected:
		/** Get compiler messages
		@return
			The messages
		*/
		String RetrieveCompilerLog();

		//! The mapping of eSHADER_OBJECT_TYPE to OpenGL corresponding values
		static unsigned int ShaderTypes[eSHADER_OBJECT_TYPE_COUNT];
		//! The parent program
		std::weak_ptr< ShaderProgram > m_parent;
		//! The program type
		eSHADER_OBJECT_TYPE m_eType;
		//! ASCII Source-Code
		String m_strSource;
		//! File containing Source-Code
		String m_pathFile;
		//! Tells if the program is compiled
		bool m_bCompiled;
		//! Tells if the program is in error after compilation
		bool m_bError;
		//! The compiler messages
		String m_compilerLog;
		//! Tells whether or not this object is attached to a program
		bool m_bAttached;
	};
}
}

//*************************************************************************************************

#endif
