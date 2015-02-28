cmake_minimum_required( VERSION 2.8 )

project( LimpEngine )

find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( SFML 2 COMPONENTS system window graphics audio REQUIRED )
find_package( Lua51 REQUIRED )
find_package( Bullet REQUIRED )
find_package( Boost  REQUIRED filesystem )


include_directories( ${GLEW_INCLUDE_PATH} )
include_directories( ${SFML_INCLUDE_DIR} )
include_directories( ${LUA_INCLUDE_DIR} )
include_directories( ${LUA_INTERFACE_INCLUDE_DIR} )
include_directories( ${BULLET_INCLUDE_DIR} )
include_directories( ${BOOST_INCLUDE_DIR} )





set( SOURCES
  #limpConfigManager.cpp
  limpOpenGL.cpp
)

set( HEADERS
    limpGame.hpp
    limpAnimation.hpp
    limpRelation.hpp
    limpScene.hpp
    limpGraphicsObject.hpp
    limpShader.hpp
    limpBufferObject.hpp
    limpLight.hpp
    limpSoundObject.hpp
    limpCamera.hpp
    limpLog.hpp
    limpStateManager.hpp
    limpColor.hpp
    limpMaterial.hpp
    limpTexture.hpp
    limpMath.hpp
    limpTypes.hpp
    limpConfigManager.hpp
    limpMesh.hpp
    limpUI.hpp
    limpDefaultGraphicTypes.hpp
    limpObject.hpp
    limpUniform.hpp
    limpEngine.hpp
    limpVertex.hpp
    limpOpenGL.hpp
    limpWorld.hpp
    limpPath.hpp
    limpPhysicsObject.hpp
    limpFramebuffer.hpp
    limpResourceManager.hpp
    limpOpenGLRenderer.hpp
)

add_library( ${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} )

target_link_libraries(${PROJECT_NAME}
                      ${OPENGL_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${SFML_LIBRARIES}
                      ${LUA_LIBRARY}
                      ${BULLET_LIBRARIES}
                      ${BOOST_LIBRARIES}
                      dl
                      )#${ASSIMP_LIBRARIES})

