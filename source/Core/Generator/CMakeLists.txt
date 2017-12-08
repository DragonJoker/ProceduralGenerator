project( Generator )
include( Doxygen )

find_package( Freetype )

if( FREETYPE_FOUND )
	message( STATUS "+ Found FreeType (version ${FREETYPE_VERSION_STRING})" )
	include_directories( ${FREETYPE_INCLUDE_DIRS} )

	#FreeType Libs
	set( FreeTypeLibraries "")
	foreach(Lib ${FREETYPE_LIBRARIES})
		if (FreeTypeLibraries)
			set(FreeTypeLibraries "${FreeTypeLibraries}|${Lib}")
		else()
			set(FreeTypeLibraries "${Lib}")
		endif()
	endforeach()

	set( BinsDependencies
		${BinsDependencies}
		Generator
		PARENT_SCOPE
	)

	set( GEN_VERSION_MAJOR	2 )
	set( GEN_VERSION_MINOR	0 )
	set( GEN_VERSION_BUILD	1 )

	add_target( 
		Generator
		api_dll
		""
		"${OPT_LINKS};${FreeTypeLibraries}"
		"GeneratorPch.h"
		"GeneratorPch.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
	)

	file( GLOB Generator_Overlay_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Material*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Material*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Material*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Overlay*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Overlay*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Overlay*.inl
	)
	file( GLOB Generator_Overlay_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Overlay*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Material*.cpp
	)
	source_group( "Header Files\\Overlay" FILES ${Generator_Overlay_HEADER_FILES} )
	source_group( "Source Files\\Overlay" FILES ${Generator_Overlay_SOURCE_FILES} )

	file( GLOB Generator_Event_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Event*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Event*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Event*.inl
	)
	file( GLOB Generator_Event_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Event*.cpp
	)
	source_group( "Header Files\\GUI\\Event" FILES ${Generator_Event_HEADER_FILES} )
	source_group( "Source Files\\GUI\\Event" FILES ${Generator_Event_SOURCE_FILES} )

	file( GLOB Generator_Gui_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Ctrl*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Ctrl*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Ctrl*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Controls*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Controls*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Controls*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Font.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Font.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Font.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Glyph.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Glyph.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Glyph.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Panel*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Panel*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Panel*.inl
	)
	file( GLOB Generator_Gui_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Ctrl*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Controls*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Font*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Glyph*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Panel*.cpp
	)
	source_group( "Header Files\\GUI" FILES ${Generator_Gui_HEADER_FILES} )
	source_group( "Source Files\\GUI" FILES ${Generator_Gui_SOURCE_FILES} )

	file( GLOB Generator_OpenGL_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Vertex*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Vertex*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Vertex*.inl
	)
	file( GLOB Generator_OpenGL_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.c
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Vertex*.cpp
	)
	source_group( "Header Files\\OpenGL" FILES ${Generator_OpenGL_HEADER_FILES} )
	source_group( "Source Files\\OpenGL" FILES ${Generator_OpenGL_SOURCE_FILES} )

	file( GLOB Generator_Pixel_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pixel*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pixel*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pixel*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Buffer.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Buffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Buffer.inl
	)
	file( GLOB Generator_Pixel_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pixel*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Buffer.cpp
	)
	source_group( "Header Files\\Pixel" FILES ${Generator_Pixel_HEADER_FILES} )
	source_group( "Source Files\\Pixel" FILES ${Generator_Pixel_SOURCE_FILES} )

	file( GLOB Generator_Math_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Random*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Random*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Random*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Point*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Point*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Point*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Position*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Size*.h
	)
	file( GLOB Generator_Math_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Random*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Point*.cpp
	)
	source_group( "Header Files\\Math" FILES ${Generator_Math_HEADER_FILES} )
	source_group( "Source Files\\Math" FILES ${Generator_Math_SOURCE_FILES} )

	file( GLOB Generator_Misc_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Signal*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Value*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Value*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Value*.inl
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Utils*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Utils*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Utils*.inl
	)
	file( GLOB Generator_Misc_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Value*.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Utils*.cpp
	)
	source_group( "Header Files\\Miscellaneous" FILES ${Generator_Misc_HEADER_FILES} )
	source_group( "Source Files\\Miscellaneous" FILES ${Generator_Misc_SOURCE_FILES} )

	file( GLOB Generator_Config_HEADER_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Config*.h
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Config*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Config*.inl
	)
	file( GLOB Generator_Config_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Src/*Config*.cpp
	)
	source_group( "Header Files\\Config" FILES ${Generator_Config_HEADER_FILES} )
	source_group( "Source Files\\Config" FILES ${Generator_Config_SOURCE_FILES} )
	
	# Preparing data files installation
	if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Data/arial.ttf )
		add_custom_command(
			TARGET Generator
			POST_BUILD
			COMMAND
				${CMAKE_COMMAND} -E
					copy_if_different
					${CMAKE_CURRENT_SOURCE_DIR}/Data/arial.ttf
					${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/share/ProceduralGenerator
		)
		install(
			FILES ${CMAKE_CURRENT_SOURCE_DIR}/Data/arial.ttf
			DESTINATION share/ProceduralGenerator
			COMPONENT Generator
		)
	endif ()

	if ( WIN32 )
		include( InstallRequiredSystemLibraries )
		# Preparing freetype dll installation
		get_filename_component( FreeTypePath ${FREETYPE_LIBRARY} PATH )
		get_filename_component( FreeTypePath ${FreeTypePath} PATH )
		if ( EXISTS ${FreeTypePath}/bin/freetype6.dll )
			add_custom_command(
				TARGET Generator
				POST_BUILD
				COMMAND
					${CMAKE_COMMAND} -E
						copy_if_different
						${FreeTypePath}/bin/freetype6.dll
						${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
			)
			install(
				FILES ${FreeTypePath}/bin/freetype6.dll
				DESTINATION bin
				COMPONENT Generator
			)
		endif ()
		if ( EXISTS ${FreeTypePath}/bin/zlib1.dll )
			add_custom_command(
				TARGET Generator
				POST_BUILD
				COMMAND
					${CMAKE_COMMAND} -E
						copy_if_different
						${FreeTypePath}/bin/zlib1.dll
						${PROJECTS_BINARIES_OUTPUT_DIR}/$<CONFIGURATION>/bin
			)
			install(
				FILES ${FreeTypePath}/bin/zlib1.dll
				DESTINATION bin
				COMPONENT Generator
			)
		endif ()
	endif ()

	copy_languages( Generator ../internationalisation ProceduralGenerator "fr" )
	add_target_astyle( Generator ".h;.inl;.hpp;.c;.cpp" )
	set_property( TARGET Generator PROPERTY FOLDER "Core" )
	target_add_doc( Generator "" "*.h *.hpp" "\"${CMAKE_CURRENT_SOURCE_DIR}/Src/glew.h\" \"${CMAKE_CURRENT_SOURCE_DIR}/Src/glxew.h\" \"${CMAKE_CURRENT_SOURCE_DIR}/Src/wglew.h\"" )
else ()
	message( SEND_ERROR "Missing FreeType library" )
endif ()
