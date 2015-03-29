#include "SfxGpuStep.h"

#include <GlFrameBuffer.h>
#include <GlTexture.h>

namespace ShaderEffects
{
	GpuStep::GpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize )
		: ProceduralTextures::GpuStep( p_generator, p_size, p_bordersSize )
		, m_frameBuffer1( std::make_shared< ProceduralTextures::gl::FrameBuffer >( m_openGl ) )
		, m_frameBuffer2( std::make_shared< ProceduralTextures::gl::FrameBuffer >( m_openGl ) )
		, m_texture1( std::make_shared< ProceduralTextures::gl::Texture >( m_openGl ) )
		, m_texture2( std::make_shared< ProceduralTextures::gl::Texture >( m_openGl ) )
		, m_iSeparationOffset( 50 )
		, m_eSeparationType( eSEPARATION_NONE )
	{
		m_itSelectedEffect = m_arrayEffects.end();
		m_texture2->Resize( p_size );
		m_frameBuffer1->Resize( p_size );
		m_frameBuffer2->Resize( p_size );
		m_frameBuffer1->AttachTexture( GL_COLOR_ATTACHMENT0, m_texture1 );
		m_frameBuffer2->AttachTexture( GL_COLOR_ATTACHMENT0, m_texture2 );
	}

	GpuStep::~GpuStep()
	{
		m_arrayEffects.clear();
	}

	void GpuStep::ResetTime()
	{
		for ( std::unique_ptr< Effect > & l_effect : m_arrayEffects )
		{
			l_effect->ResetTime();
		}
	}

	void GpuStep::SetSepType( int p_value )
	{
		m_eSeparationType = eSEPARATION( p_value );
	}

	void GpuStep::SetSepOffset( int p_value )
	{
		m_iSeparationOffset = p_value;
	}

	void GpuStep::SelectEffect( uint32_t p_value )
	{
		m_itSelectedEffect = m_arrayEffects.begin() + p_value;
	}

	void GpuStep::AddEffect()
	{
		m_arrayEffects.push_back( std::make_unique< Effect >( m_openGl, m_finalBuffer ) );
		m_itSelectedEffect = m_arrayEffects.end();
	}

	void GpuStep::RemoveEffect()
	{
		m_arrayEffects.erase( m_itSelectedEffect );
		m_itSelectedEffect = m_arrayEffects.end();
	}

	void GpuStep::CompileEffect()
	{
		if ( m_itSelectedEffect != m_arrayEffects.end() )
		{
			( *m_itSelectedEffect )->Compile();
		}
	}

	ProceduralTextures::String GpuStep::GetCompilerLog()
	{
		ProceduralTextures::String l_strText;

		if ( m_itSelectedEffect != m_arrayEffects.end() )
		{
			ProceduralTextures::String l_strVertexLog = ( *m_itSelectedEffect )->GetCompilerLog( ProceduralTextures::gl::eSHADER_OBJECT_TYPE_VERTEX );
			ProceduralTextures::String l_strFragmentLog = ( *m_itSelectedEffect )->GetCompilerLog( ProceduralTextures::gl::eSHADER_OBJECT_TYPE_PIXEL );
			ProceduralTextures::String l_strLinkerLog = ( *m_itSelectedEffect )->GetLinkerLog();

			if ( ! l_strVertexLog.empty() )
			{
				l_strText += _( "Vertex Shader :" ) + ProceduralTextures::String( _T( "\n" ) ) + l_strVertexLog;
			}

			if ( ! l_strFragmentLog.empty() )
			{
				if ( ! l_strText.empty() )
				{
					l_strText += _T( "\n\n" );
				}

				l_strText += _( "Fragment Shader :" ) + ProceduralTextures::String( _T( "\n" ) ) + l_strFragmentLog + _T( "\n\n" );
			}

			if ( ! l_strLinkerLog.empty() )
			{
				if ( ! l_strText.empty() )
				{
					l_strText += _T( "\n\n" );
				}

				l_strText += _( "Linker :" ) + ProceduralTextures::String( _T( "\n" ) ) + l_strLinkerLog + _T( "\n\n" );
			}

			if ( l_strText.empty() )
			{
				l_strText = _( "No error" );
			}
		}

		return l_strText;
	}

	void GpuStep::SetVertexShaderPath( ProceduralTextures::String const & p_path )
	{
		if ( m_itSelectedEffect != m_arrayEffects.end() && !p_path.empty() )
		{
			( *m_itSelectedEffect )->SetVertexFile( p_path );
		}
	}

	void GpuStep::SetFragmentShaderPath( ProceduralTextures::String const & p_path )
	{
		if ( m_itSelectedEffect != m_arrayEffects.end() && !p_path.empty() )
		{
			( *m_itSelectedEffect )->SetFragmentFile( p_path );
		}
	}

	void GpuStep::SetImage( ProceduralTextures::PixelBuffer const & p_image )
	{
		m_initialised = false;

		if ( m_arrayEffects.size() == 0 )
		{
			AddEffect();
		}
		else
		{
			for ( std::unique_ptr< Effect > & l_effect : m_arrayEffects )
			{
				l_effect->UpdateImages();
			}
		}

		m_initialised = true;
	}

	void GpuStep::DoInitialise()
	{
		m_texture1->Create();
		m_texture2->Create();
		m_frameBuffer1->Create();
		m_frameBuffer2->Create();
	}

	void GpuStep::DoCleanup()
	{
		for ( std::unique_ptr< Effect > & l_effect : m_arrayEffects )
		{
			l_effect->Cleanup();
		}

		m_arrayEffects.clear();
		m_frameBuffer1->Cleanup();
		m_frameBuffer2->Cleanup();
		m_texture1->Cleanup();
		m_texture2->Cleanup();
		m_frameBuffer1->Destroy();
		m_frameBuffer2->Destroy();
		m_texture1->Destroy();
		m_texture2->Destroy();
	}

	void GpuStep::DoResize()
	{
		m_frameBuffer1->Resize( m_sizeImage );
		m_frameBuffer2->Resize( m_sizeImage );
		m_texture1->Resize( m_sizeImage );
		m_texture2->Resize( m_sizeImage );
	}

	void GpuStep::DoPreRender()
	{
		m_texture1->Initialise( m_finalBuffer );
		m_texture2->Initialise( m_sizeImage );
		m_frameBuffer1->Initialise( m_sizeImage );
		m_frameBuffer2->Initialise( m_sizeImage );

		for ( std::unique_ptr< Effect > & l_effect : m_arrayEffects )
		{
			if ( !l_effect->IsInitialised() )
			{
				l_effect->Initialise();
			}
		}

		m_texture1->Activate();
		m_texture1->UploadAsync();
		m_texture1->Deactivate();
	}

	void GpuStep::DoRender( bool & p_bChanged )
	{
		std::shared_ptr< ProceduralTextures::gl::FrameBuffer > l_pFrameBufferTexture = m_frameBuffer1;
		std::shared_ptr< ProceduralTextures::gl::FrameBuffer > l_pFrameBufferRender = m_frameBuffer2;
		m_openGl->Viewport( 0, 0, m_sizeImage.x(), m_sizeImage.y() );

		for ( std::unique_ptr< Effect > & l_effect : m_arrayEffects )
		{
			if ( l_effect->IsInitialised() )
			{
				if ( l_pFrameBufferRender->Bind() )
				{
					if ( l_effect->Activate( m_eSeparationType, m_iSeparationOffset ) )
					{
						m_openGl->Clear( GL_COLOR_BUFFER_BIT );
						l_pFrameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Activate();
						DoSubRender( l_effect->GetVertex(), l_effect->GetTexture() );
						l_pFrameBufferTexture->GetTexture( GL_COLOR_ATTACHMENT0 )->Deactivate();
						l_effect->Deactivate();
					}

					l_pFrameBufferRender->Unbind();
				}

				std::swap( l_pFrameBufferTexture, l_pFrameBufferRender );
			}
		}

		DoRenderFrameBuffer( *l_pFrameBufferTexture );
		p_bChanged = false;
	}

	void GpuStep::DoPostRender()
	{
	}
}
