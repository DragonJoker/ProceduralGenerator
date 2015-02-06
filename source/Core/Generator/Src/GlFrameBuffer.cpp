#include "GlFrameBuffer.h"
#include "GlDownloadPixelBuffer.h"
#include "GlTexture.h"
#include "OpenGl.h"
#include "Buffer.h"

#define GL_BUFFER_OFFSET( n ) ((char* )NULL + ( n ) )

namespace ProceduralTextures
{
	GlFrameBuffer::GlFrameBuffer( OpenGl * p_pOpenGl )
		:	m_uiGlBuffer( GL_INVALID_INDEX )
		,	m_bInitialised( false )
		,	m_pOpenGl( p_pOpenGl )
	{
		m_pDownloadBuffers[0] = NULL;
		m_pDownloadBuffers[1] = NULL;
	}

	GlFrameBuffer::~GlFrameBuffer()
	{
	}

	bool GlFrameBuffer::Create()
	{
		m_pOpenGl->GenFramebuffers( 1, &m_uiGlBuffer );
		return m_uiGlBuffer != GL_INVALID_INDEX;
	}

	void GlFrameBuffer::Destroy()
	{
		if ( m_uiGlBuffer != GL_INVALID_INDEX )
		{
			m_pOpenGl->DeleteFramebuffers( 1, &m_uiGlBuffer );
			m_uiGlBuffer = GL_INVALID_INDEX;
		}
	}

	bool GlFrameBuffer::Initialise( wxSize const & p_size )
	{
		if ( !m_bInitialised && !m_mapAttachments.empty() )
		{
			m_sizeImage = p_size;
			m_pOpenGl->BindFramebuffer( GL_FRAMEBUFFER, m_uiGlBuffer );

			for ( std::map< GLenum, GlTexture * >::iterator l_it = m_mapAttachments.begin() ; l_it != m_mapAttachments.end() ; ++l_it )
			{
				m_pOpenGl->FramebufferTexture2D( GL_FRAMEBUFFER, l_it->first, GL_TEXTURE_2D, l_it->second->GetGlIndex(), 0 );
			}

			GLenum l_eStatus = m_pOpenGl->CheckFramebufferStatus( GL_FRAMEBUFFER );
			m_pOpenGl->BindFramebuffer( GL_FRAMEBUFFER, 0 );
			DoInitialisePbos();
			m_bInitialised = ( l_eStatus == GL_FRAMEBUFFER_COMPLETE );
		}

		return m_bInitialised;
	}

	void GlFrameBuffer::Cleanup()
	{
		m_bInitialised = false;
		DoCleanupPbos();
	}

	bool GlFrameBuffer::Activate()
	{
		return m_pOpenGl->BindFramebuffer( GL_FRAMEBUFFER, m_uiGlBuffer );
	}

	void GlFrameBuffer::Deactivate()
	{
		m_pOpenGl->BindFramebuffer( GL_FRAMEBUFFER, 0 );
	}

	bool GlFrameBuffer::ActivateTexture( GLenum p_attachment )
	{
		std::map< GLenum, GlTexture * >::iterator l_it = m_mapAttachments.find( p_attachment );
		bool l_bReturn = l_it != m_mapAttachments.end();

		if ( l_bReturn )
		{
			l_bReturn = l_it->second->Activate();
		}

		return l_bReturn;
	}

	void GlFrameBuffer::DeactivateTexture( GLenum p_attachment )
	{
		std::map< GLenum, GlTexture * >::iterator l_it = m_mapAttachments.find( p_attachment );

		if ( l_it != m_mapAttachments.end() )
		{
			l_it->second->Deactivate();
		}
	}

	void GlFrameBuffer::AttachTexture( GLenum p_attachment, GlTexture * p_pTexture )
	{
		m_mapAttachments[p_attachment] = p_pTexture;
	}

	GlTexture * GlFrameBuffer::GetTexture( GLenum p_attachment )
	{
		GlTexture * l_pReturn = NULL;
		std::map< GLenum, GlTexture * >::iterator l_it = m_mapAttachments.find( p_attachment );

		if ( l_it != m_mapAttachments.end() )
		{
			l_pReturn = l_it->second;
		}

		return l_pReturn;
	}

	bool GlFrameBuffer::DownloadSync( GLenum p_attachment, PixelBuffer & p_buffer )
	{
		bool l_bReturn = false;

		if ( m_pOpenGl->ReadBuffer( p_attachment ) )
		{
			l_bReturn = m_pOpenGl->ReadPixels( 0, 0, m_sizeImage.x, m_sizeImage.y, GL_RGBA, GL_UNSIGNED_BYTE, p_buffer.ptr() );
		}

		return l_bReturn;
	}

	bool GlFrameBuffer::DownloadAsync( GLenum p_attachment, PixelBuffer & p_buffer )
	{
		bool l_bReturn = false;

		if ( m_pOpenGl->ReadBuffer( p_attachment ) )
		{
			m_iCurrentDlPbo = ( m_iCurrentDlPbo + 1 ) % 2;
			int l_nextDlPbo = ( m_iCurrentDlPbo + 1 ) % 2;
			GlDownloadPixelBuffer * pBufferOut = m_pDownloadBuffers[m_iCurrentDlPbo];
			GlDownloadPixelBuffer * pBufferIn = m_pDownloadBuffers[l_nextDlPbo];

			if ( pBufferIn && pBufferOut )
			{
				if ( pBufferOut->Activate() )
				{
					m_pOpenGl->ReadPixels( 0, 0, m_sizeImage.x, m_sizeImage.y, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

					if ( pBufferIn->Activate() )
					{
						void * pData = pBufferIn->Lock( GL_READ_ONLY );

						if ( pData )
						{
							memcpy( p_buffer.ptr(), pData, p_buffer.size() );
							pBufferIn->Unlock();
							l_bReturn = true;
						}

						pBufferIn->Deactivate();
					}

					pBufferOut->Deactivate();
				}
			}
		}

		return l_bReturn;
	}

	void GlFrameBuffer::Resize( const wxSize & p_size )
	{
		m_size = p_size;
		m_bInitialised = false;
	}

	void GlFrameBuffer::DoCleanupPbos()
	{
		if ( m_pDownloadBuffers[0] )
		{
			m_pDownloadBuffers[0]->Destroy();
			delete m_pDownloadBuffers[0];
			m_pDownloadBuffers[0] = NULL;
		}

		if ( m_pDownloadBuffers[1] )
		{
			m_pDownloadBuffers[1]->Destroy();
			delete m_pDownloadBuffers[1];
			m_pDownloadBuffers[1] = NULL;
		}
	}

	void GlFrameBuffer::DoInitialisePbos()
	{
		if ( !m_pDownloadBuffers[0] )
		{
			m_pDownloadBuffers[0] = new GlDownloadPixelBuffer( m_pOpenGl );
			m_pDownloadBuffers[0]->Create();
			m_pDownloadBuffers[0]->Initialise( m_sizeImage.x * m_sizeImage.y * 4 );
		}

		if ( !m_pDownloadBuffers[0] )
		{
			m_pDownloadBuffers[1] = new GlDownloadPixelBuffer( m_pOpenGl );
			m_pDownloadBuffers[1]->Create();
			m_pDownloadBuffers[1]->Initialise( m_sizeImage.x * m_sizeImage.y * 4 );
		}
	}
}
