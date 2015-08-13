#include "GlGpuIOBuffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		GpuIOBuffer::GpuIOBuffer( OpenGl & p_openGl, GLenum p_packMode, GLenum p_drawMode )
			: BufferObject( p_openGl, p_packMode, p_drawMode )
		{
		}

		GpuIOBuffer::~GpuIOBuffer()
		{
		}

		bool GpuIOBuffer::Initialise()
		{
			return false;
		}

		bool GpuIOBuffer::Initialise( size_t p_size )
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

		bool GpuIOBuffer::Activate()
		{
			return Bind();
		}

		void GpuIOBuffer::Deactivate()
		{
			Unbind();
		}
	}
}
