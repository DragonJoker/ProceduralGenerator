#include "GlBuffer.h"
#include "OpenGl.h"

namespace ProceduralTextures
{
	GlBuffer::GlBuffer( OpenGl * p_pOpenGl, GLenum p_target, GLenum p_mode )
		:	m_uiGlIndex( GL_INVALID_INDEX )
		,	m_target( p_target )
		,	m_mode( p_mode )
		,	m_pOpenGl( p_pOpenGl )
	{
	}

	GlBuffer::~GlBuffer()
	{
	}

	bool GlBuffer::Create()
	{
		m_pOpenGl->GenBuffers( 1, &m_uiGlIndex );
		return m_uiGlIndex != GL_INVALID_INDEX;
	}

	void GlBuffer::Destroy()
	{
		if ( m_uiGlIndex != GL_INVALID_INDEX )
		{
			m_pOpenGl->DeleteBuffers( 1, &m_uiGlIndex );
			m_uiGlIndex = GL_INVALID_INDEX;
		}
	}

	bool GlBuffer::Bind()
	{
		return m_pOpenGl->BindBuffer( m_target, m_uiGlIndex );
	}

	void GlBuffer::Unbind()
	{
		m_pOpenGl->BindBuffer( m_target, 0 );
	}

	void GlBuffer::Data( void const * p_pBuffer, size_t p_uiSize )
	{
		m_pOpenGl->BufferData( m_target, p_uiSize, p_pBuffer, m_mode );
	}

	void * GlBuffer::Lock( GLenum p_access )
	{
		void * l_pReturn = NULL;

		if ( m_uiGlIndex != GL_INVALID_INDEX )
		{
			l_pReturn = m_pOpenGl->MapBuffer( m_target, p_access );
		}

		return l_pReturn;
	}

	void GlBuffer::Unlock()
	{
		if ( m_uiGlIndex != GL_INVALID_INDEX )
		{
			m_pOpenGl->UnmapBuffer( m_target );
		}
	}
}
