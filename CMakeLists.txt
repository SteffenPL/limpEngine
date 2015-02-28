cmake_minimum_required( VERSION 2.8 )


set( CMAKE_MODULE_PATH      ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(Limp)

if( THIRDPARTY_LIBS_DIR )
  message( "Thirdparty libs: " ${THIRDPARTY_LIBS_DIR} )


  set( SFML_ROOT ${THIRDPARTY_LIBS_DIR}/SFML-2.0 )
  set( GLEW_ROOT_DIR ${THIRDPARTY_LIBS_DIR}/glew-1.9.0 )
  set( ASSIMP_ROOT_DIR ${THIRDPARTY_LIBS_DIR}/assimp-3.0 )
  set( ENV{LUA_DIR} ${THIRDPARTY_LIBS_DIR}/Lua-5.1 )


  FIND_LIBRARY( ASSIMP_LIBRARY
        NAMES assimp
        PATHS
        ${THIRDPARTY_LIBS_DIR}/lib
      )
  set( ASSIMP_LIBRARIES ${ASSIMP_LIBRARIES} )


endif( THIRDPARTY_LIBS_DIR )

set( LIMP_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/LimpEngine )
set( LUA_INTERFACE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/LuaInterface-1.0/include )

set( SFML_STATIC_LIBRARIES On )
add_definitions( -DGLEW_STATIC )


#set(Boost_USE_STATIC_LIBS        OFF) # only find static libs
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)


add_definitions( -std=c++11 )
add_definitions( -Wall )
add_definitions( -pthread )

add_definitions(-DBOOST_LOG_DYN_LINK)

add_subdirectory( LuaInterface-1.0 )
add_subdirectory( LimpEngine )
add_subdirectory( LimpGame )

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shared/" DESTINATION bin)

