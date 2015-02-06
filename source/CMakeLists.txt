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

#--------------------------------------------------------------------------------------------------
#	Looking for needed libraries
#--------------------------------------------------------------------------------------------------
set( wxWidgets_USE_STATIC OFF )
find_package( wxWidgets REQUIRED core base gl )
if (NOT WIN32 )
	find_package( GTK2 )
endif (NOT WIN32 )

#--------------------------------------------------------------------------------------------------
#	Adding include dirs to include search path
#--------------------------------------------------------------------------------------------------
include_directories( Core/Generator/Src )

if( wxWidgets_FOUND )
	include_directories(	${wxWidgets_INCLUDE_DIRS} )
endif()
if (NOT WIN32 )
	include_directories(	${GTK2_INCLUDE_DIRS} )
endif (NOT WIN32 )

#--------------------------------------------------------------------------------------------------
#	Normalising libraries names
#--------------------------------------------------------------------------------------------------
#wxWidgets Libs
set( wxWidgetsLibraries "" )
FOREACH(Lib ${wxWidgets_LIBRARIES} )
	if (wxWidgetsLibraries )
		set(wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
	else()
		set(wxWidgetsLibraries "${Lib}" )
	endif()
ENDFOREACH()
FOREACH(Lib ${GTK2_LIBRARIES} )
	if (wxWidgetsLibraries )
		set(wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
	else()
		set(wxWidgetsLibraries "${Lib}" )
	endif()
ENDFOREACH()

#We set additional links X11 and OpenGL for unix projects
if (UNIX )
	set(OPT_LINKS "X11;GL" )
endif ()

if( wxWidgets_FOUND )
    message( STATUS "+ Found wxWidgets" )
	#Few more configuration for wxWidgets
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )
	set( wxWidgets_CXXFLAGS "${wxWidgets_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES" )
	if (NOT WIN32 )
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXGTK__" )
	endif ()
	if (IsWxDebug )
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXDEBUG__" )
	endif ()

	#In order to be sure we have the right flags, we set it manually
	if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "" )
		if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
			set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}" )
		else()
			set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}" )
		endif()
	endif()

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

	#--------------------------------------------------------------------------------------------------
	#	Creation of each project in the workspace
	#--------------------------------------------------------------------------------------------------
	set( BinsDependencies "" )

	if ( MSVC OR NOT "${CMAKE_BUILD_TYPE}" STREQUAL "" )
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
			set( CPACK_PACKAGE_NAME "ProceduralGenerator" )
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
				set( CPACK_PACKAGE_INSTALL_DIRECTORY "ProceduralGenerator" )
				set( CPACK_WIX_UPGRADE_GUID "6B17505B-A8B5-4EEC-8978-1BF24715A8A1" )
				set( CPACK_WIX_PRODUCT_GUID "02ACFE05-CFCE-4E1D-85DA-59A068DDCF70" )
				set( CPACK_WIX_PRODUCT_ICON "${PARENT_DIR}/install/castor.ico" )
				set( CPACK_WIX_UI_DIALOG "${PARENT_DIR}/install/dialog.bmp" )
				set( CPACK_WIX_UI_BANNER "${PARENT_DIR}/install/banner.bmp" )
				set( CPACK_WIX_PROGRAM_MENU_FOLDER "ProceduralGenerator" )
				set( CPACK_WIX_CULTURES "fr-FR;en-EN" )
				include(CPack)
			elseif ( "${PROJECTS_PACKAGE_DEB}" STREQUAL "ON" )
				set( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
				set( CPACK_DEB_COMPONENT_INSTALL ON )
				set( CPACK_GENERATOR "DEB" )
				set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Sylvain Doremus" )
				set( CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}wxwxidgets-dev (>= 2.8.12)" )
				set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_ARCHITECTURE}" )
				set( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
				set( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
				include(CPack)
			elseif ( "${PROJECTS_PACKAGE_RPM}" STREQUAL "ON" )
				set( ${PGen_PACKAGE_DIR} ${CMAKE_BINARY_DIR}/_CPack_Packages/Linux/RPM )
				set( CPACK_RPM_COMPONENT_INSTALL ON )
				set( CPACK_RPM_PACKAGE_SUMMARY "ProceduralGenerator" )
				set( CPACK_RPM_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_ARCHITECTURE}" )
				set( CPACK_RPM_PACKAGE_GROUP "Development/Tools/Other" )
				set( CPACK_RPM_PACKAGE_LICENSE "LGPL" )
				set( CPACK_RPM_SPEC_MORE_DEFINE "%define _rpmdir ${PGen_PACKAGE_DIR}" )
				set( CPACK_RPM_SPEC_MORE_DEFINE "%define _rpmfilename ${CPACK_PACKAGE_FILE_NAME}.rpm" )
				set( CPACK_RPM_SPEC_MORE_DEFINE "%define _unpackaged_files_terminate_build 0" )
				set( CPACK_RPM_SPEC_MORE_DEFINE "%define _topdir ${PGen_PACKAGE_DIR}" )
				include(CPack)
			endif()
		endif ()

		cpack_add_install_type( Minimal )
		cpack_add_install_type( Developer )
		cpack_add_install_type( Full )

		CoreSetup()
		PluginsSetup()
		MainSetup()
	else()
		message( SEND_ERROR "Please select a build type (Debug or Release)" )
	endif()
else()
	SET( msg "Procedural Generator can't be built because of following missing libraries :" )
	if( NOT wxWidgets_FOUND )
		SET( msg "${msg}\n\twxWidgets" )
	endif()
endif()
