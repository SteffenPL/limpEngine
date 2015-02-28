# FindLuaInterface.cmake
#	defines: 
#		LUA_INTERFACE_INCLUDE_DIR

# find the LuaInterface include directory
find_path(LUA_INTERFACE_INCLUDE_DIR
          PATH_SUFFIXES include
          PATHS
          ${LUA_INTERFACE_ROOT}
          $ENV{LUA_INTERFACE_ROOT}
          ~/Library/Frameworks
          /Library/Frameworks
          /usr/local/
          /usr/
          /sw          # Fink
          /opt/local/  # DarwinPorts
          /opt/csw/    # Blastwave
          /opt/)

