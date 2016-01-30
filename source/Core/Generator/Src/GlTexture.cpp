#include "GlTexture.h"
#include "GlUploadPixelBuffer.h"
#include "GlDownloadPixelBuffer.h"
#include "OpenGl.h"
#include "Buffer.h"

namespace ProceduralTextures
{
	namespace gl
	{
		Texture::Texture( OpenGl & p_openGl )
			: Object( p_openGl,
					  std::bind( &OpenGl::GenTextures, std::ref( p_openGl ), std::placeholders::_1, std::placeholders::_2 ),
					  std::bind( &OpenGl::DeleteTextures, std::ref( p_openGl ), std::placeholders::_1, std::placeholders::_2 ),
					  std::bind( &OpenGl::IsTexture, std::ref( p_openGl ), std::placeholders::_1 ),
					  std::bind( &OpenGl::BindTexture, std::ref( p_openGl ), GL_TEXTURE_2D, std::placeholders::_1 )
					)
			, m_bInitialised( false )
			, m_iCurrentUlPbo( 0 )
			, m_iCurrentDlPbo( 0 )
			, m_resized( false )
		{
		}

		Texture::~Texture()
		{
			DoDeleteBuffers();
		}

		bool Texture::Initialise( std::shared_ptr< PixelBuffer > p_pBuffer )
		{
			if ( !m_bInitialised || m_resized )
			{
				Cleanup();
				m_pPixels = p_pBuffer;
				m_size = m_pPixels->GetDimensions();
				DoInitialisePbos();
				DoInitialise();
				m_bInitialised = true;
				m_resized = false;
			}

			return m_bInitialised;
		}

		bool Texture::Initialise( Size const & p_size )
		{
			if ( !m_bInitialised || m_resized )
			{
				Cleanup();
				m_size = p_size;
				m_pPixels = std::make_shared< PixelBuffer >( p_size );
				DoInitialisePbos();
				DoInitialise();
				m_bInitialised = true;
				m_resized = false;
			}

			return m_bInitialised;
		}

		void Texture::Cleanup()
		{
			m_bInitialised = false;
			DoCleanupPbos();
			DoDeleteBuffers();
		}

		bool Texture::Activate( uint32_t p_texTarget, bool p_bUpload )
		{
			bool l_bReturn = GetOpenGl().ActiveTexture( p_texTarget );
			l_bReturn &= Bind();

			if ( m_resized )
			{
				GetOpenGl().TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x(), m_size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
				m_resized = false;
			}

			if ( p_bUpload )
			{
				UploadAsync();
			}

			return l_bReturn;
		}

		void Texture::Deactivate( uint32_t p_texTarget )
		{
			GetOpenGl().ActiveTexture( p_texTarget );
			Unbind();
		}

		void Texture::UploadAsync()
		{
			m_iCurrentUlPbo = ( m_iCurrentUlPbo + 1 ) % 2;
			int l_nextUlPbo = ( m_iCurrentUlPbo + 1 ) % 2;
			std::unique_ptr< UploadPixelBuffer > & pBufferOut = m_pUploadBuffers[m_iCurrentUlPbo];
			std::unique_ptr< UploadPixelBuffer > & pBufferIn = m_pUploadBuffers[l_nextUlPbo];

			if ( pBufferIn && pBufferOut )
			{
				if ( pBufferOut->Bind() )
				{
					GetOpenGl().TexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_size.x(), m_size.y(), GL_RGBA, GL_UNSIGNED_BYTE, NULL );

					if ( pBufferIn->Bind() )
					{
						pBufferIn->Data( NULL, m_pPixels->GetSize() );
						void * pData = pBufferIn->Lock( GL_WRITE_ONLY );

						if ( pData )
						{
							memcpy( pData, m_pPixels->ConstPtr(), m_pPixels->GetSize() );
							pBufferIn->Unlock();
						}

						pBufferIn->Unbind();
					}

					pBufferOut->Unbind();
				}
			}
		}

