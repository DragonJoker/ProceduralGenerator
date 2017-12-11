#include "GpuLifeGameGpuStep.h"

#include <GlFrameBuffer.h>
#include <GlTexture.h>

#include <random>

namespace GpuLifeGame
{
	GpuStep::GpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize )
		: ProceduralTextures::GpuStep( p_generator, p_size, p_bordersSize )
		, m_frameBuffers
		{
			std::make_shared< ProceduralTextures::gl::FrameBuffer >( *m_openGl ),
			std::make_shared< ProceduralTextures::gl::FrameBuffer >( *m_openGl )
		}
		, m_textures
		{
			std::make_shared< ProceduralTextures::gl::Texture >( *m_openGl ),
			std::make_shared< ProceduralTextures::gl::Texture >( *m_openGl ),
		}
	{
		m_textures[0]->Resize( p_size );
		m_textures[1]->Resize( p_size );
		m_frameBuffers[0]->Resize( p_size );
		m_frameBuffers[1]->Resize( p_size );
		m_frameBuffers[0]->AttachTexture( GL_COLOR_ATTACHMENT0, m_textures[0] );
		m_frameBuffers[1]->AttachTexture( GL_COLOR_ATTACHMENT0, m_textures[1] );
		m_effect = std::make_unique< Effect >( *m_openGl, m_sizeImage.x(), m_sizeImage.y() );

		std::random_device l_randDevice;
		std::default_random_engine l_randEngine( l_randDevice() );
		std::uniform_int_distribution< int > l_randDistribution( 0, 1 );

		for ( uint32_t i = 0; i < p_size.y(); i++ )
		{
			for ( uint32_t j = 0; j < p_size.x(); j++ )
			{
				auto l_alive = l_randDistribution( l_randEngine ) == 0;
				( *m_finalBuffer )[i][j].r = ( *m_finalBuffer )[i][j].g = ( *m_finalBuffer )[i][j].b = ( *m_finalBuffer )[i][j].a = l_alive ? 255 : 0;
			}
		}
	}

	GpuStep::~GpuStep()
	{
		m_effect.reset();
	}

	void GpuStep::ResetTime()
	{
		m_effect->ResetTime();
	}

	void GpuStep::DoInitialise()
	{
		m_textures[0]->Create();
		m_textures[1]->Create();
		m_frameBuffers[0]->Create();
		m_frameBuffers[1]->Create();
	}

	void GpuStep::DoCleanup()
	{
		m_effect->Cleanup();
		m_effect.reset();
		m_frameBuffers[0]->Cleanup();
		m_frameBuffers[1]->Cleanup();
		m_textures[0]->Cleanup();
		m_textures[1]->Cleanup();
		m_frameBuffers[0]->Destroy();
		m_frameBuffers[1]->Destroy();
		m_textures[0]->Destroy();
		m_textures[1]->Destroy();
	}

	void GpuStep::DoResize()
	{
		m_frameBuffers[0]->Resize( m_sizeImage );
		m_frameBuffers[1]->Resize( m_sizeImage );
		m_textures[0]->Resize( m_sizeImage );
		m_textures[1]->Resize( m_sizeImage );
	}

	void GpuStep::DoPreRender()
	{
		m_textures[0]->Initialise( m_finalBuffer );
		m_textures[1]->Initialise( m_sizeImage );
		m_frameBuffers[0]->Initialise( m_sizeImage );
		m_frameBuffers[1]->Initialise( m_sizeImage );

		if ( !m_effect->IsInitialised() )
		{
			m_effect->Initialise();
		}
	}

	void GpuStep::DoRender( bool & p_bChanged )
	{
		std::shared_ptr< ProceduralTextures::gl::FrameBuffer > l_frameBufferTexture = m_frameBuffers[m_index];
		m_index = ( m_index + 1 ) % 2;
		std::shared_ptr< ProceduralTextures::gl::FrameBuffer > l_frameBufferRender = m_frameBuffers[m_index];
		m_openGl->Viewport( 0, 0, m_sizeImage.x(), m_sizeImage.y() );

		if ( m_effect->IsInitialised() )
		{
			if ( l_frameBufferRender->Bind() )
			{
				if ( m_effect->Activate( ProceduralTextures::Position( 0, 0 ) ) )
				{
					m_openGl->Clear( GL_COLOR_BUFFER_BIT );
					l_frameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Activate();
					DoSubRender( m_effect->GetVertex(), m_effect->GetTexture() );
					l_frameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Deactivate();
					m_effect->Deactivate();
				}

				l_frameBufferRender->Unbind();
			}
		}

		DoRenderFrameBuffer( *l_frameBufferTexture );
		p_bChanged = false;
	}

	void GpuStep::DoPostRender()
	{
	}
}
