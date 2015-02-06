#include "OclEffect.h"

#include <GeneratorUtils.h>
#include <Buffer.h>

#include <fstream>

namespace ProceduralTextures
{
	namespace ocl
	{
		bool CheckErr( cl_int p_iErr, const char * p_szName )
		{
			bool l_bReturn = true;
			static std::map< cl_int, std::string > MapErrors;

			if ( MapErrors.size() == 0 )
			{
				MapErrors[CL_SUCCESS                                  	] = "CL_SUCCESS";									;
				MapErrors[CL_DEVICE_NOT_FOUND                         	] = "CL_DEVICE_NOT_FOUND"							;
				MapErrors[CL_DEVICE_NOT_AVAILABLE                     	] = "CL_DEVICE_NOT_AVAILABLE"						;
				MapErrors[CL_COMPILER_NOT_AVAILABLE                   	] = "CL_COMPILER_NOT_AVAILABLE"						;
				MapErrors[CL_MEM_OBJECT_ALLOCATION_FAILURE            	] = "CL_MEM_OBJECT_ALLOCATION_FAILURE"				;
				MapErrors[CL_OUT_OF_RESOURCES                         	] = "CL_OUT_OF_RESOURCES"							;
				MapErrors[CL_OUT_OF_HOST_MEMORY                       	] = "CL_OUT_OF_HOST_MEMORY"							;
				MapErrors[CL_PROFILING_INFO_NOT_AVAILABLE             	] = "CL_PROFILING_INFO_NOT_AVAILABLE"				;
				MapErrors[CL_MEM_COPY_OVERLAP                         	] = "CL_MEM_COPY_OVERLAP"							;
				MapErrors[CL_IMAGE_FORMAT_MISMATCH                    	] = "CL_IMAGE_FORMAT_MISMATCH"						;
				MapErrors[CL_IMAGE_FORMAT_NOT_SUPPORTED               	] = "CL_IMAGE_FORMAT_NOT_SUPPORTED"					;
				MapErrors[CL_BUILD_PROGRAM_FAILURE                    	] = "CL_BUILD_PROGRAM_FAILURE"						;
				MapErrors[CL_MAP_FAILURE                              	] = "CL_MAP_FAILURE"								;
				MapErrors[CL_MISALIGNED_SUB_BUFFER_OFFSET             	] = "CL_MISALIGNED_SUB_BUFFER_OFFSET"				;
				MapErrors[CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST	] = "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"	;
#if CL_VERSION_12
				MapErrors[CL_COMPILE_PROGRAM_FAILURE                  	] = "CL_COMPILE_PROGRAM_FAILURE"					;
				MapErrors[CL_LINKER_NOT_AVAILABLE                     	] = "CL_LINKER_NOT_AVAILABLE"						;
				MapErrors[CL_LINK_PROGRAM_FAILURE                     	] = "CL_LINK_PROGRAM_FAILURE"						;
				MapErrors[CL_DEVICE_PARTITION_FAILED                  	] = "CL_DEVICE_PARTITION_FAILED"					;
				MapErrors[CL_KERNEL_ARG_INFO_NOT_AVAILABLE            	] = "CL_KERNEL_ARG_INFO_NOT_AVAILABLE"				;
#endif
				MapErrors[CL_INVALID_VALUE                            	] = "CL_INVALID_VALUE"								;
				MapErrors[CL_INVALID_DEVICE_TYPE                      	] = "CL_INVALID_DEVICE_TYPE"						;
				MapErrors[CL_INVALID_PLATFORM                         	] = "CL_INVALID_PLATFORM"							;
				MapErrors[CL_INVALID_DEVICE                           	] = "CL_INVALID_DEVICE"								;
				MapErrors[CL_INVALID_CONTEXT                          	] = "CL_INVALID_CONTEXT"							;
				MapErrors[CL_INVALID_QUEUE_PROPERTIES                 	] = "CL_INVALID_QUEUE_PROPERTIES"					;
				MapErrors[CL_INVALID_COMMAND_QUEUE                    	] = "CL_INVALID_COMMAND_QUEUE"						;
				MapErrors[CL_INVALID_HOST_PTR                         	] = "CL_INVALID_HOST_PTR"							;
				MapErrors[CL_INVALID_MEM_OBJECT                       	] = "CL_INVALID_MEM_OBJECT"							;
				MapErrors[CL_INVALID_IMAGE_FORMAT_DESCRIPTOR          	] = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"			;
				MapErrors[CL_INVALID_IMAGE_SIZE                       	] = "CL_INVALID_IMAGE_SIZE"							;
				MapErrors[CL_INVALID_SAMPLER                          	] = "CL_INVALID_SAMPLER"							;
				MapErrors[CL_INVALID_BINARY                           	] = "CL_INVALID_BINARY"								;
				MapErrors[CL_INVALID_BUILD_OPTIONS                    	] = "CL_INVALID_BUILD_OPTIONS"						;
				MapErrors[CL_INVALID_PROGRAM                          	] = "CL_INVALID_PROGRAM"							;
				MapErrors[CL_INVALID_PROGRAM_EXECUTABLE               	] = "CL_INVALID_PROGRAM_EXECUTABLE"					;
				MapErrors[CL_INVALID_KERNEL_NAME                      	] = "CL_INVALID_KERNEL_NAME"						;
				MapErrors[CL_INVALID_KERNEL_DEFINITION                	] = "CL_INVALID_KERNEL_DEFINITION"					;
				MapErrors[CL_INVALID_KERNEL                           	] = "CL_INVALID_KERNEL"								;
				MapErrors[CL_INVALID_ARG_INDEX                        	] = "CL_INVALID_ARG_INDEX"							;
				MapErrors[CL_INVALID_ARG_VALUE                        	] = "CL_INVALID_ARG_VALUE"							;
				MapErrors[CL_INVALID_ARG_SIZE                         	] = "CL_INVALID_ARG_SIZE"							;
				MapErrors[CL_INVALID_KERNEL_ARGS                      	] = "CL_INVALID_KERNEL_ARGS"						;
				MapErrors[CL_INVALID_WORK_DIMENSION                   	] = "CL_INVALID_WORK_DIMENSION"						;
				MapErrors[CL_INVALID_WORK_GROUP_SIZE                  	] = "CL_INVALID_WORK_GROUP_SIZE"					;
				MapErrors[CL_INVALID_WORK_ITEM_SIZE                   	] = "CL_INVALID_WORK_ITEM_SIZE"						;
				MapErrors[CL_INVALID_GLOBAL_OFFSET                    	] = "CL_INVALID_GLOBAL_OFFSET"						;
				MapErrors[CL_INVALID_EVENT_WAIT_LIST                  	] = "CL_INVALID_EVENT_WAIT_LIST"					;
				MapErrors[CL_INVALID_EVENT                            	] = "CL_INVALID_EVENT"								;
				MapErrors[CL_INVALID_OPERATION                        	] = "CL_INVALID_OPERATION"							;
				MapErrors[CL_INVALID_GL_OBJECT                        	] = "CL_INVALID_GL_OBJECT"							;
				MapErrors[CL_INVALID_BUFFER_SIZE                      	] = "CL_INVALID_BUFFER_SIZE"						;
				MapErrors[CL_INVALID_MIP_LEVEL                        	] = "CL_INVALID_MIP_LEVEL"							;
				MapErrors[CL_INVALID_GLOBAL_WORK_SIZE                 	] = "CL_INVALID_GLOBAL_WORK_SIZE"					;
#if CL_VERSION_12
				MapErrors[CL_INVALID_PROPERTY                         	] = "CL_INVALID_PROPERTY"							;
				MapErrors[CL_INVALID_IMAGE_DESCRIPTOR                 	] = "CL_INVALID_IMAGE_DESCRIPTOR"					;
				MapErrors[CL_INVALID_COMPILER_OPTIONS                 	] = "CL_INVALID_COMPILER_OPTIONS"					;
				MapErrors[CL_INVALID_LINKER_OPTIONS                   	] = "CL_INVALID_LINKER_OPTIONS"						;
				MapErrors[CL_INVALID_DEVICE_PARTITION_COUNT				] = "CL_INVALID_DEVICE_PARTITION_COUNT"				;
#endif
			};

			if ( p_iErr != CL_SUCCESS )
			{
				std::map< cl_int, std::string >::const_iterator l_it = MapErrors.find( p_iErr );

				if ( l_it != MapErrors.end() )
				{
					TRACE( "ERROR: %s - %d (%s)\n", p_szName, p_iErr, l_it->second.c_str() );
				}
				else
				{
					TRACE( "ERROR: %s - %d\n", p_szName, p_iErr );
				}

				l_bReturn = false;
			}

			return l_bReturn;
		}

