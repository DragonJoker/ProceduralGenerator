FIND_PATH(LibEbml_ROOT_DIR ebml/EbmlVersion.h
	HINTS
	PATH_SUFFIXES libebml
	PATHS
	/usr/local
	/usr
)

FIND_PATH(LibEbml_INCLUDE_DIR ebml/EbmlVersion.h 
	HINTS
	PATH_SUFFIXES
	PATHS
	${LibEbml_ROOT_DIR}
	/usr/local/include
	/usr/include
)

if(MSVC)
	if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
		FIND_PATH(LibEbml_LIBRARY_RELEASE_DIR libebml.so libebml_md.lib
			HINTS
			PATH_SUFFIXES lib64 Win64 x64
			PATHS
			${LibEbml_ROOT_DIR}
			/usr/local
			/usr
		)

		FIND_PATH(LibEbml_LIBRARY_DEBUG_DIR libebml.so libebmld_md.lib
			HINTS
			PATH_SUFFIXES lib64 Win64 x64
			PATHS
			${LibEbml_ROOT_DIR}
			/usr/local
			/usr
		)
	else()
		FIND_PATH(LibEbml_LIBRARY_RELEASE_DIR libebml.so libebml_md.lib
			HINTS
			PATH_SUFFIXES lib Win32 x86
			PATHS
			${LibEbml_ROOT_DIR}
			${LibEbml_ROOT_DIR}/lib
			/usr/local
			/usr
		)

		FIND_PATH(LibEbml_LIBRARY_DEBUG_DIR libebmld.so libebmld_md.lib
			HINTS
			PATH_SUFFIXES lib Win32 x86
			PATHS
			${LibEbml_ROOT_DIR}
			${LibEbml_ROOT_DIR}/lib
			/usr/local
			/usr
		)
	endif()

	FIND_LIBRARY(LibEbml_LIBRARY_RELEASE
	  NAMES libebml.so libebml_md.lib
	  HINTS
	  PATHS
	  ${LibEbml_LIBRARY_RELEASE_DIR}
	  /usr/local
	  /usr
	)

	FIND_LIBRARY(LibEbml_LIBRARY_DEBUG
	  NAMES libebmld.so libebmld_md.lib
	  HINTS
	  PATHS
	  ${LibEbml_LIBRARY_DEBUG_DIR}
	  /usr/local
	  /usr
	)

	SET(LibEbml_LIBRARIES ${LibEbml_LIBRARY_RELEASE})
	SET(LibEbml_LIBRARY_DIRS ${LibEbml_LIBRARY_RELEASE_DIR})

	IF (MSVC)
		SET(LibEbml_LIBRARIES optimized ${LibEbml_LIBRARY_RELEASE} debug ${LibEbml_LIBRARY_DEBUG})
		SET(LibEbml_LIBRARY_DIRS ${LibEbml_LIBRARY_RELEASE_DIR} ${LibEbml_LIBRARY_DEBUG_DIR})
	ENDIF ()

	MARK_AS_ADVANCED( LibEbml_LIBRARY_RELEASE_DIR )
	MARK_AS_ADVANCED( LibEbml_LIBRARY_DEBUG_DIR )
	MARK_AS_ADVANCED( LibEbml_LIBRARY_RELEASE )
	MARK_AS_ADVANCED( LibEbml_LIBRARY_DEBUG_DIR )
else()
	if (CMAKE_CL_64 OR CMAKE_GENERATOR MATCHES Win64)
		FIND_PATH(LibEbml_LIBRARY_DIR libebml.so libebml_md.lib
			HINTS
			PATH_SUFFIXES lib64 Win64 x64 lib/x86_64-linux-gnu
			PATHS
			${LibEbml_ROOT_DIR}
			/usr/local
			/usr
		)
	else()
		FIND_PATH(LibEbml_LIBRARY_DIR libebml.so libebml_md.lib
			HINTS
			PATH_SUFFIXES lib Win32 x86 lib/x86_64-linux-gnu
			PATHS
			${LibEbml_ROOT_DIR}
			${LibEbml_ROOT_DIR}/lib
			/usr/local
			/usr
		)
	endif()

	FIND_LIBRARY(LibEbml_LIBRARY
	  NAMES libebml.so libebml_md.lib
	  HINTS
	  PATHS
	  ${LibEbml_LIBRARY_RELEASE_DIR}
	  /usr/local
	  /usr
	)

	SET(LibEbml_LIBRARIES ${LibEbml_LIBRARY})
	SET(LibEbml_LIBRARY_DIRS ${LibEbml_LIBRARY_DIR})

	MARK_AS_ADVANCED( LibEbml_LIBRARY_DIR )
	MARK_AS_ADVANCED( LibEbml_LIBRARY )
endif()
find_package_handle_standard_args( LibEbml DEFAULT_MSG LibEbml_LIBRARIES LibEbml_INCLUDE_DIR )
