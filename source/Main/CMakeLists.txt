option( PGEN_BUILD_PROCEDURAL_GENERATOR "Build ProceduralGenerator application" TRUE )

function( MainInit )
	set( PGN "no (not wanted )" PARENT_SCOPE )
endfunction( MainInit )

function( MainBuild )
	if ( ${PGEN_BUILD_PROCEDURAL_GENERATOR} )
		set( Build ${PGN} )
		add_subdirectory( Main/ProceduralGenerator )
		set( PGN ${Build} PARENT_SCOPE )
	endif ()
endfunction( MainBuild )

function( MainSummary msg )
	set( msg_tmp "\n\nApplications :" )
	SET( msg_tmp "${msg_tmp}\n    Procedural Generator          ${PGN}" )
	SET( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( MainSummary )

function( MainSetup )
	cpack_add_component_group( Runtime
		DISPLAY_NAME "ProceduralGenerator runtime"
		DESCRIPTION "Runtime applications for ProceduralGenerator"
	)

	cpack_add_component_group( Samples
		DISPLAY_NAME "ProceduralGenerator sample data"
		DESCRIPTION "Sample datas for ProceduralGenerator"
	)

	cpack_add_component( ProceduralGenerator
		DISPLAY_NAME "ProceduralGenerator application"
		DESCRIPTION "The main application used to visualise the plug-ins job"
		REQUIRED
		GROUP Runtime
		INSTALL_TYPES Minimal Developer Full
	)
	cpack_add_component( ProceduralGenerator_Samples
		DISPLAY_NAME "ProceduralGenerator sample data"
		DESCRIPTION "Sample images and shaders to use with ProceduralGenerator plug-ins"
		GROUP Samples
		INSTALL_TYPES Developer Full
	)
endfunction( MainSetup )

