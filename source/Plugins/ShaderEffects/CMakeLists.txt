project( ShaderEffects )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	ShaderEffects
	PARENT_SCOPE
)

set( SFX_VERSION_MAJOR	2 )
set( SFX_VERSION_MINOR	0 )
set( SFX_VERSION_BUILD	0 )

add_target(
	ShaderEffects
	plugin
	"Generator"
	"Generator;${OPT_LINKS}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

copy_languages( ShaderEffects ../internationalisation ProceduralGenerator "fr" )
set( Build "yes (version ${SFX_VERSION_MAJOR}.${SFX_VERSION_MINOR}.${SFX_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ShaderEffects ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET ShaderEffects PROPERTY FOLDER "Plugins" )
