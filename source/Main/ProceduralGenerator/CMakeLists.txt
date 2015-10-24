project( ProceduralGenerator )

set( OpenCV_STATIC FALSE )
find_package( OpenCV )
find_package( FFMPEG )
find_package( wxWidgets )

if (NOT WIN32 )
	find_package( GTK2 )
endif (NOT WIN32 )

if( wxWidgets_FOUND )
	find_package( wxWidgets REQUIRED core base gl )
    message( STATUS "+ Found wxWidgets" )
	if( OPENCV_FOUND OR OpenCV_FOUND )
		include_directories( ${OpenCV_INCLUDE_DIR} )
		include_directories( ${OpenCV_INCLUDE_DIRS} )
		link_directories( ${OpenCV_LINK_DIRECTORIES} )
	endif()

	if( FFMPEG_FOUND )
		include_directories( ${FFMPEG_INCLUDE_DIR} )
	endif ()

	include_directories( ${wxWidgets_INCLUDE_DIRS} )

	if (NOT WIN32 )
		include_directories( ${GTK2_INCLUDE_DIRS} )
	endif (NOT WIN32 )

	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( PGEN_VERSION_MAJOR	2 )
	set( PGEN_VERSION_MINOR	0 )
	set( PGEN_VERSION_BUILD	1 )

	#wxWidgets Libs
	set( wxWidgetsLibraries "" )
	FOREACH(Lib ${wxWidgets_LIBRARIES} )
		if (wxWidgetsLibraries )
			set(wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
		else()
			set(wxWidgetsLibraries "${Lib}" )
		endif()
	ENDFOREACH()
	FOREACH(Lib ${GTK2_LIBRARIES} )
		if (wxWidgetsLibraries )
			set(wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
		else()
			set(wxWidgetsLibraries "${Lib}" )
		endif()
	ENDFOREACH()

	#OpenCV libs
	set( OpenCVLibraries "")
	set( OpenCV_LIBS
		${OpenCV_LIBS}
		${OpenCV_core_LIBRARY}
		${OpenCV_highgui_LIBRARY}
	)
	FOREACH(Lib ${OpenCV_LIBS})
		if (OpenCVLibraries)
			set(OpenCVLibraries "${OpenCVLibraries}|${Lib}")
		else()
			set(OpenCVLibraries "${Lib}")
		endif()
	ENDFOREACH()

	#FFMPEG Libs
	set( FFMpegLibraries "")
	FOREACH(Lib ${FFMPEG_LIBRARIES})
		if (FFMpegLibraries)
			set(FFMpegLibraries "${FFMpegLibraries}|${Lib}")
		else()
			set(FFMpegLibraries "${Lib}")
		endif()
	ENDFOREACH()

	#Few more configuration for wxWidgets
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )
	set( wxWidgets_CXXFLAGS "${wxWidgets_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES" )
	if (NOT WIN32 )
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXGTK__" )
	endif ()
	if (IsWxDebug )
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXDEBUG__" )
	endif ()

	set( PGEN_C_FLAGS "" )
	set( PGEN_CXX_FLAGS "" )
	set( PGEN_LINKS "" )

	if( FFMPEG_FOUND )
		set( PGEN_C_FLAGS "${PGEN_C_FLAGS} -DPGEN_RECORDS -DPGEN_FFMPEG")
		set( PGEN_CXX_FLAGS "${PGEN_CXX_FLAGS} -DPGEN_RECORDS -DPGEN_FFMPEG")
		message( STATUS "+ Found FFMPEG" )
	elseif( OpenCV_FOUND )
		set( PGEN_C_FLAGS "${PGEN_C_FLAGS} -DPGEN_RECORDS -DPGEN_OCV")
		set( PGEN_CXX_FLAGS "${PGEN_CXX_FLAGS} -DPGEN_RECORDS -DPGEN_OCV")
		message( STATUS "+ Found OpenCV (version ${OpenCV_VERSION})" )
	endif()

	add_target(
		ProceduralGenerator
		bin
		"${BinsDependencies}"
		"Generator;${FFMpegLibraries};${OpenCVLibraries};${OPT_LINKS};${wxWidgetsLibraries}"
		"PrecompiledHeader.h"
		"PrecompiledHeader.cpp"
		"${wxWidgets_CFLAGS} ${PGEN_C_FLAGS}"
		"${wxWidgets_CXXFLAGS} ${PGEN_CXX_FLAGS}"	""
	)

	if ( WIN32 )
		if( FFMPEG_FOUND )
			# Preparing FFMPEG dlls installation
		elseif( OPENCV_FOUND OR OpenCV_FOUND )
			# Preparing OpenCV dlls installation
			set( OpenCV_DLLS )
			set( OpenCVd_DLLS )
			get_filename_component( OpenCvLibPath "${OpenCV_LIB_DIR_OPT}" PATH )
			string( REPLACE "." "" Version "${OpenCV_VERSION}" )
			foreach ( LIB ${OpenCV_LIBS} )
				get_filename_component( OpenCvLibName ${LIB} NAME_WE )
				set( OpenCvLibPathName ${OpenCvLibPath}/bin/${OpenCvLibName}${Version}.dll )
				if ( EXISTS ${OpenCvLibPathName} )
					add_custom_command(
						TARGET ProceduralGenerator
						POST_BUILD
						COMMAND if 1==$<CONFIG:Release>
							${CMAKE_COMMAND} -E
								copy_if_different
								${OpenCvLibPathName}
								${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
					)
					set( OpenCV_DLLS
						${OpenCV_DLLS}
						${OpenCvLibPathName}
					)
				else ()
					set( OpenCvLibPathName ${OpenCV_CONFIG_PATH}/bin${OpenCV_LIB_DIR_OPT}/${OpenCvLibName}${Version}.dll )
					if ( EXISTS ${OpenCvLibPathName} )
						add_custom_command(
							TARGET ProceduralGenerator
							POST_BUILD
							COMMAND if 1==$<CONFIG:Release>
								${CMAKE_COMMAND} -E
									copy_if_different
									${OpenCvLibPathName}
									${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
						)
						set( OpenCV_DLLS
							${OpenCV_DLLS}
							${OpenCvLibPathName}
						)
					endif ()
				endif ()
				set( OpenCvLibPathNamed ${OpenCvLibPath}/bin/${OpenCvLibName}${Version}d.dll )
				if ( EXISTS ${OpenCvLibPathNamed} )
					add_custom_command(
						TARGET ProceduralGenerator
						POST_BUILD
						COMMAND if 1==$<CONFIG:Debug>
							${CMAKE_COMMAND} -E
								copy_if_different
								${OpenCvLibPathNamed}
								${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
					)
					set( OpenCVd_DLLS
						${OpenCVd_DLLS}
						${OpenCvLibPathNamed}
					)
				else ()
					set( OpenCvLibPathNamed ${OpenCV_CONFIG_PATH}/bin${OpenCV_LIB_DIR_DBG}/${OpenCvLibName}${Version}d.dll )
					if ( EXISTS ${OpenCvLibPathNamed} )
						add_custom_command(
							TARGET ProceduralGenerator
							POST_BUILD
							COMMAND if 1==$<CONFIG:Debug>
								${CMAKE_COMMAND} -E
									copy_if_different
									${OpenCvLibPathNamed}
									${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
						)
						set( OpenCV_DLLS
							${OpenCV_DLLS}
							${OpenCvLibPathNamed}
						)
					endif ()
				endif ()
			endforeach ()
			install(
				FILES ${OpenCV_DLLS}
				DESTINATION bin
				CONFIGURATIONS Release
				COMPONENT ProceduralGenerator
			)
			install(
				FILES ${OpenCVd_DLLS}
				DESTINATION bin
				CONFIGURATIONS Debug
				COMPONENT ProceduralGenerator
			)
		endif ()
	endif ()
	
	install(
		DIRECTORY ${CMAKE_SOURCE_DIR}/../data
		DESTINATION share/ProceduralGenerator
		COMPONENT ProceduralGenerator_Samples
	)
	
	copy_languages( ProceduralGenerator ../internationalisation ProceduralGenerator "fr" )
	add_target_astyle( ProceduralGenerator ".h;.inl;.hpp;.c;.cpp" )
	set_property( TARGET ProceduralGenerator PROPERTY FOLDER "Main" )
	set( Build "yes (version ${PGEN_VERSION_MAJOR}.${PGEN_VERSION_MINOR}.${PGEN_VERSION_BUILD})" PARENT_SCOPE )
else ()
	set( Build "no (missing wxWidgets library)" PARENT_SCOPE )
endif ()
