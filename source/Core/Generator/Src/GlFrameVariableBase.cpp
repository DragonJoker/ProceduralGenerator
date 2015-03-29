#include "GlFrameVariableBase.h"
#include "GlShaderProgram.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	namespace gl
	{
		FrameVariableBase::FrameVariableBase( std::shared_ptr< OpenGl > p_pOpenGl, std::shared_ptr< ShaderProgram > p_pProgram )
			: Holder( p_pOpenGl )
			, m_glIndex( GL_INVALID_INDEX )
			, m_used( true )
			, m_program( p_pProgram )
			, m_changed( true )
		{
		}

		FrameVariableBase::~FrameVariableBase()
		{
		}
	}
}
