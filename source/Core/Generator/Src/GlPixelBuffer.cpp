#include "GlPixelBuffer.h"

namespace ProceduralTextures
{
	GlPixelBuffer::GlPixelBuffer( OpenGl * p_pOpenGl, GLenum p_packMode, GLenum p_drawMode )
		: GlBuffer( p_pOpenGl, p_packMode, p_drawMode )
	{
	}

	GlPixelBuffer::~GlPixelBuffer()
	{
	}

	bool GlPixelBuffer::Initialise()
	{
		return false;
	}

	bool GlPixelBuffer::Initialise( size_t p_size )
	{
		bool l_return = Create();

		if ( l_return )
		{
			l_return = Bind();

			if ( l_return )
			{
				Data( NULL, p_size );
				Unbind();
			}
		}

		return l_return;
	}

	bool GlPixelBuffer::Activate()
	{
		return Bind();
	}

	void GlPixelBuffer::Deactivate()
	{
		Unbind();
	}
}
