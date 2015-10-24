#include "GlDownloadPixelBuffer.h"
#include "Buffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		DownloadPixelBuffer::DownloadPixelBuffer( OpenGl & p_openGl )
			: m_ioBuffer( p_openGl, GL_PIXEL_PACK_BUFFER, GL_STREAM_READ )
		{
		}

		DownloadPixelBuffer::~DownloadPixelBuffer()
		{
		}

		bool DownloadPixelBuffer::Initialise( size_t p_size )
		{
			return m_ioBuffer.Initialise( p_size );
		}

		void DownloadPixelBuffer::Cleanup()
		{
			m_ioBuffer.Cleanup();
		}

		bool DownloadPixelBuffer::Bind()
		{
			return m_ioBuffer.Bind();
		}

		void DownloadPixelBuffer::Unbind()
		{
			m_ioBuffer.Unbind();
		}

		void DownloadPixelBuffer::Data( void const * p_buffer, size_t p_size )
		{
			return m_ioBuffer.Data( p_buffer, p_size );
		}

		void * DownloadPixelBuffer::Lock( unsigned int p_access )
		{
			return m_ioBuffer.Lock( p_access );
		}

		void DownloadPixelBuffer::Unlock()
		{
			m_ioBuffer.Unlock();
		}
	}
}
