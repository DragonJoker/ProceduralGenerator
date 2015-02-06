#include "GlTexture.h"
#include "GlUploadPixelBuffer.h"
#include "GlDownloadPixelBuffer.h"
#include "OpenGl.h"
#include "Buffer.h"

namespace ProceduralTextures
{
	GlTexture::GlTexture( OpenGl * p_pOpenGl )
		:	m_uiGlTexture( GL_INVALID_INDEX )
		,	m_bInitialised( false )
		,	m_pPixels( NULL )
		,	m_bOwnsPixels( true )
		,	m_pxColour( 1.0f, 1.0f, 1.0f, 1.0f )
		,	m_iCurrentUlPbo( 0 )
		,	m_iCurrentDlPbo( 0 )
		,	m_pOpenGl( p_pOpenGl )
	{
		m_pDownloadBuffers[0] = NULL;
		m_pDownloadBuffers[1] = NULL;
		m_pUploadBuffers[0] = NULL;
		m_pUploadBuffers[1] = NULL;
	}

	GlTexture::~GlTexture()
	{
		DoDeleteBuffers();
	}

	bool GlTexture::Create()
	{
		m_pOpenGl->GenTextures( 1, & m_uiGlTexture );
		return m_uiGlTexture != GL_INVALID_INDEX;
	}

	void GlTexture::Destroy()
	{
		if ( m_uiGlTexture != GL_INVALID_INDEX )
		{
			m_pOpenGl->DeleteTextures( 1, & m_uiGlTexture );
			m_uiGlTexture = GL_INVALID_INDEX;
		}
	}

	bool GlTexture::Initialise( PixelBuffer * p_pBuffer )
	{
		if ( !m_bInitialised )
		{
			Cleanup();
			m_pPixels = p_pBuffer;
			DoInitialisePbos();
			m_bOwnsPixels = false;
			DoInitialise();
			m_bInitialised = true;
		}

		return m_bInitialised;
	}

	bool GlTexture::Initialise( const wxSize & p_size )
	{
		if ( !m_bInitialised )
		{
			Cleanup();
			m_size = p_size;
			m_pPixels = new PixelBuffer( Size( p_size.x, p_size.y ) );
			DoInitialisePbos();
			m_bOwnsPixels = true;
			DoInitialise();
			m_bInitialised = true;
		}

		return m_bInitialised;
	}

	void GlTexture::Cleanup()
	{
		m_bInitialised = false;
		DoCleanupPbos();
		DoDeleteBuffers();
	}

