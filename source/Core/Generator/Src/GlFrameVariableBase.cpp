#include "GlFrameVariableBase.h"
#include "GlShaderProgram.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		FrameVariableBase::FrameVariableBase( OpenGl & p_openGl, ShaderProgram & p_program )
			: Holder( p_openGl )
			, m_glIndex( GL_INVALID_INDEX )
			, m_used( true )
			, m_program( p_program )
			, m_changed( true )
		{
		}

		FrameVariableBase::~FrameVariableBase()
		{
		}
	}
}
