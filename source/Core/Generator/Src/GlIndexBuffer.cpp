#include "GlIndexBuffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		IndexBuffer::IndexBuffer( std::shared_ptr< OpenGl > p_pOpenGl, uint32_t p_mode )
			: BufferObject( p_pOpenGl, GL_ELEMENT_ARRAY_BUFFER, p_mode )
		{
			m_pBuffer[0] = 0;
			m_pBuffer[1] = 1;
			m_pBuffer[2] = 2;
			m_pBuffer[3] = 0;
			m_pBuffer[4] = 2;
			m_pBuffer[5] = 3;
		}

		IndexBuffer::~IndexBuffer()
		{
		}

		bool IndexBuffer::Initialise()
		{
			bool l_return = Create();

			if ( l_return )
			{
				l_return = Bind();

				if ( l_return )
				{
					Data( m_pBuffer.data(), m_pBuffer.size() * sizeof( uint32_t ) );
					Unbind();
				}
			}

			return l_return;
		}

		bool IndexBuffer::Activate()
		{
			return Bind();
		}

		void IndexBuffer::Deactivate()
		{
			Unbind();
		}
	}
}
