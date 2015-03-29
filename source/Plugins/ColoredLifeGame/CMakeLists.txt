project( ColoredLifeGame )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	ColoredLifeGame
	PARENT_SCOPE
)

set( CLG_VERSION_MAJOR	2 )
set( CLG_VERSION_MINOR	0 )
set( CLG_VERSION_BUILD	0 )

add_target(
	ColoredLifeGame
	plugin
	"Generator"
	"Generator;${OPT_LINKS}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

copy_languages( ColoredLifeGame ../internationalisation ProceduralGenerator "fr" )
set( Build "yes (version ${CLG_VERSION_MAJOR}.${CLG_VERSION_MINOR}.${CLG_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ColoredLifeGame ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET ColoredLifeGame PROPERTY FOLDER "Plugins" )
