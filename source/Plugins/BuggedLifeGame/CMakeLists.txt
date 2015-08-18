project( BuggedLifeGame )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	BuggedLifeGame
	PARENT_SCOPE
)

set( BLG_VERSION_MAJOR	2 )
set( BLG_VERSION_MINOR	0 )
set( BLG_VERSION_BUILD	1 )

add_target(
	BuggedLifeGame
	plugin
	"Generator"
	"Generator;${OPT_LINKS}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

copy_languages( BuggedLifeGame ../internationalisation ProceduralGenerator "fr" )
set( Build "yes (version ${BLG_VERSION_MAJOR}.${BLG_VERSION_MINOR}.${BLG_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( BuggedLifeGame ".h;.inl;.hpp;.cpp" )
set_property( TARGET BuggedLifeGame PROPERTY FOLDER "Plugins" )
