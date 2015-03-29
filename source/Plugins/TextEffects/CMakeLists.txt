project( TextEffects )
find_package( Freetype )

if( ${FREETYPE_FOUND} )
	include_directories( ${FREETYPE_INCLUDE_DIRS} )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( BinsDependencies
		${BinsDependencies}
		TextEffects
		PARENT_SCOPE
	)

	set( TFX_VERSION_MAJOR	2 )
	set( TFX_VERSION_MINOR	0 )
	set( TFX_VERSION_BUILD	0 )

	# We add this project only if FreeType was found, if not it is not an error (so we can build workspace)
	add_target(
		TextEffects
		plugin
		"Generator"
		"Generator;${FREETYPE_LIBRARIES};${OPT_LINKS}"
		"PrecompiledHeader.h"
		"PrecompiledHeader.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
	)

	if ( WIN32 )
		# Preparing freetype dll installation
		get_filename_component( FreeTypePath ${FREETYPE_LIBRARY} PATH )
		get_filename_component( FreeTypePath ${FreeTypePath} PATH )
		if ( EXISTS ${FreeTypePath}/bin/freetype6.dll )
			add_custom_command(
				TARGET TextEffects
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
				COMPONENT TextEffects
			)
		endif ()
		if ( EXISTS ${FreeTypePath}/bin/zlib1.dll )
			add_custom_command(
				TARGET TextEffects
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
				COMPONENT TextEffects
			)
		endif ()
	endif ()

	copy_languages( TextEffects ../internationalisation ProceduralGenerator "fr" )
	set( Build "yes (version ${TFX_VERSION_MAJOR}.${TFX_VERSION_MINOR}.${TFX_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( TextEffects ".h;.inl;.hpp;.c;.cpp" )
	set_property( TARGET TextEffects PROPERTY FOLDER "Plugins" )
else()
	set( Build "no (missing FreeType library)" PARENT_SCOPE )
endif()