		void Texture::UploadSync()
		{
			GetOpenGl().TexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_size.x(), m_size.y(), GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels->ConstPtr() );
		}

		void Texture::DownloadAsync()
		{
			m_iCurrentDlPbo = ( m_iCurrentDlPbo + 1 ) % 2;
			int l_nextDlPbo = ( m_iCurrentDlPbo + 1 ) % 2;
			std::unique_ptr< DownloadPixelBuffer > & pBufferOut = m_pDownloadBuffers[m_iCurrentDlPbo];
			std::unique_ptr< DownloadPixelBuffer > & pBufferIn = m_pDownloadBuffers[l_nextDlPbo];

			if ( pBufferIn && pBufferOut )
			{
				if ( pBufferOut->Bind() )
				{
					GetOpenGl().GetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

					if ( pBufferIn->Bind() )
					{
						pBufferIn->Data( NULL, m_pPixels->GetSize() );
						void * pData = pBufferIn->Lock( GL_READ_ONLY );

						if ( pData )
						{
							memcpy( m_pPixels->Ptr(), pData, m_pPixels->GetSize() );
							pBufferIn->Unlock();
						}

						pBufferIn->Unbind();
					}

					pBufferOut->Unbind();
				}
			}
		}

		void Texture::DownloadSync()
		{
			GetOpenGl().GetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels->Ptr() );
		}

		void Texture::Resize( Size const & p_size )
		{
			m_resized |= m_size != p_size;
			m_size = p_size;
		}

		void Texture::DoInitialise()
		{
			bool l_bResult = Bind();

			if ( l_bResult )
			{
				l_bResult &= GetOpenGl().TexParameter( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
				l_bResult &= GetOpenGl().TexParameter( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
				l_bResult &= GetOpenGl().TexParameter( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
				l_bResult &= GetOpenGl().TexParameter( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				l_bResult &= GetOpenGl().TexParameter( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
				l_bResult &= GetOpenGl().TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x(), m_size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels->ConstPtr() );
				Unbind();
			}
		}

		void Texture::DoDeleteBuffers()
		{
			if ( m_pUploadBuffers[0] )
			{
				m_pUploadBuffers[0].reset();
			}

			if ( m_pUploadBuffers[1] )
			{
				m_pUploadBuffers[1].reset();
			}

			if ( m_pDownloadBuffers[0] )
			{
				m_pDownloadBuffers[0].reset();
			}

			if ( m_pDownloadBuffers[1] )
			{
				m_pDownloadBuffers[1].reset();
			}

			m_pPixels.reset();
		}

		void Texture::DoCleanupPbos()
		{
			if ( m_pUploadBuffers[0] )
			{
				m_pUploadBuffers[0]->Cleanup();
			}

			if ( m_pUploadBuffers[1] )
			{
				m_pUploadBuffers[1]->Cleanup();
			}

			if ( m_pDownloadBuffers[0] )
			{
				m_pDownloadBuffers[0]->Cleanup();
			}

			if ( m_pDownloadBuffers[1] )
			{
				m_pDownloadBuffers[1]->Cleanup();
			}
		}

		void Texture::DoInitialisePbos()
		{
			m_pUploadBuffers[0] = std::make_unique< UploadPixelBuffer >( GetOpenGl() );
			m_pUploadBuffers[0]->Initialise( m_pPixels->GetSize() );
			m_pUploadBuffers[1] = std::make_unique< UploadPixelBuffer >( GetOpenGl() );
			m_pUploadBuffers[1]->Initialise( m_pPixels->GetSize() );
			m_pDownloadBuffers[0] = std::make_unique< DownloadPixelBuffer >( GetOpenGl() );
			m_pDownloadBuffers[0]->Initialise( m_pPixels->GetSize() );
			m_pDownloadBuffers[1] = std::make_unique< DownloadPixelBuffer >( GetOpenGl() );
			m_pDownloadBuffers[1]->Initialise( m_pPixels->GetSize() );
		}
	}
}