		std::string & Replace( std::string & p_str, std::string const & p_find, std::string const & p_replaced )
		{
			std::string l_temp;
			std::string l_return;
			std::size_t	l_currentPos	= 0;
			std::size_t	l_pos			= 0;

			while ( ( l_pos = p_str.find( p_find, l_currentPos ) ) != std::string::npos )
			{
				l_return.append( p_str.substr( l_currentPos, l_pos - l_currentPos ) );
				l_return.append( p_replaced );
				l_currentPos = l_pos + p_find.size();
			}

			if ( l_currentPos != p_str.size() )
			{
				l_return.append( p_str.substr( l_currentPos, l_pos - l_currentPos ) );
			}

			p_str = l_return;
			return p_str;
		}

		std::string RetrieveName( std::string const & p_strText )
		{
			std::string l_strReturn = p_strText.substr( 0, p_strText.find( ')' ) );
			Replace( l_strReturn, "(", "" );
			Replace( l_strReturn, ")", "" );
			Replace( l_strReturn, " ", "" );
			return l_strReturn;
		}
	}

	//*************************************************************************************************

	Effect::Effect( cl::Context const & p_clContext, cl::Device const & p_clDevice, cl::CommandQueue const & p_clQueue, wxImage * p_pImage, PixelBuffer * p_pBuffer )
		:	m_pImage( p_pImage )
		,	m_clProgram()
		,	m_clKernel()
		,	m_strFile()
		,	m_strKernel( wxT( "Kernel" ) )
		,	m_bNewProgram( false )
		,	m_llStartTime( 0 )
		,	m_iWidth( 0 )
		,	m_iHeight( 0 )
		,	m_clContext( p_clContext )
		,	m_clDevice( p_clDevice )
		,	m_clQueue( p_clQueue )
		,	m_bLoaded( false )
		,	m_pInputBuffer( p_pBuffer )
		,	m_outputImage( NULL )
		,	m_bProgramLoaded( false )
		,	m_bKernelLoaded( false )
	{
		if ( m_pImage && m_pImage->IsOk() )
		{
			m_iWidth = m_pImage->GetWidth();
			m_iHeight = m_pImage->GetHeight();
		}
	}

