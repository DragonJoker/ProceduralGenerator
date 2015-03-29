#include "GlUploadPixelBuffer.h"
#include "Buffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		UploadPixelBuffer::UploadPixelBuffer( std::shared_ptr< OpenGl > p_pOpenGl )
			: GpuIOBuffer( p_pOpenGl, GL_PIXEL_UNPACK_BUFFER, GL_STREAM_DRAW )
		{
		}

		UploadPixelBuffer::~UploadPixelBuffer()
		{
		}
	}
}
