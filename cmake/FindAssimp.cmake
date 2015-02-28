#
# Try to find Assimp library and include path.
# Once done this will define
#
# ASSIMP_FOUND
# ASSIMP_INCLUDE_PATH
# ASSIMP_LIBRARIES
# 

FIND_PATH( ASSIMP_INCLUDE_PATH assimp/ai_assert.h
		${ASSIMP_ROOT_DIR}/include )
		
FIND_LIBRARY( ASSIMP_LIBRARY
			NAMES assimp
			PATHS
			${ASSIMP_ROOT_DIR}/lib
		)	

SET(ASSIMP_FOUND "NO")
IF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)
	SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
	SET(ASSIMP_FOUND "YES")
ENDIF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ASSIMP DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_PATH)
