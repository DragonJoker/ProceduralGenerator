option( PGEN_BUILD_LIFE_GAME "Build LifeGame plugin" TRUE )
option( PGEN_BUILD_BUGGED_LIFE_GAME "Build BuggedLifeGame plugin" TRUE )
option( PGEN_BUILD_COLORED_LIFE_GAME "Build ColoredLifeGame plugin" TRUE )
option( PGEN_BUILD_PERLIN_NOISE "Build PerlinNoise plugin" TRUE )
option( PGEN_BUILD_DIFFUSION_LIMITED_AGGREGATION "Build DiffusionLimitedAggregation plugin" TRUE )
option( PGEN_BUILD_ITERATED_FUNCTION_SYSTEM "Build IteratedFunctionSystem plugin" TRUE )
option( PGEN_BUILD_VORONOI_2D "Build Voronoi2D plugin" TRUE )
option( PGEN_BUILD_IMAGE_EFFECTS "Build ImageEffects plugin" TRUE )
option( PGEN_BUILD_SHADER_EFFECTS "Build ShaderEffects plugin" TRUE )
option( PGEN_BUILD_GPU_LIFE_GAME "Build GPULifeGame plugin" TRUE )
option( PGEN_BUILD_GPU_PROGRAMMING "Build GPUProgramming plugin" TRUE )
option( PGEN_BUILD_TEXT_EFFECTS "Build TextEffects plugin" TRUE )
option( PGEN_BUILD_OCL_PROCESSING "Build OCLProcessing plugin" TRUE )
option( PGEN_BUILD_SHADER_WEBCAM "Build ShaderWebcam plugin" TRUE )

function( PluginsInit )
	set( LFG "no (not wanted )" PARENT_SCOPE )
	set( BLG "no (not wanted )" PARENT_SCOPE )
	set( CLG "no (not wanted )" PARENT_SCOPE )
	set( PNS "no (not wanted )" PARENT_SCOPE )
	set( DLA "no (not wanted )" PARENT_SCOPE )
	set( IFS "no (not wanted )" PARENT_SCOPE )
	set( V2D "no (not wanted )" PARENT_SCOPE )
	set( IFX "no (not wanted )" PARENT_SCOPE )
	set( SFX "no (not wanted )" PARENT_SCOPE )
	set( GPU "no (not wanted )" PARENT_SCOPE )
	set( GPL "no (not wanted )" PARENT_SCOPE )
	set( TFX "no (not wanted )" PARENT_SCOPE )
	set( OCL "no (not wanted )" PARENT_SCOPE )
	set( SWM "no (not wanted )" PARENT_SCOPE )
endfunction( PluginsInit )

function( PluginsBuild )
	if ( ${PGEN_BUILD_LIFE_GAME} )
		set( Build ${LFG} )
		add_subdirectory( Plugins/LifeGame )
		set( LFG ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_BUGGED_LIFE_GAME} )
		set( Build ${BLG} )
		add_subdirectory( Plugins/BuggedLifeGame )
		set( BLG ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_COLORED_LIFE_GAME} )
		set( Build ${CLG} )
		add_subdirectory( Plugins/ColoredLifeGame )
		set( CLG ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_PERLIN_NOISE} )
		set( Build ${PNS} )
		add_subdirectory( Plugins/PerlinNoise )
		set( PNS ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_DIFFUSION_LIMITED_AGGREGATION} )
		set( Build ${DLA} )
		add_subdirectory( Plugins/DiffusionLimitedAggregation )
		set( DLA ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_ITERATED_FUNCTION_SYSTEM} )
		set( Build ${IFS} )
		add_subdirectory( Plugins/IteratedFunctionSystem )
		set( IFS ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_VORONOI_2D} )
		set( Build ${V2D} )
		add_subdirectory( Plugins/Voronoi2D )
		set( V2D ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_IMAGE_EFFECTS} )
		set( Build ${IFX} )
		add_subdirectory( Plugins/ImageEffects )
		set( IFX ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_SHADER_EFFECTS} )
		set( Build ${SFX} )
		add_subdirectory( Plugins/ShaderEffects )
		set( SFX ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_GPU_PROGRAMMING} )
		set( Build ${GPU} )
		add_subdirectory( Plugins/GPUProgramming )
		set( GPU ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_GPU_LIFE_GAME} )
		set( Build ${GPL} )
		add_subdirectory( Plugins/GPULifeGame )
		set( GPL ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_TEXT_EFFECTS} )
		set( Build ${TFX} )
		add_subdirectory( Plugins/TextEffects )
		set( TFX ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_OCL_PROCESSING} )
		set( Build ${OCL} )
		add_subdirectory( Plugins/OCLProcessing )
		set( OCL ${Build} PARENT_SCOPE )
	endif ()
	if ( ${PGEN_BUILD_SHADER_WEBCAM} )
		set( Build ${SWM} )
		add_subdirectory( Plugins/ShaderWebcam )
		set( SWM ${Build} PARENT_SCOPE )
	endif ()
endfunction( PluginsBuild )

