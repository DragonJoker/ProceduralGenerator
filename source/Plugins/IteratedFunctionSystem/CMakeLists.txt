project( IteratedFunctionSystem )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	IteratedFunctionSystem
	PARENT_SCOPE
)

set( IFS_VERSION_MAJOR	2 )
set( IFS_VERSION_MINOR	0 )
set( IFS_VERSION_BUILD	0 )

add_target(
	IteratedFunctionSystem
	plugin
	"Generator"
	"Generator;${OPT_LINKS}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

copy_languages( IteratedFunctionSystem ../internationalisation ProceduralGenerator "fr" )
set( Build "yes (version ${IFS_VERSION_MAJOR}.${IFS_VERSION_MINOR}.${IFS_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( IteratedFunctionSystem ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET IteratedFunctionSystem PROPERTY FOLDER "Plugins" )
