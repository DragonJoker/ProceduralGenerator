project( DiffusionLimitedAggregation )
include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

set( BinsDependencies
	${BinsDependencies}
	DiffusionLimitedAggregation
	PARENT_SCOPE
)

set( DLA_VERSION_MAJOR	2 )
set( DLA_VERSION_MINOR	0 )
set( DLA_VERSION_BUILD	1 )

add_target(
	DiffusionLimitedAggregation
	plugin
	"Generator"
	"Generator;${OPT_LINKS}"
	"PrecompiledHeader.h"
	"PrecompiledHeader.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

copy_languages( DiffusionLimitedAggregation ../internationalisation ProceduralGenerator "fr" )
set( Build "yes (version ${DLA_VERSION_MAJOR}.${DLA_VERSION_MINOR}.${DLA_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( DiffusionLimitedAggregation ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET DiffusionLimitedAggregation PROPERTY FOLDER "Plugins" )
