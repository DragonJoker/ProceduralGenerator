project( PerlinNoise )

option( PGEN_BUILD_PERLINNOISE "Build Perlin Noise plugin" TRUE )

if( ${PGEN_BUILD_PERLINNOISE} )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( BinsDependencies
	    ${BinsDependencies}
	    PerlinNoise
	    PARENT_SCOPE
	)

	set( PNS_VERSION_MAJOR	1 )
	set( PNS_VERSION_MINOR	3 )
	set( PNS_VERSION_BUILD	3 )

	add_target(
		PerlinNoise
		plugin
		"Generator"
		"Generator;${OPT_LINKS}"
		"PrecompiledHeader.h"
		"PrecompiledHeader.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
 )

	copy_languages( PerlinNoise ../internationalisation ProceduralGenerator "fr" )
	set( Build "yes (version ${PNS_VERSION_MAJOR}.${PNS_VERSION_MINOR}.${PNS_VERSION_BUILD})" PARENT_SCOPE )
	add_target_astyle( PerlinNoise ".h;.inl;.hpp;.c;.cpp" )
	set_property( TARGET PerlinNoise PROPERTY FOLDER "Plugins" )
endif()
