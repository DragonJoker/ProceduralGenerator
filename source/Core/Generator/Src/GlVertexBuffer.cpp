#include "GlVertexBuffer.h"

namespace ProceduralTextures
{
	namespace gl
	{
		template<> uint32_t VertexBuffer::m_dataType = GL_FLOAT;
		template<> uint32_t VertexBufferI::m_dataType = GL_INT;
	}
}
