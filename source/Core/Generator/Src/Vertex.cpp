#include "Vertex.h"

namespace ProceduralTextures
{
	template<> unsigned long long Vertex::Count = 0;
	template<> size_t Vertex::Size = 4;
	template<> unsigned long long VertexI::Count = 0;
	template<> size_t VertexI::Size = 4;
}
