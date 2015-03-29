project( PerlinNoise )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	PerlinNoise
	PARENT_SCOPE
)

set( PNS_VERSION_MAJOR	2 )
set( PNS_VERSION_MINOR	0 )
set( PNS_VERSION_BUILD	0 )

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