function( PluginsSummary msg )
	set( msg_tmp "\n\nPlugins :" )
	SET( msg_tmp "${msg_tmp}\n    Life Game                     ${LFG}" )
	SET( msg_tmp "${msg_tmp}\n    Coloured Life Game            ${CLG}" )
	SET( msg_tmp "${msg_tmp}\n    Bugged Life Game              ${BLG}" )
	SET( msg_tmp "${msg_tmp}\n    Perlin Noise                  ${PNS}" )
	SET( msg_tmp "${msg_tmp}\n    Diffusion Limited Aggregation ${DLA}" )
	SET( msg_tmp "${msg_tmp}\n    Iterated Function Systems     ${IFS}" )
	SET( msg_tmp "${msg_tmp}\n    Voronoi 2D                    ${V2D}" )
	SET( msg_tmp "${msg_tmp}\n    Image Effects                 ${IFX}" )
	SET( msg_tmp "${msg_tmp}\n    Shader Effects                ${SFX}" )
	SET( msg_tmp "${msg_tmp}\n    GPU Programming               ${GPU}" )
	SET( msg_tmp "${msg_tmp}\n    GPU Life Game                 ${GPL}" )
	SET( msg_tmp "${msg_tmp}\n    Text Effects                  ${TFX}" )
	SET( msg_tmp "${msg_tmp}\n    OCL Programming               ${OCL}" )
	SET( msg_tmp "${msg_tmp}\n    Shader Webcam                 ${SWM}" )
	SET( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( PluginsSummary )

macro( PluginsSetup )
	setup_add_component_group( Plugins
		DISPLAY_NAME "ProceduralGenerator plug-ins"
		DESCRIPTION "ProceduralGenerator plug-ins"
	)

	if( ${PGEN_BUILD_LIFE_GAME} )
		setup_add_component( LifeGame
			DISPLAY_NAME "LifeGame plug-in"
			DESCRIPTION "Simple life game implementation"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_BUGGED_LIFE_GAME} )
		setup_add_component( BuggedLifeGame
			DISPLAY_NAME "BuggerdLifeGame plug-in"
			DESCRIPTION "A bugged life game implementation that still has a nice render"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_COLORED_LIFE_GAME} )
		setup_add_component( ColoredLifeGame
			DISPLAY_NAME "ColoredLifeGame plug-in"
			DESCRIPTION "A coloured life game implementation"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_PERLIN_NOISE} )
		setup_add_component( PerlinNoise
			DISPLAY_NAME "PerlinNoise plug-in"
			DESCRIPTION "A Perlin nois implementation"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_DIFFUSION_LIMITED_AGGREGATION} )
		setup_add_component( DiffusionLimitedAggregation
			DISPLAY_NAME "DiffusionLimitedAggregation plug-in"
			DESCRIPTION "A diffusion limited aggregation implementation"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_ITERATED_FUNCTION_SYSTEM} )
		setup_add_component( IteratedFunctionSystem
			DISPLAY_NAME "IteratedFunctionSystem plug-in"
			DESCRIPTION "An iterated function systems implementation, with few IFS already implemented"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_VORONOI_2D} )
		setup_add_component( Voronoi2D
			DISPLAY_NAME "Voronoi2D plug-in"
			DESCRIPTION "A slow Voronoi diagram implementation"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_IMAGE_EFFECTS} )
		setup_add_component( ImageEffects
			DISPLAY_NAME "ImageEffects plug-in"
			DESCRIPTION "Image processing, using CPU"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_SHADER_EFFECTS} )
		setup_add_component( ShaderEffects
			DISPLAY_NAME "ShaderEffects plug-in"
			DESCRIPTION "Image processing, using GPU"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_GPU_PROGRAMMING} )
		setup_add_component( GPUProgramming
			DISPLAY_NAME "GPUProgramming plug-in"
			DESCRIPTION "Shader programming plug-in"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_GPU_LIFE_GAME} )
		setup_add_component( GPULifeGame
			DISPLAY_NAME "GPULifeGame plug-in"
			DESCRIPTION "Life game on GPU plug-in"
			GROUP Plugins
			DEPENDS Generator
			#INSTALL_TYPES Minimal Developer Full
		)
	endif()
	if( ${PGEN_BUILD_TEXT_EFFECTS} )
		STRING( FIND "${TFX}" "yes" POS )
		if( ${POS} STREQUAL 0 )
			setup_add_component( TextEffects
				DISPLAY_NAME "TextEffects plug-in"
				DESCRIPTION "Text rendering plug-in"
				GROUP Plugins
				DEPENDS Generator
				#INSTALL_TYPES Minimal Developer Full
			)
		endif()
	endif()
	if( ${PGEN_BUILD_OCL_PROCESSING} )
		STRING( FIND "${OCL}" "yes" POS )
		if( ${POS} STREQUAL 0 )
			setup_add_component( OCLProcessing
				DISPLAY_NAME "OCLProcessing plug-in"
				DESCRIPTION "OpenCL programmong plug-in"
				GROUP Plugins
				DEPENDS Generator
				#INSTALL_TYPES Minimal Developer Full
			)
		endif()
	endif()
	if( ${PGEN_BUILD_SHADER_WEBCAM} )
		STRING( FIND "${SWM}" "yes" POS )
		if( ${POS} STREQUAL 0 )
			setup_add_component( ShaderWebcam
				DISPLAY_NAME "ShaderWebcam plug-in"
				DESCRIPTION "Image processing using GPU, on webcam images"
				GROUP Plugins
				DEPENDS Generator
				#INSTALL_TYPES Minimal Developer Full
			)
		endif()
	endif()
endmacro( PluginsSetup )