	Effect::~Effect()
	{
		for ( size_t i = 0 ; i < m_arrayImages.size() ; i++ )
		{
			delete m_arrayImages[i];
		}

		m_arrayImages.clear();
		m_arrayImagesFiles.clear();
	}

	bool Effect::LoadFile( const wxString & p_strPath )
	{
		m_iArgsCount = 0;
		m_bProgramLoaded = false;
		m_bKernelLoaded = false;
		static const std::string KernelName = "KERNEL_NAME";
		static const std::string KernelParam = "KERNEL_PARAM";
		m_strFile = p_strPath;
		m_arrayKernels.clear();
		std::ifstream l_file( ( char const * )m_strFile.char_str() );

		if ( l_file.is_open() )
		{
			std::string l_strProg( std::istreambuf_iterator< char >( l_file ), ( std::istreambuf_iterator< char >() ) );
			size_t l_uiOffset = 0;
			size_t l_uiKIndex = 0;
			std::string l_strProgSearch = l_strProg;

			if ( ( l_uiOffset = l_strProgSearch.find( "#define " + KernelName ) ) != std::string::npos )
			{
				l_strProgSearch = l_strProgSearch.substr( l_uiOffset + ( "#define " + KernelName ).size() );
			}

			l_uiOffset = 0;

			while ( ( l_uiKIndex = l_strProgSearch.find( KernelName, l_uiOffset ) ) != std::string::npos )
			{
				size_t l_uiPIndex = l_strProgSearch.find( KernelName, l_uiKIndex + 1 );
				std::string l_strKernelDef = l_strProgSearch.substr( l_uiKIndex, l_uiPIndex - l_uiKIndex );
				l_uiOffset = 0;
				int l_iArgIndex = 0;
				stKERNEL l_kernel;
				l_kernel.strName = ocl::RetrieveName( l_strKernelDef.substr( KernelName.size() ) );

				while ( ( l_uiPIndex = l_strKernelDef.find( KernelParam, l_uiOffset ) ) != std::string::npos )
				{
					l_kernel.mapParams.insert( std::make_pair( ocl::RetrieveName( l_strKernelDef.substr( l_uiPIndex + KernelParam.size() ) ), l_iArgIndex++ ) );
					l_uiOffset = l_uiPIndex + 1;
				}

				m_arrayKernels.push_back( l_kernel );
				l_uiOffset = l_uiKIndex + 1;
			}

			cl::Program::Sources l_clSource( 1, std::make_pair( l_strProg.c_str(), uint32_t( l_strProg.length() ) ) );
			int l_iErr = 0;
			m_clProgram = cl::Program( m_clContext, l_clSource, &l_iErr );

			if ( ocl::CheckErr( l_iErr, "Program::Program()" ) )
			{
				m_bProgramLoaded = ocl::CheckErr( m_clProgram.build(), "Program::build()" );
			}
		}

		m_itKernel = m_arrayKernels.end();
		return m_bProgramLoaded;
	}

