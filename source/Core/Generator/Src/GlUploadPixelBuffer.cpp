#include "GlUploadPixelBuffer.h"
#include "Buffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		UploadPixelBuffer::UploadPixelBuffer( OpenGl & p_openGl )
			: GpuIOBuffer( p_openGl, GL_PIXEL_UNPACK_BUFFER, GL_STREAM_DRAW )
		{
		}

		UploadPixelBuffer::~UploadPixelBuffer()
		{
		}
	}
}
