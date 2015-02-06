#include "GlDownloadPixelBuffer.h"
#include "Buffer.h"

namespace ProceduralTextures
{
	GlDownloadPixelBuffer::GlDownloadPixelBuffer( OpenGl * p_pOpenGl )
		:	GlPixelBuffer( p_pOpenGl, GL_PIXEL_PACK_BUFFER, GL_STREAM_READ )
	{
	}

	GlDownloadPixelBuffer::~GlDownloadPixelBuffer()
	{
	}
}
