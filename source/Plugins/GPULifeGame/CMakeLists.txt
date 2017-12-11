project( GPULifeGame )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	GPULifeGame
	PARENT_SCOPE
)

set( GPULG_VERSION_MAJOR	1 )
set( GPULG_VERSION_MINOR	0 )
set( GPULG_VERSION_BUILD	0 )

add_target(
	GPULifeGame
	plugin
	"Generator"
	"Generator;${OPT_LINKS}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

copy_languages( GPULifeGame ../internationalisation ProceduralGenerator "fr" )
set( Build "yes (version ${GPULG_VERSION_MAJOR}.${GPULG_VERSION_MINOR}.${GPULG_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( GPULifeGame ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET GPULifeGame PROPERTY FOLDER "Plugins" )
