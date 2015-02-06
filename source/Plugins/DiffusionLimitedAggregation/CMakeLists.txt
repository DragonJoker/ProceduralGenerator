project( DiffusionLimitedAggregation )

option( PGEN_BUILD_DIFFUSIONLIMITEDAGGREGATION "Build Diffusion Limited Aggregation plugin" TRUE )

if( ${PGEN_BUILD_DIFFUSIONLIMITEDAGGREGATION} )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( BinsDependencies
		${BinsDependencies}
		DiffusionLimitedAggregation
		PARENT_SCOPE
	)

	set( DLA_VERSION_MAJOR	1 )
	set( DLA_VERSION_MINOR	3 )
	set( DLA_VERSION_BUILD	3 )

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
endif()
