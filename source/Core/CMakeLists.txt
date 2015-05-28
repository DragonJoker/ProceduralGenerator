function( CoreInit )
endfunction( CoreInit )

function( CoreBuild )
	add_subdirectory( Core/Generator )
endfunction( CoreBuild )

function( CoreSummary msg )
endfunction( CoreSummary )

macro( CoreSetup )
	setup_add_component_group( Runtime
		DISPLAY_NAME "ProceduralGenerator runtime"
		DESCRIPTION "Runtime applications for ProceduralGenerator"
	)
	setup_add_component_group( Development
		DISPLAY_NAME "ProceduralGenerator SDK"
		DESCRIPTION "All the libraries and includes needed to develop a plug-in for ProceduralGenerator"
	)
	setup_add_component_group( Documentation
		DISPLAY_NAME "Documentation"
		DESCRIPTION "Projects documentations"
	)

	setup_add_component( Generator
		DISPLAY_NAME "ProceduralGenerator library"
		DESCRIPTION "Base generator library used to develop plug-ins"
		REQUIRED
		GROUP Runtime
		#INSTALL_TYPES Minimal Developer Full
	)
	setup_add_component( Generator_dev
		DISPLAY_NAME "ProceduralGenerator includes"
		DESCRIPTION "Base generator includes used to develop plug-ins"
		REQUIRED
		GROUP Development
		#INSTALL_TYPES Developer Full
	)
	setup_add_component( Generator_doc
		DISPLAY_NAME "ProceduralGenerator help"
		DESCRIPTION "ProceduralGenerator HTML documentation"
		GROUP Documentation
		#INSTALL_TYPES Developer Full
	)
endmacro( CoreSetup )

