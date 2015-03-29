function( CoreInit )
endfunction( CoreInit )

function( CoreBuild )
	add_subdirectory( Core/Generator )
endfunction( CoreBuild )

function( CoreSummary msg )
endfunction( CoreSummary )

function( CoreSetup )
	cpack_add_component_group( Development
		DISPLAY_NAME "ProceduralGenerator SDK"
		DESCRIPTION "All the libraries and includes needed to develop a plug-in for ProceduralGenerator"
	)
	cpack_add_component_group( Documentation
		DISPLAY_NAME "Documentation"
		DESCRIPTION "Projects documentations"
	)

	cpack_add_component( Generator
		DISPLAY_NAME "ProceduralGenerator library"
		DESCRIPTION "Base generator library used to develop plug-ins"
		REQUIRED
		GROUP Development
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( Generator_dev
		DISPLAY_NAME "ProceduralGenerator includes"
		DESCRIPTION "Base generator includes used to develop plug-ins"
		REQUIRED
		GROUP Development
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( Generator_doc
		DISPLAY_NAME "ProceduralGenerator help"
		DESCRIPTION "ProceduralGenerator HTML documentation"
		GROUP Documentation
		INSTALL_TYPES Developer Full
	)
endfunction( CoreSetup )

