# Find LuaInterface
#	defines
#		LUA_INTERFACE_INCLUDE_DIR
#		LUA_INTERFACE_LIBRARY
#		LUA_INTERFACE_FOUND

find_path( LUA_INTERFACE_INCLUDE_DIR	luaInterface.hpp
          PATH_SUFFIXES include
          PATHS
          ${LUA_INTERFACE_ROOT}
          $ENV{LUA_INTERFACE_ROOT}
          /usr/local/
          /usr/
          /sw          # Fink
          /opt/local/  # DarwinPorts
          /opt/csw/    # Blastwave
          /opt/)

set(LUA_INTERFACE_FOUND FALSE)

if( LUA_INTERFACE_INCLUDE_DIR )
    set(LUA_INTERFACE_FOUND TRUE)
endif( LUA_INTERFACE_INCLUDE_DIR )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LuaInterface DEFAULT_MSG LUA_INTERFACE_INCLUDE_DIR)
