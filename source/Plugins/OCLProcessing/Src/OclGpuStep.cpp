#include "OclGpuStep.h"

#include <GeneratorUtils.h>
#include <GlFrameBuffer.h>
#include <GlShaderProgram.h>
#include <GlTexture.h>
#include <Buffer.h>

namespace OCLProcessing
{
	GpuStep::GpuStep( std::shared_ptr< ProceduralTextures::GeneratorBase > p_generator, ProceduralTextures::Size const & p_size, ProceduralTextures::Size const & p_bordersSize )
		: ProceduralTextures::GpuStep( p_generator, p_size, p_bordersSize )
		, m_iSeparationOffset( 50 )
		, m_eSeparationType( eSEPARATION_NONE )
		, m_outputTexture( std::make_shared< ProceduralTextures::gl::Texture >( m_openGl ) )
		, m_inputBuffer( std::make_shared< ProceduralTextures::PixelBuffer >( p_size ) )
		, m_outputBuffer( std::make_shared< ProceduralTextures::PixelBuffer >( p_size ) )
	{
		m_itSelectedEffect = m_arrayEffects.end();
		m_outputTexture->Resize( p_size );
		std::vector< cl::Device > l_arrayDevices;
		std::vector< cl::Platform > l_arrayPlatforms;
		std::string l_prog;
		cl_int l_iErr;
		cl::Platform::get( &l_arrayPlatforms );
		bool l_bContinue = ocl::CheckErr( l_arrayPlatforms.size() != 0 ? CL_SUCCESS : -1, _( "Retrieving the OpenCL platforms" ), true );

		if ( l_bContinue )
		{
			TRACE( _T( "Platform count is: %d\n" ), uint32_t( l_arrayPlatforms.size() ) );
			std::string l_strInfo;
			m_clPlatform = l_arrayPlatforms[0];
			m_clPlatform.getInfo( ( cl_platform_info )CL_PLATFORM_NAME, &l_strInfo );
			TRACE( "  Name    : %s\n", l_strInfo.c_str() );
			m_clPlatform.getInfo( ( cl_platform_info )CL_PLATFORM_VENDOR, &l_strInfo );
			TRACE( "  Vendor  : %s\n", l_strInfo.c_str() );
			m_clPlatform.getInfo( ( cl_platform_info )CL_PLATFORM_VERSION, &l_strInfo );
			TRACE( "  Version : %s\n", l_strInfo.c_str() );
			cl_context_properties l_props[3] = { CL_CONTEXT_PLATFORM, ( cl_context_properties )( m_clPlatform )(), 0 };
			m_clContext = cl::Context( CL_DEVICE_TYPE_GPU, l_props, NULL, NULL, &l_iErr );
			l_bContinue = ocl::CheckErr( l_iErr, _( "Creating the OpenCL context" ) );
		}

		if ( l_bContinue )
		{
			l_arrayDevices = m_clContext.getInfo< CL_CONTEXT_DEVICES >();
			l_bContinue = ocl::CheckErr( l_arrayDevices.size() > 0 ? CL_SUCCESS : -1, _( "Retrieving the OpenCL devices" ), true );
		}

		if ( l_bContinue )
		{
			m_clDevice = l_arrayDevices[0];
			m_clQueue = cl::CommandQueue( m_clContext, m_clDevice, 0, &l_iErr );
			l_bContinue = ocl::CheckErr( l_iErr, _( "Creating an OpenCL command queue" ), true );
		}
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

	void GpuStep::SetSeparationType( int p_value )
	{
		m_eSeparationType = eSEPARATION( p_value );
	}

	void GpuStep::SetSeparationOffset( int p_value )
	{
		m_iSeparationOffset = p_value;
	}

	void GpuStep::SelectEffect( uint32_t p_value )
	{
		unsigned int l_uiSelection = p_value;

		if ( p_value < m_arrayEffects.size() )
		{
			m_itSelectedEffect = m_arrayEffects.begin() + p_value;
		}
	}

	void GpuStep::SelectKernel( uint32_t p_value )
	{
		unsigned int l_uiSelection = p_value;

		if ( m_itSelectedEffect != m_arrayEffects.end() && p_value < ( *m_itSelectedEffect )->GetKernelCount() )
		{
			( *m_itSelectedEffect )->LoadKernel( ( *m_itSelectedEffect )->GetKernelName( p_value ).c_str() );
		}
	}

	void GpuStep::AddEffect()
	{
		m_arrayEffects.push_back( std::make_unique< Effect >( m_clContext, m_clDevice, m_clQueue, m_outputBuffer ) );
		m_itSelectedEffect = m_arrayEffects.end();
	}

	void GpuStep::RemoveEffect( uint32_t p_value )
	{
		m_arrayEffects.erase( m_arrayEffects.begin() + p_value );
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
			ProceduralTextures::String l_strLog = ( *m_itSelectedEffect )->GetCompilerLog();

			if ( !l_strLog.empty() )
			{
				l_strText += l_strLog;
			}
			else
			{
				l_strText = _( "No error" );
			}
		}

		return l_strText;
	}

