#include "GlFrameBuffer.h"
#include "GlDownloadPixelBuffer.h"
#include "GlTexture.h"
#include "OpenGl.h"
#include "Buffer.h"

#define GL_BUFFER_OFFSET( n ) ((char* )NULL + ( n ) )

namespace ProceduralTextures
{
	namespace gl
	{
		FrameBuffer::FrameBuffer( OpenGl & p_openGl )
			: Object( p_openGl,
					  std::bind( &OpenGl::GenFramebuffers, std::ref( p_openGl ), std::placeholders::_1, std::placeholders::_2 ),
					  std::bind( &OpenGl::DeleteFramebuffers, std::ref( p_openGl ), std::placeholders::_1, std::placeholders::_2 ),
					  std::bind( &OpenGl::IsFramebuffer, std::ref( p_openGl ), std::placeholders::_1 ),
					  std::bind( &OpenGl::BindFramebuffer, std::ref( p_openGl ), GL_FRAMEBUFFER, std::placeholders::_1 )
					)
			, m_bInitialised( false )
		{
		}

		FrameBuffer::~FrameBuffer()
		{
		}

		bool FrameBuffer::Initialise( Size const & p_size )
		{
			if ( !m_bInitialised && !m_mapAttachments.empty() )
			{
				m_sizeImage = p_size;
				Bind();

				for ( auto l_pair : m_mapAttachments )
				{
					GetOpenGl().FramebufferTexture2D( GL_FRAMEBUFFER, l_pair.first, GL_TEXTURE_2D, l_pair.second.lock()->GetGlName(), 0 );
				}

				GLenum l_eStatus = GetOpenGl().CheckFramebufferStatus( GL_FRAMEBUFFER );
				Unbind();
				DoInitialiseIoBuffers();
				m_bInitialised = ( l_eStatus == GL_FRAMEBUFFER_COMPLETE );
			}

			return m_bInitialised;
		}

		void FrameBuffer::Cleanup()
		{
			m_bInitialised = false;
			DoCleanupIoBuffers();
		}

		bool FrameBuffer::ActivateTexture( uint32_t p_attachment, uint32_t p_target )
		{
			auto l_it = m_mapAttachments.find( p_attachment );
			bool l_bReturn = l_it != m_mapAttachments.end();

			if ( l_bReturn )
			{
				l_bReturn = l_it->second.lock()->Activate( p_target );
			}

			return l_bReturn;
		}

		void FrameBuffer::DeactivateTexture( uint32_t p_attachment, uint32_t p_target )
		{
			auto l_it = m_mapAttachments.find( p_attachment );

			if ( l_it != m_mapAttachments.end() )
			{
				l_it->second.lock()->Deactivate( p_target );
			}
		}

		void FrameBuffer::AttachTexture( uint32_t p_attachment, std::shared_ptr< Texture > p_pTexture )
		{
			m_mapAttachments[p_attachment] = p_pTexture;
		}

		std::shared_ptr< Texture > FrameBuffer::GetTexture( uint32_t p_attachment )
		{
			std::shared_ptr< Texture > l_pReturn;
			auto l_it = m_mapAttachments.find( p_attachment );

			if ( l_it != m_mapAttachments.end() )
			{
				l_pReturn = l_it->second.lock();
			}

			return l_pReturn;
		}

		bool FrameBuffer::DownloadSync( uint32_t p_attachment, PixelBuffer & p_buffer )
		{
			bool l_bReturn = false;

			if ( GetOpenGl().ReadBuffer( p_attachment ) )
			{
				l_bReturn = GetOpenGl().ReadPixels( 0, 0, m_sizeImage.x(), m_sizeImage.y(), GL_RGBA, GL_UNSIGNED_BYTE, p_buffer.Ptr() );
			}

			return l_bReturn;
		}

		bool FrameBuffer::DownloadAsync( uint32_t p_attachment, PixelBuffer & p_buffer )
		{
			bool l_bReturn = false;

			if ( GetOpenGl().ReadBuffer( p_attachment ) )
			{
				m_iCurrentDlPbo = ( m_iCurrentDlPbo + 1 ) % 2;
				int l_nextDlPbo = ( m_iCurrentDlPbo + 1 ) % 2;
				std::unique_ptr< DownloadPixelBuffer > & pBufferOut = m_pDownloadBuffers[m_iCurrentDlPbo];

				if ( pBufferOut )
				{
					if ( pBufferOut->Bind() )
					{
						GetOpenGl().ReadPixels( 0, 0, m_sizeImage.x(), m_sizeImage.y(), GL_RGBA, GL_UNSIGNED_BYTE, 0 );
						void * pData = pBufferOut->Lock( GL_READ_ONLY );

						if ( pData )
						{
							memcpy( p_buffer.Ptr(), pData, p_buffer.GetSize() );
							pBufferOut->Unlock();
							l_bReturn = true;
						}

						pBufferOut->Unbind();
					}
				}
			}

			return l_bReturn;
		}

		void FrameBuffer::Resize( Size const & p_size )
		{
			m_size = p_size;
			m_bInitialised = false;
		}

		void FrameBuffer::DoCleanupIoBuffers()
		{
			if ( m_pDownloadBuffers[0] )
			{
				m_pDownloadBuffers[0]->Cleanup();
				m_pDownloadBuffers[0].reset();
			}

			if ( m_pDownloadBuffers[1] )
			{
				m_pDownloadBuffers[1]->Cleanup();
				m_pDownloadBuffers[1].reset();
			}
		}

		void FrameBuffer::DoInitialiseIoBuffers()
		{
			if ( !m_pDownloadBuffers[0] )
			{
				m_pDownloadBuffers[0] = std::make_unique< DownloadPixelBuffer >( GetOpenGl() );
				m_pDownloadBuffers[0]->Initialise( m_sizeImage.x() * m_sizeImage.y() * 4 );
			}

			if ( !m_pDownloadBuffers[1] )
			{
				m_pDownloadBuffers[1] = std::make_unique< DownloadPixelBuffer >( GetOpenGl() );
				m_pDownloadBuffers[1]->Initialise( m_sizeImage.x() * m_sizeImage.y() * 4 );
			}
		}
	}
}
