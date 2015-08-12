#include "GpuStep.h"

#include "Buffer.h"
#include "Generator.h"
#include "GeneratorUtils.h"
#include "GeometryBuffers.h"
#include "GlFrameBuffer.h"
#include "GlShaderObject.h"
#include "GlShaderProgram.h"
#include "GlTexture.h"
#include "OpenGl.h"
#include "OverlayManager.h"

namespace ProceduralTextures
{
	static String VertexShader =
		_T( "attribute vec2 vertex;\n" )
		_T( "attribute vec2 texture;\n" )
		_T( "varying vec2 pxl_texture;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "    pxl_texture = texture;\n" )
		_T( "    gl_Position = vec4( vertex, 0.0, 1.0 );\n" )
		_T( "}\n" );

	static String PixelShader =
		_T( "varying vec2 pxl_texture;\n" )
		_T( "uniform sampler2D pxl_mapTexture;\n" )
		_T( "void main()\n" )
		_T( "{\n" )
		_T( "    gl_FragColor = texture2D( pxl_mapTexture, pxl_texture );\n" )
		_T( "}\n" );

	GpuStep::GpuStep( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size, Size const & p_bordersSize )
		: m_generator( p_generator )
		, m_openGl( std::make_shared< gl::OpenGl >() )
		, m_sizeImage( p_size )
		, m_sizeDisplay( p_size )
		, m_bordersSize( p_bordersSize )
		, m_initialised( false )
		, m_finalBuffer( std::make_shared< PixelBuffer >( p_size ) )
		, m_saveBuffer( std::make_shared< PixelBuffer >( p_size ) )
		, m_toSave( false )
		, m_vertexAttribLocation( GL_INVALID_INDEX )
		, m_textureAttribLocation( GL_INVALID_INDEX )
	{
		m_overlayManager = std::make_shared< OverlayManager >( m_openGl );
		m_texture = std::make_shared< gl::Texture >( m_openGl );
		m_textureButton = std::make_shared< gl::Texture >( m_openGl );
		m_textureButtonMouseOver = std::make_shared< gl::Texture >( m_openGl );
		m_frameBuffer = std::make_shared< gl::FrameBuffer >( m_openGl );
		m_textureSave = std::make_shared< gl::Texture >( m_openGl );
		m_textureOverlay = std::make_shared< gl::Texture >( m_openGl );
		m_frameBufferSave = std::make_shared< gl::FrameBuffer >( m_openGl );
		m_frameBufferOverlay = std::make_shared< gl::FrameBuffer >( m_openGl );
		m_geometryBuffers = std::make_shared< GeometryBuffers >( m_openGl, GL_STATIC_DRAW );
		m_program = std::make_shared< gl::ShaderProgram >( m_openGl );
		m_texture->Resize( p_size );
		m_frameBuffer->Resize( p_size );
		m_frameBuffer->AttachTexture( GL_COLOR_ATTACHMENT0, m_texture );
		m_frameBufferSave->AttachTexture( GL_COLOR_ATTACHMENT0, m_textureSave );
		m_frameBufferOverlay->AttachTexture( GL_COLOR_ATTACHMENT0, m_textureOverlay );
		m_program->CreateObject( gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_program->CreateObject( gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_program->SetProgramText( VertexShader, gl::eSHADER_OBJECT_TYPE_VERTEX );
		m_program->SetProgramText( PixelShader, gl::eSHADER_OBJECT_TYPE_PIXEL );
		m_uniformImage = m_program->CreateIntFrameVariable( _T( "pxl_mapTexture" ) );
	}

	GpuStep::~GpuStep()
	{
		m_textureButton.reset();
		m_textureButtonMouseOver.reset();
		m_texture.reset();
		m_frameBuffer.reset();
		m_textureSave.reset();
		m_textureOverlay.reset();
		m_frameBufferSave.reset();
		m_frameBufferOverlay.reset();
		m_geometryBuffers.reset();
		m_program.reset();
		m_overlayManager.reset();
		m_openGl.reset();
		m_finalBuffer->Clear();
		m_saveBuffer->Clear();
	}

	void GpuStep::Initialise()
	{
		m_openGl->Initialise();
		m_openGl->ClearColor( 0.0, 0.0, 0.0, 1.0 );
		m_openGl->Enable( GL_TEXTURE_2D );
		m_textureButton->Create();
		m_textureButtonMouseOver->Create();
		m_texture->Create();
		m_frameBuffer->Create();
		m_textureSave->Create();
		m_textureOverlay->Create();
		m_frameBufferSave->Create();
		m_frameBufferOverlay->Create();
		m_geometryBuffers->Initialise();
		m_program->Initialise();
		m_vertexAttribLocation = m_program->GetAttributeLocation( _T( "vertex" ) );
		m_textureAttribLocation = m_program->GetAttributeLocation( _T( "texture" ) );
		m_overlayManager->Initialise();
		DoInitialise();
		m_initialised = true;
	}

	void GpuStep::Cleanup()
	{
		DoCleanup();
		m_overlayManager->Cleanup();

		m_program->Cleanup();
		m_texture->Cleanup();
		m_textureSave->Cleanup();
		m_frameBuffer->Cleanup();
		m_frameBufferSave->Cleanup();
		m_textureButton->Cleanup();
		m_textureButtonMouseOver->Cleanup();

		m_geometryBuffers->Cleanup();
		m_texture->Destroy();
		m_textureSave->Destroy();
		m_frameBuffer->Destroy();
		m_frameBufferSave->Destroy();
		m_textureButton->Destroy();
		m_textureButtonMouseOver->Destroy();

		m_openGl->Cleanup();
	}

	void GpuStep::Render( std::function< void() > p_swapFunction )
	{
		Clock::time_point l_beforeTime = Clock::now();
		PreRender();
		Render( true );
		PostRender();
		glFlush();
		p_swapFunction();
		m_time = std::chrono::duration_cast< std::chrono::milliseconds >( Clock::now() - l_beforeTime );
	}

	void GpuStep::Resize( int p_iWidth, int p_iHeight )
	{
		Resize( Size( p_iWidth, p_iHeight ) );
	}

	void GpuStep::Resize( Size const & p_size )
	{
		m_finalBuffer->Initialise( p_size );
		m_saveBuffer->Initialise( p_size );
		m_sizeImage = p_size;
		m_sizeDisplay = p_size;
		Size l_sizeConfig;
		Size l_screenSize;
		System::GetScreenSize( 0, l_screenSize );
		Size l_maxSize = l_screenSize - m_bordersSize;
		double l_dRatio = double( p_size.x() ) / double( p_size.y() );
		bool l_bChanged = false;
		l_maxSize = Size(	std::min< int >( l_maxSize.x(), int( l_maxSize.y() * l_dRatio ) ),
							std::min< int >( l_maxSize.y(), int( l_maxSize.x() / l_dRatio ) ) );

		while ( m_sizeDisplay.x() > l_maxSize.x() || m_sizeDisplay.y() > l_maxSize.y() )
		{
			if ( m_sizeDisplay.x() > l_maxSize.x() )
			{
				m_sizeDisplay.x() = l_maxSize.x();
				m_sizeDisplay.y() = int( m_sizeDisplay.x() / l_dRatio );
			}
			else if ( m_sizeDisplay.y() > l_maxSize.y() )
			{
				m_sizeDisplay.y() = l_maxSize.y();
				m_sizeDisplay.x() = int( m_sizeDisplay.y() * l_dRatio );
			}

			l_bChanged = true;
		}

		m_texture->Resize( p_size );
		m_frameBuffer->Resize( p_size );
		m_textureSave->Resize( p_size );
		m_frameBufferSave->Resize( p_size );
		m_textureOverlay->Resize( m_sizeDisplay );
		m_frameBufferOverlay->Resize( m_sizeDisplay );
		DoResize();
	}

	void GpuStep::UpdateDisplaySize( Size const & p_size )
	{
		double l_dRatio = double( m_sizeDisplay.x() ) / double( m_sizeDisplay.y() );
		m_sizeDisplay = Size(	std::min< int >( p_size.x(), int( p_size.y() * l_dRatio ) ),
								std::min< int >( p_size.y(), int( p_size.x() / l_dRatio ) ) );
		m_displayOffset = Position( ( p_size.x() - m_sizeDisplay.x() ) / 2, ( p_size.y() - m_sizeDisplay.y() ) / 2 );
		m_textureOverlay->Resize( m_sizeDisplay );
		m_frameBufferOverlay->Resize( m_sizeDisplay );
	}

	void GpuStep::SaveFrame()
	{
		m_toSave = true;
	}

	void GpuStep::UpdateBuffer( PixelBuffer const & p_buffer )
	{
		m_finalBuffer->Set( p_buffer );
	}

	void GpuStep::DoSubRender( uint32_t p_vertex, uint32_t p_texture )
	{
		m_geometryBuffers->Draw( p_vertex, p_texture );
	}

	void GpuStep::DoRenderFrameBuffer( gl::FrameBuffer & p_frameBuffer )
	{
		m_frameBuffer->Bind();
		m_openGl->Clear( GL_COLOR_BUFFER_BIT );
		m_program->Activate();
		p_frameBuffer.GetTexture( GL_COLOR_ATTACHMENT0 )->Activate();
		DoSubRender( m_vertexAttribLocation, m_textureAttribLocation );
		p_frameBuffer.GetTexture( GL_COLOR_ATTACHMENT0 )->Deactivate();
		m_program->Deactivate();
		m_frameBuffer->Unbind();
	}

	void GpuStep::DoInitBuffers( Size const & p_size )
	{
		m_finalBuffer->Initialise( p_size );
		m_saveBuffer->Initialise( p_size );
	}

	void GpuStep::PreRender()
	{
		Size l_imageSize = m_finalBuffer->GetDimensions();
		m_texture->Initialise( m_finalBuffer );
		m_frameBuffer->Initialise( l_imageSize );
		m_textureSave->Initialise( l_imageSize );
		m_frameBufferSave->Initialise( l_imageSize );
		m_textureOverlay->Initialise( m_sizeDisplay );
		m_frameBufferOverlay->Initialise( m_sizeDisplay );

		if ( m_generator.lock()->GetButtonImage() )
		{
			m_textureButton->Initialise( m_generator.lock()->GetButtonImage() );
		}

		if ( m_generator.lock()->GetButtonMouseOverImage() )
		{
			m_textureButtonMouseOver->Initialise( m_generator.lock()->GetButtonMouseOverImage() );
		}

		DoPreRender();
	}

	void GpuStep::Render( bool p_bChanged )
	{
		DoRender( p_bChanged );
		Size l_imageSize = m_finalBuffer->GetDimensions();

		if ( m_toSave )
		{
			// Render into the save framebuffer, At image size
			m_openGl->Viewport( 0, 0, l_imageSize.x(), l_imageSize.y() );
			m_frameBufferSave->Bind();
			m_openGl->Clear( GL_COLOR_BUFFER_BIT );
			m_program->Activate();
			m_texture->Activate( GL_TEXTURE0, p_bChanged );
			DoSubRender( m_vertexAttribLocation, m_textureAttribLocation );
			m_texture->Deactivate();
			m_program->Deactivate();
			DoSaveFrame();
			m_frameBufferSave->Unbind();
			p_bChanged = false;
		}

		// Render into the overlays texture
		m_openGl->Viewport( 0, 0, m_sizeDisplay.x(), m_sizeDisplay.y() );
		m_frameBufferOverlay->Bind();
		m_openGl->Clear( GL_COLOR_BUFFER_BIT );
		m_program->Activate();
		m_texture->Activate( GL_TEXTURE0, p_bChanged );
		DoSubRender( m_vertexAttribLocation, m_textureAttribLocation );
		m_overlayManager->Render( m_sizeDisplay );
		m_texture->Deactivate();
		m_program->Deactivate();
		m_frameBufferOverlay->Unbind();
		p_bChanged = false;

		// Direct rendering into the window, At display size
		m_openGl->Viewport( m_displayOffset.x(), m_displayOffset.y(), m_sizeDisplay.x(), m_sizeDisplay.y() );
		m_openGl->Clear( GL_COLOR_BUFFER_BIT );
		m_program->Activate();
		m_textureOverlay->Activate();
		DoSubRender( m_vertexAttribLocation, m_textureAttribLocation );
		m_textureOverlay->Deactivate();
		m_program->Deactivate();
	}

	void GpuStep::PostRender()
	{
		DoPostRender();
	}

	void GpuStep::DoSaveFrame()
	{
		if ( m_toSave )
		{
			m_frameBufferSave->DownloadAsync( GL_COLOR_ATTACHMENT0, *m_finalBuffer );
			m_saveBuffer->Set( *m_finalBuffer );
			m_toSave = false;
		}
	}

	//************************************************************************************************

	DefaultGpuStep::DefaultGpuStep( std::shared_ptr< GeneratorBase > p_generator, Size const & p_size, Size const & p_bordersSize )
		: GpuStep( p_generator, p_size, p_bordersSize )
	{
	}

	DefaultGpuStep::~DefaultGpuStep()
	{
	}

	void DefaultGpuStep::DoInitialise()
	{
	}

	void DefaultGpuStep::DoCleanup()
	{
	}

	void DefaultGpuStep::DoResize()
	{
	}

	void DefaultGpuStep::DoPreRender()
	{
	}

	void DefaultGpuStep::DoRender( bool & )
	{
	}

	void DefaultGpuStep::DoPostRender()
	{
	}
}
