project( ShaderWebcam )

set( OpenCV_STATIC FALSE )
find_package( OpenCV )

if( ${OpenCV_FOUND} )
	include_directories( ${OpenCV_INCLUDE_DIR} )
	include_directories( ${OpenCV_INCLUDE_DIRS} )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( BinsDependencies
		${BinsDependencies}
		ShaderWebcam
		PARENT_SCOPE
	)

	set( SWM_VERSION_MAJOR	2 )
	set( SWM_VERSION_MINOR	0 )
	set( SWM_VERSION_BUILD	0 )

	set( OpenCVLibraries "")
	FOREACH(Lib ${OpenCV_LIBS})
		if (OpenCVLibraries)
			set(OpenCVLibraries "${OpenCVLibraries}|${Lib}")
		else()
			set(OpenCVLibraries "${Lib}")
		endif()
	ENDFOREACH()
	set( OpenCvLibraries "")
	FOREACH(Lib ${OpenCV_core_LIBRARY})
		if (OpenCvLibraries)
			set(OpenCvLibraries "${OpenCvLibraries}|${Lib}")
		else()
			set(OpenCvLibraries "${Lib}")
		endif()
	ENDFOREACH()
	FOREACH(Lib ${OpenCV_highgui_LIBRARY})
		if (OpenCvLibraries)
			set(OpenCvLibraries "${OpenCvLibraries}|${Lib}")
		else()
			set(OpenCvLibraries "${Lib}")
		endif()
	ENDFOREACH()

	# We add this project only if OpenCV was found, if not it is not an error (so we can build workspace)
	add_target(
		ShaderWebcam
		plugin
		"Generator"
		"Generator;${OpenCVLibraries};${OPT_LINKS}"
		"PrecompiledHeader.h"
		"PrecompiledHeader.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
	)

	copy_languages( ShaderWebcam ../internationalisation ProceduralGenerator "fr" )
	set( Build "yes (version ${SWM_VERSION_MAJOR}.${SWM_VERSION_MINOR}.${SWM_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( ShaderWebcam ".h;.inl;.hpp;.c;.cpp" )
	set_property( TARGET ShaderWebcam PROPERTY FOLDER "Plugins" )
else()
	set( Build "no (missing OpenCV library)" PARENT_SCOPE )
endif()
