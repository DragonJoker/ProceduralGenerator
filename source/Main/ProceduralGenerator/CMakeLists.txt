project( ProceduralGenerator )

find_package( OpenCV )
find_package( FFMPEG )

if( OPENCV_FOUND OR OpenCV_FOUND )
	include_directories( ${OpenCV_INCLUDE_DIR} )
	include_directories( ${OpenCV_INCLUDE_DIRS} )
	link_directories( ${OpenCV_LINK_DIRECTORIES} )
endif()

if( FFMPEG_FOUND )
	include_directories( ${FFMPEG_INCLUDE_DIR} )
endif ()

include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( PGEN_VERSION_MAJOR	1 )
set( PGEN_VERSION_MINOR	3 )
set( PGEN_VERSION_BUILD	3 )
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
	message( STATUS "+ Found OpenCV" )
endif()

add_target(
	ProceduralGenerator
	bin
	"${BinsDependencies}"
	"Generator;${FFMpegLibraries};${OpenCVLibraries};${OPT_LINKS}"
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
		get_filename_component( OpenCvLibPath ${OpenCV_LIB_DIR_OPT} PATH )
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

copy_languages( ProceduralGenerator ../internationalisation ProceduralGenerator "fr" )
add_target_astyle( ProceduralGenerator ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET ProceduralGenerator PROPERTY FOLDER "Main" )
