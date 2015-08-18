#include "GlIndexBuffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		IndexBuffer::IndexBuffer( OpenGl & p_openGl, uint32_t p_mode )
			: m_bufferObject( p_openGl, GL_ELEMENT_ARRAY_BUFFER, p_mode )
		{
			m_bufferData[0] = 0;
			m_bufferData[1] = 1;
			m_bufferData[2] = 2;
			m_bufferData[3] = 0;
			m_bufferData[4] = 2;
			m_bufferData[5] = 3;
		}

		IndexBuffer::~IndexBuffer()
		{
		}

		bool IndexBuffer::Initialise()
		{
			bool l_return = m_bufferObject.Create();

			if ( l_return )
			{
				l_return = m_bufferObject.Bind();

				if ( l_return )
				{
					m_bufferObject.Data( m_bufferData.data(), m_bufferData.size() * sizeof( uint32_t ) );
					m_bufferObject.Unbind();
				}
			}

			return l_return;
		}

		void IndexBuffer::Cleanup()
		{
			m_bufferObject.Destroy();
		}

		bool IndexBuffer::Bind()
		{
			return m_bufferObject.Bind();
		}

		void IndexBuffer::Unbind()
		{
			m_bufferObject.Unbind();
		}
	}
}