	bool Effect::LoadKernel( const wxString & p_strName )
	{
		m_iArgsCount = 0;
		m_itKernel = std::find_if( m_arrayKernels.begin(), m_arrayKernels.end(), [&]( stKERNEL p_kernel )
		{
			return p_kernel.strName == ( char const * )p_strName.char_str();
		} );

		if ( m_itKernel != m_arrayKernels.end() )
		{
			m_strKernel = p_strName;
			int l_iErr = 0;
			m_clKernel = cl::Kernel( m_clProgram, ( char const * )m_strKernel.char_str(), &l_iErr );
			m_bKernelLoaded = ocl::CheckErr( l_iErr, "Kernel::Kernel()" );

			if ( m_bKernelLoaded )
			{
				m_iArgsCount = m_clKernel.getInfo< CL_KERNEL_NUM_ARGS >( &l_iErr );
				m_bKernelLoaded = ocl::CheckErr( l_iErr, "Kernel::getInfo()" );
			}
		}

		m_llStartTime = wxGetLocalTimeMillis();
		m_bNewProgram = false;
		m_bLoaded = m_bProgramLoaded && m_bKernelLoaded;
		return m_bKernelLoaded;
	}

	void Effect::Compile()
	{
		m_bNewProgram = true;
		m_bLoaded = false;
	}

	void Effect::SetImagePath( size_t p_iImage, const wxString & p_strImagePath )
	{
		wxImage * l_pImage = NULL;

		if ( p_iImage >= m_arrayImages.size() )
		{
			l_pImage = new wxImage();
			m_arrayImages.push_back( l_pImage );
			m_arrayImagesFiles.push_back( p_strImagePath );
		}
		else
		{
			l_pImage = m_arrayImages[p_iImage];
			m_arrayImagesFiles[p_iImage] = p_strImagePath;
		}

		if ( l_pImage->LoadFile( p_strImagePath ) )
		{
			int l_iWidth = l_pImage->GetWidth();
			int l_iHeight = l_pImage->GetHeight();
			TRACE( "Original : %dx%d\n", l_iWidth, l_iHeight );

			if ( m_iWidth != l_iWidth || m_iHeight != l_iHeight )
			{
				TRACE( "Computed : %dx%d\n", m_iWidth, m_iHeight );
				l_pImage->Rescale( m_iWidth, m_iHeight, wxIMAGE_QUALITY_HIGH );
				TRACE( "Scaled : %dx%d\n", l_pImage->GetWidth(), l_pImage->GetHeight() );
			}
		}
		else
		{
			//		ShowMessageBox( wxT( "Image loading error" ), wxT( "No handler found for this image format" ), wxOK | wxCENTRE | wxICON_ERROR );
		}
	}

	void Effect::Initialise()
	{
		Cleanup();
		DoInitialiseImages();
		m_llStartTime = wxGetLocalTimeMillis();
		m_bNewProgram = false;
		m_bLoaded = m_bProgramLoaded && m_bKernelLoaded;
	}

	void Effect::Cleanup()
	{
		DoCleanupImages();
	}

