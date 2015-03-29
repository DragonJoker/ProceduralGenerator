#include "GlBufferObject.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		BufferObject::BufferObject( std::shared_ptr< OpenGl > p_pOpenGl, GLenum p_target, GLenum p_mode )
			: Object( p_pOpenGl,
					  std::bind( &OpenGl::GenBuffers, p_pOpenGl, std::placeholders::_1, std::placeholders::_2 ),
					  std::bind( &OpenGl::DeleteBuffers, p_pOpenGl, std::placeholders::_1, std::placeholders::_2 ),
					  std::bind( &OpenGl::IsBuffer, p_pOpenGl, std::placeholders::_1 ),
					  std::bind( &OpenGl::BindBuffer, p_pOpenGl, p_target, std::placeholders::_1 )
					)
			, m_target( p_target )
			, m_mode( p_mode )
			, m_locked( false )
		{
		}

		BufferObject::~BufferObject()
		{
		}

		void BufferObject::Data( void const * p_pBuffer, size_t p_uiSize )
		{
			GetOpenGl()->BufferData( m_target, p_uiSize, p_pBuffer, m_mode );
		}

		void * BufferObject::Lock( GLenum p_access )
		{
			void * l_pReturn = NULL;

			if ( IsValid() )
			{
				l_pReturn = GetOpenGl()->MapBuffer( m_target, p_access );
				m_locked = l_pReturn != NULL;
			}

			return l_pReturn;
		}

		void BufferObject::Unlock()
		{
			if ( m_locked )
			{
				GetOpenGl()->UnmapBuffer( m_target );
			}
		}
	}
}