	bool GlTexture::Activate( GLenum p_texTarget, GLenum p_mode, BlendMode * p_pBlendRgb, BlendMode * p_pBlendAlpha, bool p_bUpload )
	{
		bool l_bReturn = m_pOpenGl->ActiveTexture( p_texTarget );
		l_bReturn &= m_pOpenGl->Enable( GL_TEXTURE_2D );
		l_bReturn &= m_pOpenGl->AttachTexture( GL_TEXTURE_2D, m_uiGlTexture );

		if ( p_bUpload )
		{
			UploadAsync();
		}

		l_bReturn &= m_pOpenGl->TexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, & m_pxColour.r );
		l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, p_mode );

		if ( p_mode == GL_COMBINE )
		{
			if ( p_pBlendRgb && p_pBlendRgb->function != GL_NONE )
			{
				// RGB Combine
				l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB, p_pBlendRgb->function );

				if ( p_pBlendRgb->source1 != GL_NONE && p_pBlendRgb->operand1 != GL_NONE )
				{
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB, p_pBlendRgb->source1 );
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB, p_pBlendRgb->operand1 );
				}

				if ( p_pBlendRgb->function != GL_REPLACE && p_pBlendRgb->source2 != GL_NONE && p_pBlendRgb->operand2 != GL_NONE )
				{
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB, p_pBlendRgb->source2 );
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB, p_pBlendRgb->operand2 );
				}

				if ( p_pBlendRgb->function == GL_INTERPOLATE && p_pBlendRgb->source3 != GL_NONE && p_pBlendRgb->operand3 != GL_NONE )
				{
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_RGB, p_pBlendRgb->source3 );
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_RGB, p_pBlendRgb->operand3 );
				}
			}

			if ( p_pBlendAlpha && p_pBlendAlpha->function != GL_NONE )
			{
				// Alpha Combine
				l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA, p_pBlendAlpha->function );

				if ( p_pBlendAlpha->source1 != GL_NONE && p_pBlendAlpha->operand1 != GL_NONE )
				{
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, p_pBlendAlpha->source1 );
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, p_pBlendAlpha->operand1 );
				}

				if ( p_pBlendAlpha->function != GL_REPLACE && p_pBlendAlpha->source2 != GL_NONE && p_pBlendAlpha->operand2 != GL_NONE )
				{
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, p_pBlendAlpha->source2 );
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, p_pBlendAlpha->operand2 );
				}

				if ( p_pBlendAlpha->function == GL_INTERPOLATE && p_pBlendAlpha->source3 != GL_NONE && p_pBlendAlpha->operand3 != GL_NONE )
				{
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, p_pBlendAlpha->source3 );
					l_bReturn &= m_pOpenGl->TexEnvi( GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, p_pBlendAlpha->operand3 );
				}
			}
		}

		return l_bReturn;
	}

	void GlTexture::Deactivate( GLenum p_texTarget )
	{
		bool l_bReturn = m_pOpenGl->ActiveTexture( p_texTarget );
		l_bReturn &= m_pOpenGl->Enable( GL_TEXTURE_2D );
		l_bReturn &= m_pOpenGl->AttachTexture( GL_TEXTURE_2D, 0 );
	}

	void GlTexture::UploadAsync()
	{
		m_iCurrentUlPbo = ( m_iCurrentUlPbo + 1 ) % 2;
		int l_nextUlPbo = ( m_iCurrentUlPbo + 1 ) % 2;
		GlUploadPixelBuffer * pBufferOut = m_pUploadBuffers[m_iCurrentUlPbo];
		GlUploadPixelBuffer * pBufferIn = m_pUploadBuffers[l_nextUlPbo];

		if ( pBufferIn && pBufferOut )
		{
			if ( pBufferOut->Activate() )
			{
				m_pOpenGl->TexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

				if ( pBufferIn->Activate() )
				{
					pBufferIn->Data( NULL, m_pPixels->size() );
					void * pData = pBufferIn->Lock( GL_WRITE_ONLY );

					if ( pData )
					{
						memcpy( pData, m_pPixels->const_ptr(), m_pPixels->size() );
						pBufferIn->Unlock();
					}

					pBufferIn->Deactivate();
				}

				pBufferOut->Deactivate();
			}
		}
	}

	void GlTexture::UploadSync()
	{
		m_pOpenGl->TexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels->const_ptr() );
	}

	void GlTexture::DownloadAsync()
	{
		m_iCurrentDlPbo = ( m_iCurrentDlPbo + 1 ) % 2;
		int l_nextDlPbo = ( m_iCurrentDlPbo + 1 ) % 2;
		GlDownloadPixelBuffer * pBufferOut = m_pDownloadBuffers[m_iCurrentDlPbo];
		GlDownloadPixelBuffer * pBufferIn = m_pDownloadBuffers[l_nextDlPbo];
        
		if ( pBufferIn && pBufferOut )
		{
			if ( pBufferOut->Activate() )
			{
				m_pOpenGl->GetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

				if ( pBufferIn->Activate() )
				{
					pBufferIn->Data( NULL, m_pPixels->size() );
					void * pData = pBufferIn->Lock( GL_READ_ONLY );

					if ( pData )
					{
						memcpy( m_pPixels->ptr(), pData, m_pPixels->size() );
						pBufferIn->Unlock();
					}

					pBufferIn->Deactivate();
				}

				pBufferOut->Deactivate();
			}
		}
	}

	void GlTexture::DownloadSync()
	{
		m_pOpenGl->GetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels->ptr() );
	}

	void GlTexture::Resize( const wxSize & p_size )
	{
		m_size = p_size;
		m_bInitialised = false;
	}

	void GlTexture::DoInitialise()
	{
		bool l_bResult = m_pOpenGl->AttachTexture( GL_TEXTURE_2D, m_uiGlTexture );
		l_bResult &= m_pOpenGl->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		l_bResult &= m_pOpenGl->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		l_bResult &= m_pOpenGl->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		l_bResult &= m_pOpenGl->TexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		l_bResult &= m_pOpenGl->TexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
		l_bResult &= m_pOpenGl->TexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels->const_ptr() );
		l_bResult &= m_pOpenGl->AttachTexture( GL_TEXTURE_2D, 0 );
	}

	void GlTexture::DoDeleteBuffers()
	{
		if ( m_bOwnsPixels )
		{
			delete m_pPixels;
		}

		if ( m_pUploadBuffers[0] )
		{
			delete m_pUploadBuffers[0];
			m_pUploadBuffers[0] = NULL;
		}

		if ( m_pUploadBuffers[1] )
		{
			delete m_pUploadBuffers[1];
			m_pUploadBuffers[1] = NULL;
		}

		if ( m_pDownloadBuffers[0] )
		{
			delete m_pDownloadBuffers[0];
			m_pDownloadBuffers[0] = NULL;
		}

		if ( m_pDownloadBuffers[1] )
		{
			delete m_pDownloadBuffers[1];
			m_pDownloadBuffers[1] = NULL;
		}

		m_pPixels = NULL;
		m_bOwnsPixels = true;
	}

	void GlTexture::DoCleanupPbos()
	{
		if ( m_pUploadBuffers[0] )
		{
			m_pUploadBuffers[0]->Destroy();
		}

		if ( m_pUploadBuffers[1] )
		{
			m_pUploadBuffers[1]->Destroy();
		}

		if ( m_pDownloadBuffers[0] )
		{
			m_pDownloadBuffers[0]->Destroy();
		}

		if ( m_pDownloadBuffers[1] )
		{
			m_pDownloadBuffers[1]->Destroy();
		}
	}

	void GlTexture::DoInitialisePbos()
	{
		m_pUploadBuffers[0] = new GlUploadPixelBuffer( m_pOpenGl );
		m_pUploadBuffers[0]->Create();
		m_pUploadBuffers[0]->Initialise( m_pPixels->size() );
		m_pUploadBuffers[1] = new GlUploadPixelBuffer( m_pOpenGl );
		m_pUploadBuffers[0]->Create();
		m_pUploadBuffers[1]->Initialise( m_pPixels->size() );
		m_pDownloadBuffers[0] = new GlDownloadPixelBuffer( m_pOpenGl );
		m_pDownloadBuffers[0]->Create();
		m_pDownloadBuffers[0]->Initialise( m_pPixels->size() );
		m_pDownloadBuffers[1] = new GlDownloadPixelBuffer( m_pOpenGl );
		m_pDownloadBuffers[1]->Create();
		m_pDownloadBuffers[1]->Initialise( m_pPixels->size() );
	}
}
