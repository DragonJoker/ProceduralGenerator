#include "GlFrameVariableBase.h"
#include "GlShaderProgram.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	GlFrameVariableBase::GlFrameVariableBase( OpenGl * p_pOpenGl, GlShaderProgram * p_pProgram )
		:	m_glIndex( GL_INVALID_INDEX )
		,	m_used( true )
		,	m_pProgram( p_pProgram )
		,	m_bChanged( true )
		,	m_pOpenGl( p_pOpenGl )
	{
	}

	GlFrameVariableBase::~GlFrameVariableBase()
	{
	}
}
