project( ShaderEffects )

option( PGEN_BUILD_SHADEREFFECTS "Build Bugged Life Game plugin" TRUE )

if( ${PGEN_BUILD_SHADEREFFECTS} )
	include_directories( ${CMAKE_SOURCE_DIR}/Core/Generator/Src )

	set( BinsDependencies
		${BinsDependencies}
		ShaderEffects
		PARENT_SCOPE
	)

	set( SFX_VERSION_MAJOR	1 )
	set( SFX_VERSION_MINOR	3 )
	set( SFX_VERSION_BUILD	3 )

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
endif()
