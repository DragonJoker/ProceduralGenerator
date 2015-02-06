FIND_PATH(LibMatroska_ROOT_DIR matroska/KaxVersion.h
	HINTS
	PATH_SUFFIXES Matroska libmatroska
	PATHS
	/usr/local
	/usr
)

FIND_PATH(LibMatroska_INCLUDE_DIR matroska/KaxVersion.h 
	HINTS
	PATH_SUFFIXES
	PATHS
	${LibMatroska_ROOT_DIR}
	/usr/local/include
	/usr/include
)

if( MSVC )
	if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
		FIND_PATH(LibMatroska_LIBRARY_RELEASE_DIR libmatroska.so libmatroska_md.lib
			HINTS
			PATH_SUFFIXES lib64 Win64 x64
			PATHS
			${LibMatroska_ROOT_DIR}
			/usr/local
			/usr
		)

		FIND_PATH(LibMatroska_LIBRARY_DEBUG_DIR libmatroska.so libmatroskad_md.lib
			HINTS
			PATH_SUFFIXES lib64 Win64 x64
			PATHS
			${LibMatroska_ROOT_DIR}
			/usr/local
			/usr
		)
	else()
		FIND_PATH(LibMatroska_LIBRARY_RELEASE_DIR libmatroska.so libmatroska_md.lib
			HINTS
			PATH_SUFFIXES lib Win32 x86
			PATHS
			${LibMatroska_ROOT_DIR}
			${LibMatroska_ROOT_DIR}/lib
			/usr/local
			/usr
		)

		FIND_PATH(LibMatroska_LIBRARY_DEBUG_DIR libmatroskad.so libmatroskad_md.lib
			HINTS
			PATH_SUFFIXES lib Win32 x86
			PATHS
			${LibMatroska_ROOT_DIR}
			${LibMatroska_ROOT_DIR}/lib
			/usr/local
			/usr
		)
	endif()

	FIND_LIBRARY(LibMatroska_LIBRARY_RELEASE
	  NAMES libmatroska.so libmatroska_md.lib
	  HINTS
	  PATHS
	  ${LibMatroska_LIBRARY_RELEASE_DIR}
	  /usr/local
	  /usr
	)

	FIND_LIBRARY(LibMatroska_LIBRARY_DEBUG
	  NAMES libmatroskad.so libmatroskad_md.lib
	  HINTS
	  PATHS
	  ${LibMatroska_LIBRARY_DEBUG_DIR}
	  /usr/local
	  /usr
	)

	SET(LibMatroska_LIBRARIES ${LibMatroska_LIBRARY_RELEASE})
	SET(LibMatroska_LIBRARY_DIRS ${LibMatroska_LIBRARY_RELEASE_DIR})

	IF (MSVC)
		SET(LibMatroska_LIBRARIES optimized ${LibMatroska_LIBRARY_RELEASE} debug ${LibMatroska_LIBRARY_DEBUG})
		SET(LibMatroska_LIBRARY_DIRS ${LibMatroska_LIBRARY_RELEASE_DIR} ${LibMatroska_LIBRARY_DEBUG_DIR})
	ENDIF ()

	MARK_AS_ADVANCED( LibMatroska_LIBRARY_RELEASE_DIR )
	MARK_AS_ADVANCED( LibMatroska_LIBRARY_DEBUG_DIR )
	MARK_AS_ADVANCED( LibMatroska_LIBRARY_RELEASE )
MARK_AS_ADVANCED( LibMatroska_LIBRARY_DEBUG_DIR )
else()
	if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
		FIND_PATH(LibMatroska_LIBRARY_DIR libmatroska.so libmatroska_md.lib
			HINTS
			PATH_SUFFIXES lib64 Win64 x64 lib/x86_64-linux-gnu
			PATHS
			${LibMatroska_ROOT_DIR}
			/usr/local
			/usr
		)
	else()
		FIND_PATH(LibMatroska_LIBRARY_DIR libmatroska.so libmatroska_md.lib
			HINTS
			PATH_SUFFIXES lib Win32 x86 lib/x86_64-linux-gnu
			PATHS
			${LibMatroska_ROOT_DIR}
			${LibMatroska_ROOT_DIR}/lib
			/usr/local
			/usr
		)
	endif()

	FIND_LIBRARY(LibMatroska_LIBRARY
	  NAMES libmatroska.so libmatroska_md.lib
	  HINTS
	  PATHS
	  ${LibMatroska_LIBRARY_DIR}
	  /usr/local
	  /usr
	)

	SET(LibMatroska_LIBRARIES ${LibMatroska_LIBRARY})
	SET(LibMatroska_LIBRARY_DIRS ${LibMatroska_LIBRARY_DIR})

	MARK_AS_ADVANCED( LibMatroska_LIBRARY_DIR )
	MARK_AS_ADVANCED( LibMatroska_LIBRARY )
endif()
find_package_handle_standard_args( LibMatroska DEFAULT_MSG LibMatroska_LIBRARIES LibMatroska_INCLUDE_DIR )
