project( LifeGame )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	LifeGame
	PARENT_SCOPE
)

set( LFG_VERSION_MAJOR	2 )
set( LFG_VERSION_MINOR	0 )
set( LFG_VERSION_BUILD	1 )

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
