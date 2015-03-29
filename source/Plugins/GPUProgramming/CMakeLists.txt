project( GPUProgramming )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	GPUProgramming
	PARENT_SCOPE
)

set( GPU_VERSION_MAJOR	2 )
set( GPU_VERSION_MINOR	0 )
set( GPU_VERSION_BUILD	0 )

add_target(
	GPUProgramming
	plugin
	"Generator"
	"Generator;${OPT_LINKS}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

copy_languages( GPUProgramming ../internationalisation ProceduralGenerator "fr" )
set( Build "yes (version ${GPU_VERSION_MAJOR}.${GPU_VERSION_MINOR}.${GPU_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( GPUProgramming ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET GPUProgramming PROPERTY FOLDER "Plugins" )
