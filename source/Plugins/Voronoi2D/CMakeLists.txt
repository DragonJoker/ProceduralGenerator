project( Voronoi2D )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	Voronoi2D
	PARENT_SCOPE
)

set( V2D_VERSION_MAJOR	2 )
set( V2D_VERSION_MINOR	0 )
set( V2D_VERSION_BUILD	1 )

add_target(
	Voronoi2D
	plugin
	"Generator"
	"Generator;${OPT_LINKS}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

copy_languages( Voronoi2D ../internationalisation ProceduralGenerator "fr" )
set( Build "yes (version ${V2D_VERSION_MAJOR}.${V2D_VERSION_MINOR}.${V2D_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( Voronoi2D ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET Voronoi2D PROPERTY FOLDER "Plugins" )
