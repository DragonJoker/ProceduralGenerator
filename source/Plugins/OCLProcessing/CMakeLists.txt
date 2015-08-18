project( OCLProcessing )
find_package( OpenCL )

if( ${OPENCL_FOUND} )
	message( STATUS "+ Found OpenCL (version ${OPENCL_VERSION_STRING})" )
	include_directories( ${OPENCL_INCLUDE_DIRS} )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( BinsDependencies
		${BinsDependencies}
		OCLProcessing
		PARENT_SCOPE
	)

	set( OCL_VERSION_MAJOR	2 )
	set( OCL_VERSION_MINOR	0 )
	set( OCL_VERSION_BUILD	1 )

	set( OpenCLLibraries "")
	FOREACH(Lib ${OPENCL_LIBRARIES})
		if (OpenCLLibraries)
			set(OpenCLLibraries "${OpenCLLibraries}|${Lib}")
		else()
			set(OpenCLLibraries "${Lib}")
		endif()
	ENDFOREACH()

	# We add this project only if OpenCL was found, if not it is not an error (so we can build workspace)
	add_target(
		OCLProcessing
		plugin
		"Generator"
		"Generator;${OpenCLLibraries};${OPT_LINKS}"
		"PrecompiledHeader.h"
		"PrecompiledHeader.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
	)
 
	copy_languages( OCLProcessing ../internationalisation ProceduralGenerator "fr" )
	set( Build "yes (version ${OCL_VERSION_MAJOR}.${OCL_VERSION_MINOR}.${OCL_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( OCLProcessing ".h;.inl;.hpp;.c;.cpp" )
	set_property( TARGET OCLProcessing PROPERTY FOLDER "Plugins" )
else()
	set( Build "no (missing OpenCL library)" PARENT_SCOPE )
endif()
