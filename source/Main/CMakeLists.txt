function( MainInit )
endfunction( MainInit )

function( MainBuild )
	add_subdirectory( Main/ProceduralGenerator )
endfunction( MainBuild )

function( MainSummary msg )
endfunction( MainSummary )

function( MainSetup )
	cpack_add_component_group( Samples
		DISPLAY_NAME "ProceduralGenerator sample data"
		DESCRIPTION "Sample datas for ProceduralGenerator"
	)

	cpack_add_component( ProceduralGenerator
		DISPLAY_NAME "ProceduralGenerator application"
		DESCRIPTION "The main application used to visualise the plug-ins job"
		REQUIRED
		GROUP Development
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( ProceduralGenerator_Samples
		DISPLAY_NAME "ProceduralGenerator sample data"
		DESCRIPTION "Sample images and shaders to use with ProceduralGenerator plug-ins"
		GROUP Samples
		INSTALL_TYPES Developer Full
	)
endfunction( MainSetup )

