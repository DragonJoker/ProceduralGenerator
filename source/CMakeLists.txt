cmake_minimum_required( VERSION 2.8 )
#--------------------------------------------------------------------------------------------------
#	Initial configurations
#--------------------------------------------------------------------------------------------------
# Set project name, used in folders and in workspace creation
set( MAIN_PROJECT_NAME "ProceduralGenerator" )

# Set project version numbers
set( VERSION_MAJOR	2 )
set( VERSION_MINOR	0 )
set( VERSION_BUILD	0 )
set( VERSION_YEAR	2015 )

macro( setup_initialise )
endmacro( setup_initialise )

macro( setup_add_install_type )
endmacro( setup_add_install_type )

macro( setup_add_component_group )
endmacro( setup_add_component_group )

macro( setup_add_component )
endmacro( setup_add_component )

macro( setup_finalise )
endmacro( setup_finalise )

# Used to look for external modules
set( CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/CMake"
    "${CMAKE_SOURCE_DIR}/CMake/Modules"
)
# Experimental Precompiled headers support for GCC
include( PCHSupport )
# Force Unicode projects (MBCS is past :P)
add_definitions( -DUNICODE -D_UNICODE )

set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Set only Debug and Release configuration types" FORCE )

# Declare the project
project( ${MAIN_PROJECT_NAME} )

include( Setup )
include( Project )
include( CompilerVersion )
include( UnitTest )
include( CompilationFlags )
include( AStyleUtils )

# Organize projects into folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

# Adding include dirs to include search path
include_directories( Core/Generator/Src )

if ( MSVC OR NOT "${CMAKE_BUILD_TYPE}" STREQUAL "" )
	find_package( OpenGL )

	if( OPENGL_FOUND )
		# Normalise libraries names
		set( OPT_LINKS "" )
		foreach( Lib ${OPENGL_gl_LIBRARY} )
			if( OPT_LINKS )
				set( OPT_LINKS "${OPT_LINKS}|${Lib}" )
			else()
				set( OPT_LINKS "${Lib}" )
			endif()
		endforeach()

		if( MSVC )
			# We remove the existing /Zm flag to replace it by /Zm300
			string( REGEX REPLACE ".*(/Zm[0-9]+).*" "\\1" ZmFlag ${CMAKE_CXX_FLAGS} )
			if( (NOT "${ZmFlag}" STREQUAL "") AND (NOT "${CMAKE_CXX_FLAGS}" STREQUAL "${ZmFlag}") )
				string( REPLACE ${ZmFlag} "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
			endif()
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm300" )
		endif()

		msg_debug( "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}" )
		msg_debug( "CMAKE_C_FLAGS    : ${CMAKE_C_FLAGS}" )
		msg_debug( "CMAKE_CXX_FLAGS  : ${CMAKE_CXX_FLAGS}" )

		#	Creation of each project in the workspace
		set( BinsDependencies "" )

		add_subdirectory( Core )
		add_subdirectory( Plugins )
		add_subdirectory( Main )

		CoreInit()
		PluginsInit()
		MainInit()

		CoreBuild()
		PluginsBuild()
		MainBuild()

		SET( msg "\nProcedural Generator following projects will be built" )
		CoreSummary( ${msg} )
		PluginsSummary( ${msg} )
		MainSummary( ${msg} )
		MESSAGE( STATUS ${msg} )

		if ( ( "${PROJECTS_PACKAGE_WIX}" STREQUAL "ON" ) OR ( "${PROJECTS_PACKAGE_DEB}" STREQUAL "ON" ) OR ( "${PROJECTS_PACKAGE_RPM}" STREQUAL "ON" ) )
			get_filename_component( PARENT_DIR ${CMAKE_SOURCE_DIR} PATH )
			file( MAKE_DIRECTORY "${PROJECTS_SETUP_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}" )
			set( CPACK_PACKAGE_NAME "${MAIN_PROJECT_NAME}" )
			set( CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD}" )
			set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Procedural Generator application" )
			set( CPACK_PACKAGE_DESCRIPTION "Cellular automats and Image processing application" )
			set( CPACK_PACKAGE_RELEASE 1 )
			set( CPACK_PACKAGE_CONTACT "dragonjoker59@hotmail.com" )
			set( CPACK_PACKAGE_VENDOR "DragonJoker" )
			set( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${PROJECTS_PLATFORM}" )
			set( CPACK_OUTPUT_FILE_PREFIX "${PROJECTS_SETUP_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}" )
			set( CPACK_RESOURCE_FILE_LICENSE "${PARENT_DIR}/LGPL.txt" )

			if ( "${PROJECTS_PACKAGE_WIX}" STREQUAL "ON" )
				set( CPACK_GENERATOR "WIX" )
				set( CPACK_PACKAGE_INSTALL_DIRECTORY "${MAIN_PROJECT_NAME}" )
				set( CPACK_WIX_UPGRADE_GUID "A11C3761-629C-4D78-83CA-AEB540FEA119" )
				set( CPACK_WIX_PRODUCT_GUID "9AFBAB99-31B8-463E-970F-7AA2A186D3AF" )
				set( CPACK_WIX_PRODUCT_ICON "${PARENT_DIR}/install/pgen.ico" )
				set( CPACK_WIX_UI_DIALOG "${PARENT_DIR}/install/dialog.bmp" )
				set( CPACK_WIX_UI_BANNER "${PARENT_DIR}/install/banner.bmp" )
				set( CPACK_WIX_PROGRAM_MENU_FOLDER "${MAIN_PROJECT_NAME}" )
				set( CPACK_WIX_CULTURES "fr-FR;en-EN" )
			elseif ( "${PROJECTS_PACKAGE_DEB}" STREQUAL "ON" )
				set( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
				set( CPACK_DEB_COMPONENT_INSTALL ON )
				set( CPACK_GENERATOR "DEB" )
				set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Sylvain Doremus" )
				set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_ARCHITECTURE}" )
				set( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
				set( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
			elseif ( "${PROJECTS_PACKAGE_RPM}" STREQUAL "ON" )
				set( ${PGen_PACKAGE_DIR} ${CMAKE_BINARY_DIR}/_CPack_Packages/Linux/RPM )
				set( CPACK_RPM_COMPONENT_INSTALL ON )
				set( CPACK_RPM_PACKAGE_SUMMARY "${MAIN_PROJECT_NAME}" )
				set( CPACK_RPM_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_ARCHITECTURE}" )
				set( CPACK_RPM_PACKAGE_GROUP "Development/Tools/Other" )
				set( CPACK_RPM_PACKAGE_LICENSE "LGPL" )
				set( CPACK_RPM_SPEC_MORE_DEFINE "%define _rpmdir ${PGen_PACKAGE_DIR}" )
				set( CPACK_RPM_SPEC_MORE_DEFINE "%define _rpmfilename ${CPACK_PACKAGE_FILE_NAME}.rpm" )
				set( CPACK_RPM_SPEC_MORE_DEFINE "%define _unpackaged_files_terminate_build 0" )
				set( CPACK_RPM_SPEC_MORE_DEFINE "%define _topdir ${PGen_PACKAGE_DIR}" )
			endif ()
		
			setup_initialise()

			setup_add_install_type( Minimal )
			setup_add_install_type( Developer )
			setup_add_install_type( Full )

			CoreSetup()
			PluginsSetup()
			MainSetup()
		
			setup_finalise()
		endif ()
	endif ()
else()
	message( SEND_ERROR "Please select a build type (Debug or Release)" )
endif()
