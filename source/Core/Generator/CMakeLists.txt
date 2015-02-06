project( Generator )

set( BinsDependencies
	${BinsDependencies}
	Generator
	PARENT_SCOPE
)

set( GEN_VERSION_MAJOR	1 )
set( GEN_VERSION_MINOR	3 )
set( GEN_VERSION_BUILD	3 )

add_target( 
	Generator
	lib
	""
	"${wxWidgetsLibraries};${OPT_LINKS}"
	"GeneratorPch.h"
	"GeneratorPch.cpp"
	"${wxWidgets_CFLAGS}"
	"${wxWidgets_CXXFLAGS}"
)

file( GLOB Generator_OpenGL_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.inl
)
file( GLOB Generator_OpenGL_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Gl*.c
)
source_group( "Header Files\\OpenGL" FILES ${Generator_OpenGL_HEADER_FILES} )
source_group( "Source Files\\OpenGL" FILES ${Generator_OpenGL_SOURCE_FILES} )

file( GLOB Generator_VertexBuffer_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Vertex*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Vertex*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Vertex*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Declaration*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Declaration*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Declaration*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Element*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Element*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Element*.inl
)
file( GLOB Generator_VertexBuffer_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Vertex*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Element*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Declaration*.cpp
)
source_group( "Header Files\\Vertex" FILES ${Generator_VertexBuffer_HEADER_FILES} )
source_group( "Source Files\\Vertex" FILES ${Generator_VertexBuffer_SOURCE_FILES} )

file( GLOB Generator_Pixel_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pixel*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pixel*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pixel*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Buffer*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Buffer*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Buffer*.inl
)
file( GLOB Generator_Pixel_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pixel*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Buffer*.cpp
)
source_group( "Header Files\\Pixel" FILES ${Generator_Pixel_HEADER_FILES} )
source_group( "Source Files\\Pixel" FILES ${Generator_Pixel_SOURCE_FILES} )

file( GLOB Generator_Gui_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Control*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Control*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Control*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Overlay*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Overlay*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Overlay*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Panel*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Panel*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Panel*.inl
)
file( GLOB Generator_Gui_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Control*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Overlay*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Panel*.cpp
)
source_group( "Header Files\\GUI" FILES ${Generator_Gui_HEADER_FILES} )
source_group( "Source Files\\GUI" FILES ${Generator_Gui_SOURCE_FILES} )

file( GLOB Generator_Misc_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Random*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Random*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Random*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Point*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Point*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Point*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Value*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Value*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Value*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Utils*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Utils*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Utils*.inl
)
file( GLOB Generator_Misc_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Random*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Point*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Value*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Utils*.cpp
)
source_group( "Header Files\\Miscellaneous" FILES ${Generator_Misc_HEADER_FILES} )
source_group( "Source Files\\Miscellaneous" FILES ${Generator_Misc_SOURCE_FILES} )

if ( WIN32 )
	include( InstallRequiredSystemLibraries )
endif()

copy_languages( Generator ../internationalisation ProceduralGenerator "fr" )
add_target_astyle( Generator ".h;.inl;.hpp;.c;.cpp" )
set_property( TARGET Generator PROPERTY FOLDER "Core" )