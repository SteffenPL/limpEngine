cmake_minimum_required(VERSION 2.8)

project(LimpGame)


find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( SFML 2 COMPONENTS system window graphics audio REQUIRED )
find_package( Lua51 REQUIRED )
find_package( LimpEngine REQUIRED )
find_package( LuaInterface REQUIRED )
find_package( Assimp REQUIRED )
find_package( Bullet REQUIRED )
find_package( Boost  COMPONENTS log log_setup thread date_time filesystem system REQUIRED)

include_directories( ${GLEW_INCLUDE_PATH} )
include_directories( ${SFML_INCLUDE_DIR} )

include_directories( ${LUA_INCLUDE_DIR} )
include_directories( ${LUA_INTERFACE_INCLUDE_DIR} )
include_directories( ${LIMP_ENGINE_INCLUDE_DIR} )
include_directories( ${ASSIMP_INCLUDE_DIR} )
include_directories( ${BULLET_INCLUDE_DIR} )
include_directories( ${BOOST_INCLUDE_DIRS} )

set( SOURCES
    main.cpp
    #game.cpp
)

set( HEADERS
  game.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} )
target_link_libraries(
                      ${PROJECT_NAME}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${SFML_LIBRARIES}
                      ${LUA_LIBRARY} dl pthread
                      ${BULLET_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${ASSIMP_LIBRARIES}
                      )

