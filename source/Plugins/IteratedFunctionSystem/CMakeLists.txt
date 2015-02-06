project( IteratedFunctionSystem )

option( PGEN_BUILD_ITERATEDFUNCTIONSYSTEM "Build Iterated Function System plugin" TRUE )

if( ${PGEN_BUILD_ITERATEDFUNCTIONSYSTEM} )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( BinsDependencies
		${BinsDependencies}
		IteratedFunctionSystem
		PARENT_SCOPE
	)

	set( IFS_VERSION_MAJOR	1 )
	set( IFS_VERSION_MINOR	3 )
	set( IFS_VERSION_BUILD	3 )

	add_target(
		IteratedFunctionSystem
		plugin
		"Generator"
		"Generator;${OPT_LINKS}"
		""
		""
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
 )

	copy_languages( IteratedFunctionSystem ../internationalisation ProceduralGenerator "fr" )
	set( Build "yes (version ${IFS_VERSION_MAJOR}.${IFS_VERSION_MINOR}.${IFS_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( IteratedFunctionSystem ".h;.inl;.hpp;.c;.cpp" )
	set_property( TARGET IteratedFunctionSystem PROPERTY FOLDER "Plugins" )
endif()