	bool Effect::Apply( eSEPARATION p_eSeparationType, int p_iSeparationOffset )
	{
		bool l_bReturn = true;

		if ( m_bKernelLoaded )
		{
			if ( m_iWidth != m_pImage->GetWidth() || m_iHeight != m_pImage->GetHeight() || !m_outputImage )
			{
				DoInitialiseImages();
			}

			cl::size_t< 3 > origin;
			origin[0] = 0;	//x
			origin[1] = 0;	//y
			origin[2] = 0;	//z
			cl::size_t< 3 > region;
			region[0] = m_iWidth;	//x
			region[1] = m_iHeight;	//y
			region[2] = 1;			//z
			int l_iArgsCount = 0;
			int l_iErr = m_clQueue.enqueueWriteImage( m_inputImage2DBuffer, true, origin, region, 0, 0, m_pInputBuffer->ptr(), NULL, NULL );
			bool l_bContinue = ocl::CheckErr( l_iErr, "Queue::enqueueWriteImage()" );

			if ( m_iArgsCount > 0 )
			{
				std::map< std::string, int >::iterator l_it = m_itKernel->mapParams.begin();

				while ( l_it != m_itKernel->mapParams.end() && l_bContinue )
				{
					if ( l_it->second < m_iArgsCount )
					{
						if ( l_it->first == "pg_sep_type" )
						{
							l_bContinue = ocl::CheckErr( m_clKernel.setArg( l_it->second, sizeof( int ), &p_eSeparationType ), "setArg pg_sep_type" );
						}
						else if ( l_it->first == "pg_sep_offset" )
						{
							l_bContinue = ocl::CheckErr( m_clKernel.setArg( l_it->second, sizeof( int ), &p_iSeparationOffset ), "setArg pg_sep_offset" );
						}
						else if ( l_it->first == "pg_time" )
						{
							int l_iValue = ( wxGetLocalTimeMillis() - m_llStartTime ).ToLong();
							l_bContinue = ocl::CheckErr( m_clKernel.setArg( l_it->second, sizeof( int ), &l_iValue ), "setArg pg_time" );
						}
						else if ( l_it->first == "pg_srcImg" )
						{
							l_bContinue = ocl::CheckErr( m_clKernel.setArg( l_it->second, m_inputImage2DBuffer ), "setArg 0" );
						}
						else if ( l_it->first == "pg_dstImg" )
						{
							l_bContinue = ocl::CheckErr( m_clKernel.setArg( l_it->second, m_outputImage2DBuffer ), "setArg 1" );
						}
					}

					++l_it;
				}
			}

			if ( l_bContinue )
			{
				l_bContinue = ocl::CheckErr( m_clQueue.enqueueNDRangeKernel( m_clKernel, cl::NullRange, cl::NDRange( m_iWidth, m_iHeight ), cl::NDRange( 4, 4 ), NULL, NULL ), "Queue::enqueueNDRangeKernel()" );
			}

			if ( l_bContinue )
			{
				l_bContinue = ocl::CheckErr( m_clQueue.finish(), "Queue::finish()" );
			}

			if ( l_bContinue )
			{
				l_bContinue = ocl::CheckErr( m_clQueue.enqueueReadImage( m_outputImage2DBuffer, true, origin, region, 0, 0, m_pInputBuffer->ptr(), NULL, NULL ), "Queue::enqueueReadImage()" );
			}
		}

		return l_bReturn;
	}

	wxString Effect::GetCompilerLog()
	{
		wxString l_strReturn;
		int l_iErr = 0;
		int l_iStatus = m_clProgram.getBuildInfo< CL_PROGRAM_BUILD_STATUS >( m_clDevice, &l_iErr );

		if ( l_iStatus != CL_BUILD_SUCCESS )
		{
			std::string l_strInfo = m_clProgram.getBuildInfo< CL_PROGRAM_BUILD_LOG >( m_clDevice, &l_iErr );

			if ( ocl::CheckErr( l_iErr, "Program::getBuildInfo()" ) )
			{
				l_strReturn = wxString( l_strInfo.c_str(), wxMBConvLibc() );
			}
		}

		return l_strReturn;
	}

	void Effect::UpdateImages()
	{
		m_iWidth = m_pImage->GetWidth();
		m_iHeight = m_pImage->GetHeight();

		for ( size_t i = 0 ; i < m_arrayImages.size() ; i++ )
		{
			m_arrayImages[i]->LoadFile( m_arrayImagesFiles[i] );
			m_arrayImages[i]->ResampleBicubic( m_iWidth, m_iHeight );
		}
	}

	void Effect::DoCleanupImages()
	{
		delete m_outputImage;
		m_outputImage = NULL;
	}

	void Effect::DoInitialiseImages()
	{
		DoCleanupImages();
		int l_iErr = 0;
		m_outputImage = new wxImage( *m_pImage );
		bool l_bContinue = true;
		m_iWidth = m_pImage->GetWidth();
		m_iHeight = m_pImage->GetHeight();
		cl::ImageFormat l_clImageFormat( CL_RGBA, CL_UNSIGNED_INT8 );

		if ( l_bContinue )
		{
			m_inputImage2DBuffer = cl::Image2D( m_clContext, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, l_clImageFormat, m_iWidth, m_iHeight, m_iWidth * 4, m_pInputBuffer->ptr(), &l_iErr );
			l_bContinue = ocl::CheckErr( l_iErr, "m_inputImage2DBuffer loading" );
		}

		if ( l_bContinue )
		{
			m_outputImage2DBuffer = cl::Image2D( m_clContext, CL_MEM_COPY_HOST_PTR | CL_MEM_WRITE_ONLY, l_clImageFormat, m_iWidth, m_iHeight, 0, m_pInputBuffer->ptr(), &l_iErr );
			l_bContinue = ocl::CheckErr( l_iErr, "m_outputImage2DBuffer loading" );
		}
	}
}
