#include "GlDownloadPixelBuffer.h"
#include "Buffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		DownloadPixelBuffer::DownloadPixelBuffer( std::shared_ptr< OpenGl > p_pOpenGl )
			: GpuIOBuffer( p_pOpenGl, GL_PIXEL_PACK_BUFFER, GL_STREAM_READ )
		{
		}

		DownloadPixelBuffer::~DownloadPixelBuffer()
		{
		}
	}
}
