project( LifeGame )

option( PGEN_BUILD_LIFEGAME "Build Life Game plugin" TRUE )

if( ${PGEN_BUILD_LIFEGAME} )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( BinsDependencies
		${BinsDependencies}
		LifeGame
		PARENT_SCOPE
	)

	set( LFG_VERSION_MAJOR	1 )
	set( LFG_VERSION_MINOR	3 )
	set( LFG_VERSION_BUILD	3 )

	add_target(
		LifeGame
		plugin
		"Generator"
		"Generator;${OPT_LINKS}"
		"PrecompiledHeader.h"
		"PrecompiledHeader.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
 )

	copy_languages( LifeGame ../internationalisation ProceduralGenerator "fr" )
	set( Build "yes (version ${LFG_VERSION_MAJOR}.${LFG_VERSION_MINOR}.${LFG_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( LifeGame ".h;.inl;.hpp;.c;.cpp" )
	set_property( TARGET LifeGame PROPERTY FOLDER "Plugins" )
endif()
