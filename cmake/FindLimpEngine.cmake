# Find LimpEngine
#	defines
#		LIMP_ENGINE_INCLUDE_DIR
#		LIMP_ENGINE_FOUND

find_path( LIMP_ENGINE_INCLUDE_DIR	limpEngine.hpp
          PATHS
          ${LIMP_ENGINE_ROOT}  
          $ENV{LIMP_ENGINE_ROOT}        
		  /usr/local/
          /usr/
          /sw          # Fink
          /opt/local/  # DarwinPorts
          /opt/csw/    # Blastwave
          /opt/ )

set(LIMP_ENGINE_FOUND FALSE)
if( LIMP_ENGINE_INCLUDE_DIR )
    set(LIMP_ENGINE_FOUND TRUE)
endif( LIMP_ENGINE_INCLUDE_DIR )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LimpEngine DEFAULT_MSG LIMP_ENGINE_INCLUDE_DIR)