	bool GpuStep::SetFilePath( ProceduralTextures::String const & p_path )
	{
		bool l_return = false;

		if ( m_itSelectedEffect != m_arrayEffects.end() && !p_path.empty() )
		{
			l_return = ( *m_itSelectedEffect )->LoadFile( p_path );
		}

		return l_return;
	}

	void GpuStep::SetImage( ProceduralTextures::PixelBuffer const & p_image )
	{
		m_initialised = false;
		*m_inputBuffer = p_image;
		Resize( p_image.GetDimensions() );

		if ( m_arrayEffects.size() == 0 )
		{
			AddEffect();
		}
		else
		{
			for ( auto & l_effect : m_arrayEffects )
			{
				l_effect->UpdateImages();
			}
		}

		m_initialised = true;
	}

	void GpuStep::DoInitialise()
	{
		m_outputTexture->Create();
	}

	void GpuStep::DoCleanup()
	{
		for ( auto & l_effect : m_arrayEffects )
		{
			l_effect->Cleanup();
		}

		m_arrayEffects.clear();
		m_outputTexture->Cleanup();
		m_outputTexture->Destroy();
	}

	void GpuStep::DoResize()
	{
		m_outputTexture->Resize( m_sizeImage );

		for ( auto & l_effect : m_arrayEffects )
		{
			l_effect->Resize( m_sizeImage );
		}
	}

	void GpuStep::DoPreRender()
	{
		m_outputBuffer->Initialise( m_sizeImage );
		std::memcpy( m_outputBuffer->Ptr(), m_inputBuffer->ConstPtr(), m_inputBuffer->GetSize() );
		m_outputTexture->Initialise( m_outputBuffer );

		for ( auto & l_effect : m_arrayEffects )
		{
			if ( !l_effect->IsInitialised() )
			{
				l_effect->Initialise();
			}
		}
	}

	void GpuStep::DoRender( bool & p_bChanged )
	{
		for ( auto & l_effect : m_arrayEffects )
		{
			l_effect->Apply( m_eSeparationType, m_iSeparationOffset );
		}

		m_openGl->Viewport( 0, 0, m_sizeImage.x(), m_sizeImage.y() );
		m_frameBuffer->Bind();
		m_openGl->Clear( GL_COLOR_BUFFER_BIT );
		m_program->Activate();
		m_outputTexture->Activate( GL_TEXTURE0, true );
		DoSubRender( m_vertexAttribLocation, m_textureAttribLocation );
		m_outputTexture->Deactivate();
		m_program->Deactivate();
		m_frameBuffer->Unbind();
		p_bChanged = false;
	}

	void GpuStep::DoPostRender()
	{
	}
}
