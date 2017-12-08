#include "GlUploadPixelBuffer.h"
#include "Buffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		UploadPixelBuffer::UploadPixelBuffer( OpenGl & p_openGl )
			: m_ioBuffer( p_openGl, GL_PIXEL_UNPACK_BUFFER, GL_STREAM_DRAW )
		{
		}

		UploadPixelBuffer::~UploadPixelBuffer()
		{
		}

		bool UploadPixelBuffer::Initialise( size_t p_size )
		{
			return m_ioBuffer.Initialise( p_size );
		}

		void UploadPixelBuffer::Cleanup()
		{
			m_ioBuffer.Cleanup();
		}

		bool UploadPixelBuffer::Bind()
		{
			return m_ioBuffer.Bind();
		}

		void UploadPixelBuffer::Unbind()
		{
			m_ioBuffer.Unbind();
		}

		void UploadPixelBuffer::Data( void const * p_buffer, size_t p_size )
		{
			return m_ioBuffer.Data( p_buffer, p_size );
		}

		void * UploadPixelBuffer::Lock( uint32_t p_access )
		{
			return m_ioBuffer.Lock( p_access );
		}

		void UploadPixelBuffer::Unlock()
		{
			m_ioBuffer.Unlock();
		}
	}
}
