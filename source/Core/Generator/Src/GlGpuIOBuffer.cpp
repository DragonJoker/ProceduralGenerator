#include "GlGpuIOBuffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		GpuIOBuffer::GpuIOBuffer( OpenGl & p_openGl, GLenum p_packMode, GLenum p_drawMode )
			: m_bufferObject( p_openGl, p_packMode, p_drawMode )
		{
		}

		GpuIOBuffer::~GpuIOBuffer()
		{
		}

		bool GpuIOBuffer::Initialise( size_t p_size )
		{
			bool l_return = m_bufferObject.Create();

			if ( l_return )
			{
				l_return = m_bufferObject.Bind();

				if ( l_return )
				{
					m_bufferObject.Data( NULL, p_size );
					m_bufferObject.Unbind();
				}
			}

			return l_return;
		}

		void GpuIOBuffer::Cleanup()
		{
			m_bufferObject.Destroy();
		}

		bool GpuIOBuffer::Bind()
		{
			return m_bufferObject.Bind();
		}

		void GpuIOBuffer::Unbind()
		{
			m_bufferObject.Unbind();
		}

		void GpuIOBuffer::Data( void const * p_buffer, size_t p_size )
		{
			return m_bufferObject.Data( p_buffer, p_size );
		}

		void * GpuIOBuffer::Lock( unsigned int p_access )
		{
			return m_bufferObject.Lock( p_access );
		}

		void GpuIOBuffer::Unlock()
		{
			m_bufferObject.Unlock();
		}
	}
}
