#include "GlUploadPixelBuffer.h"
#include "Buffer.h"

namespace ProceduralTextures
{
	GlUploadPixelBuffer::GlUploadPixelBuffer( OpenGl * p_pOpenGl )
		:	GlPixelBuffer( p_pOpenGl, GL_PIXEL_UNPACK_BUFFER, GL_STREAM_DRAW )
	{
	}

	GlUploadPixelBuffer::~GlUploadPixelBuffer()
	{
	}
}
