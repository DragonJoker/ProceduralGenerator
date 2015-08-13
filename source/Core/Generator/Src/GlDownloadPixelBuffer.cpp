#include "GlDownloadPixelBuffer.h"
#include "Buffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		DownloadPixelBuffer::DownloadPixelBuffer( OpenGl & p_openGl )
			: GpuIOBuffer( p_openGl, GL_PIXEL_PACK_BUFFER, GL_STREAM_READ )
		{
		}

		DownloadPixelBuffer::~DownloadPixelBuffer()
		{
		}
	}
}
