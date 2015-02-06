#include "GlIndexBuffer.h"

namespace ProceduralTextures
{
	GlIndexBuffer::GlIndexBuffer( OpenGl * p_pOpenGl )
		:	GlBuffer( p_pOpenGl, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW )
	{
		m_pBuffer[0] = 0;
		m_pBuffer[1] = 1;
		m_pBuffer[2] = 2;
		m_pBuffer[3] = 0;
		m_pBuffer[4] = 2;
		m_pBuffer[5] = 3;
	}

	GlIndexBuffer::~GlIndexBuffer()
	{
	}

	bool GlIndexBuffer::Initialise()
	{
		bool l_return = Create();

		if ( l_return )
		{
			l_return = Bind();

			if ( l_return )
			{
				Data( m_pBuffer, sizeof( m_pBuffer ) );
				Unbind();
			}
		}

		return l_return;
	}

	bool GlIndexBuffer::Activate()
	{
		return Bind();
	}

	void GlIndexBuffer::Deactivate()
	{
		Unbind();
	}
}
