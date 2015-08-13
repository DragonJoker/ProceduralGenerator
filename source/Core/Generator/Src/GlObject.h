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
#ifndef ___GENERATOR_GL_OBJECT_H___
#define ___GENERATOR_GL_OBJECT_H___

#include "GlHolder.h"

namespace ProceduralTextures
{
namespace gl
{
	/*!
	@author
		Sylvain DOREMUS
	@date
		12/02/2015
	@version
		2.0.0
	@brief
		Helper class to create an object on GPU
	*/
	template< typename CreateFunction > class CreatorHelper;

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/02/2015
	@version
		2.0.0
	@brief
		Helper class to create an object on GPU
	@remarks
		Specialization for glGenBuffers, glGenTextures, ...
	*/
	template<> class CreatorHelper< std::function< bool( int, uint32_t * ) > >
	{
	public:
		typedef std::function< bool( int, uint32_t * ) > CreateFunction;

		static uint32_t Apply( CreateFunction p_creator )
		{
			uint32_t l_return = -1;

			if ( !p_creator( 1, &l_return ) )
			{
				l_return = -1;
			}

			return l_return;
		}
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/02/2015
	@version
		2.0.0
	@brief
		Helper class to create an object on GPU
	@remarks
		Specialization for glCreateShader
	*/
	template<> class CreatorHelper< std::function< uint32_t() > >
	{
	public:
		typedef std::function< uint32_t() > CreateFunction;

		static uint32_t Apply( CreateFunction p_creator )
		{
			return p_creator();
		}
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/02/2015
	@version
		2.0.0
	@brief
		Helper class to destroy an object on GPU
	*/
	template< typename DestroyFunction > class DestroyerHelper;

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/02/2015
	@version
		2.0.0
	@brief
		Helper class to destroy an object on GPU
	@remarks
		Specialization for glDeleteBuffers, glDeleteTextures, ...
	*/
	template<> class DestroyerHelper< std::function< bool( int, uint32_t const * ) > >
	{
	public:
		typedef std::function< bool( int, uint32_t const * ) > DestroyFunction;

		static void Apply( DestroyFunction p_destroyer, uint32_t p_glName )
		{
			p_destroyer( 1, &p_glName );
		}
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/02/2015
	@version
		2.0.0
	@brief
		Helper class to destroy an object on GPU
	@remarks
		Specialization for glDeleteShader
	*/
	template<> class DestroyerHelper< std::function< bool( uint32_t ) > >
	{
	public:
		typedef std::function< bool( uint32_t ) > DestroyFunction;

		static void Apply( DestroyFunction p_destroyer, uint32_t p_glName )
		{
			p_destroyer( p_glName );
		}
	};

	/*!
	@author
		Sylvain DOREMUS
	@date
		12/02/2015
	@version
		2.0.0
	@brief
		Base class for each OpenGL object (with glGen* and glDelete* functions)
	*/
	template< typename CreateFunction, typename DestroyFunction >
	class Object
		: public Holder
	{
		typedef std::function< bool( uint32_t ) > ValidatorFunction;
		typedef std::function< bool( uint32_t ) > BindFunction;

	public:
		/** Constructor
		@param[in] p_openGl
			The OpenGl instance
		@param[in] p_creator
			The creation function
		@param[in] p_destroyer
			The destruction function
		@param[in] p_validator
			The validation function
		@param[in] p_binder
			The binding function
		 */
		Object( OpenGl & p_openGl, CreateFunction p_creator, DestroyFunction p_destroyer, ValidatorFunction p_validator, BindFunction p_binder = BindFunction() )
			: Holder( p_openGl )
			, m_creator( p_creator )
			, m_destroyer( p_destroyer )
			, m_validator( p_validator )
			, m_binder( p_binder )
			, m_glName( -1 )
		{
		}

		/** Destructor
		 */
		virtual ~Object()
		{
		}

		/** Creates the object on GPU
		@return
			false if not created successfully
		 */
		virtual bool Create()
		{
			bool l_return = m_glName != -1;

			if ( !l_return )
			{
				m_glName = CreatorHelper< CreateFunction >::Apply( m_creator );
				l_return = m_glName != -1;

				if ( m_binder )
				{
					if ( l_return )
					{
						l_return = Bind();
					}

					if ( l_return )
					{
						Unbind();
					}
				}
			}

			return l_return && IsValid();
		}

		/** Destroys the object on GPU
		 */
		virtual void Destroy()
		{
			if ( IsValid() )
			{
				DestroyerHelper< DestroyFunction >::Apply( m_destroyer, m_glName );
			}

			m_glName = -1;
		}

		/** Binds the object on GPU
		@return
			false if not bound successfully
		 */
		virtual bool Bind()
		{
			return m_binder( m_glName );
		}

		/** Unbinds the object on GPU
		 */
		virtual void Unbind()
		{
			m_binder( 0 );
		}

		/** Tells if the object is created
		@return
			The status
		 */
		bool IsValid()const
		{
			return m_glName != -1 && m_validator( m_glName );
		}

		/** Retrieves the OpenGl instance
		@return
			The instance
		 */
		uint32_t GetGlName()const
		{
			return m_glName;
		}

	private:
		//! The creation function
		CreateFunction m_creator;
		//! The destruction function
		DestroyFunction m_destroyer;
		//! The destruction function
		ValidatorFunction m_validator;
		//! The binding function
		BindFunction m_binder;
		//! The GPU object name
		uint32_t m_glName;
	};
}
}

#endif
