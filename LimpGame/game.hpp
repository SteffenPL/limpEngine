#ifndef GAME_HPP
#define GAME_HPP

#include "limpEngine.hpp"
#include "limpGraphicsObject.hpp"
#include "limpOpenGLRenderer.hpp"
#include "luaInterface.hpp"


#include <btBulletDynamicsCommon.h>

using std::string;
using std::shared_ptr;
using std::unique_ptr;

class Game
{

public:
    Game();

    bool init();

    bool run();

    void initProjectionMatrix( Limp::Matrix4x4& mat );



private:

    Limp::Log&          m_log;
    Limp::ConfigManager m_config;
    sf::RenderWindow    m_app;
    sf::View            m_view;

    Limp::OpenGL    m_gl;
    LuaState  m_lua;

    std::map<string,Limp::Program>          m_programs;
    std::map<string,Limp::Shader>           m_shader;

    Limp::ResourceManager<string,Limp::BaseMesh>       m_meshes;
    Limp::ResourceManager<string,Limp::OpenGLRenderer> m_renderer;

};



using namespace Limp;

Game::Game():
    m_log( Log::getInstance() )
{

}

bool Game::init()
{
    // init Log
    m_log.open("log.txt");
    if( !m_log.is_open() ) return false;

    m_log.write("C++","Programm gestartet.");


    m_config.readFile("config.txt");

    // init OpenGL
    m_gl.init( m_app , m_config );
            // TODO: Error abfangen
    m_app.setFramerateLimit(60);


    m_view.setSize( Vector2D( 2 , 2 ) );
    m_view.setCenter( Vector2D( 0 , 0 ) );

    m_app.setView( m_view );

    // set Background Color
    m_gl.setClearColor( Color<float,3>( 0.f , 0.f , 0.f ) );


    auto renderer = m_renderer.setNew<OpenGLRenderer>( "colored_vertex" );
    renderer->initProgram({"shader/vert.glsl"},{"shader/frag.glsl"});
    m_gl.useProgram(renderer->program);

    Log::registerClass(m_lua);
    //Box::registerClass(lua);
    World::registerClass(m_lua);

    m_lua.set<Log*>("log",&m_log);

    return true;
}

bool Game::run()
{

    Vertex_Colored v;
    v.color = Limp::Color4f(1.f,1.f,1.f,0.5f);

    using VCMesh =  Mesh<Vertex_Colored>;


    auto grid = m_meshes.setNew<VCMesh>("grid");
    grid->createQuad( Vector3D(1.f,0.f,0.f)
                     ,Vector3D(0.f,0.f,1.f)
                     ,{ Vector4D(0.5f,0.f,0.5f,0.f)
                       ,Vector4D( 0.f , 1.f , 0.f )
                       ,Color4f( 1.f , 1.0f , 1.f , 0.4f )});

    grid->type = Limp::PrimitivType::LineStrip;


    auto axis = m_meshes.setNew<VCMesh>("axis");
    {
        axis->type = PrimitivType::Lines;

        axis->createVertexArray(4);

        v.position = Vector3D( 0.f , 0.f , 0.f );
        v.color = Limp::Color4f( 0.5f , 0.5f , 0.5f , 0.f );
        axis->vertexArray[0] = v;

        v.position = Vector3D( 1.f , 0.f , 0.f );
        v.color = Limp::Color4f( 1.f , 0.f , 0.f , 1.f );
        axis->vertexArray[1] = v;

        v.position = Vector3D( 0.f , 1.f , 0.f );
        v.color = Limp::Color4f( 0.f , 1.f , 0.f , 1.f );
        axis->vertexArray[2] = v;

        v.position = Vector3D( 0.f , 0.f , 1.f );
        v.color = Limp::Color4f( 0.f , 0.f , 1.f , 1.f );
        axis->vertexArray[3] = v;

        axis->createVertexBuffer();
        axis->createIndexArray(6);

        for( int i = 0 ; i < 6 ; ++i )
            if( i % 2 == 0 )
                axis->indexArray[i] = i / 2 + 1 ;
            else
                axis->indexArray[i] = 0;

        axis->createIndexBuffer();
    }

    auto quad = m_meshes.setNew<VCMesh>("quad");
    quad->createQuad( Vector3D(1.f,0.f,0.f)
                         ,Vector3D(0.f,0.f,1.f)
                         ,{  Vector4D()
                           , Vector4D( 0.f , 0.f , 1.f )
                           , Color4f( 1.f , .5f , 0.f , 1.0f )});


    Mesh<Vertex_Textured> cursor;
    cursor.createQuad(Vector3D(1.f,0.f,0.f)
                      ,Vector3D(0.f,0.f,1.f)
                      ,{Vector4D()
                        ,Vector4D(0.f,0.f,1.f)
                        ,Vector2D()});

    Limp::Texture2D cursorTexture;
    cursorTexture.loadFromFile( "data/textures/cursor.png" );


    auto cube = m_meshes.setNew<VCMesh>("cube");
    cube->createCube( Vector3D(1.f,1.f,1.f)
                      , { Vector4D()
                         ,Vector4D()
                         ,Color4f( 0.f , .5f , 0.9f , 0.8f )} );



    ParticleObject particles;
    particles.setMesh(cube);
    particles.setParticleCount(100*100);

    particles.simulate();


    shared_ptr<OpenGLRenderer> renderer = m_renderer.get("colored_vertex");

    // Create Camera
    Limp::Camera cam;
    cam.setPosition(Vector3D(0,20,30));
    cam.lookAt(Vector3D(0,0,0));


    // Init Matrices
    Limp::Matrix4x4 model;
    Limp::Matrix4x4 modelView;
    Limp::Matrix4x4 projection;

    // set Viewport
    initProjectionMatrix(projection);

    PointLight light;

    light.position = Vector4D( 0.f , 1.f , 0.f , 0.f );
    light.linear = 1.f;
    light.square = 0.f;
    light.range = 100.f;
    light.diffuse = Color4f( 1.f , 1.f , 0.f , 1.f );
    light.specular = Color4f( 0.f , 0.f , 1.f , 1.f );

    auto uniLight = light.initUniform( renderer->program , "light[0]" );
    light.updateUniform( uniLight );

    Uniform<int> uniLightOn( renderer->program , "lightOn" , 1 );
    uniLightOn = 0;

    m_log.write("OpenGL" , "Start rendering...");

    bool     wasMousePressed = false;
    Vector2D mousePos;
    sf::Clock clock;
    sf::Time frameTime;
    sf::Time totalTime;
    bool showGrid = true;
    bool terminalActive = false; terminalActive = !terminalActive;
    string lua_command = "";



    m_lua.set<float>("time",0.f);

    bool ok  = m_lua.loadProgramm( m_config.get<string>("start_script","scripts/start.lua") );
         ok &= m_lua.execute();

    //if( !ok ) m_log.write("Lua",m_lua.getError());

    // BULLET SANDBOX

    btDbvtBroadphase broadphase;
    btDefaultCollisionConfiguration collisionConfig;

    btCollisionDispatcher dispatcher(&collisionConfig);

    btSequentialImpulseConstraintSolver solver;


    btDiscreteDynamicsWorld dynWorld(&dispatcher,&broadphase,&solver,&collisionConfig);
    dynWorld.setGravity( btVector3(0,-10,0) );

    btStaticPlaneShape groundShape( btVector3(0,1,0) , 1 );
    btSphereShape fallShape(1);
    btSphereShape fallShape2(1);

    btDefaultMotionState groundMotionState( btTransform(btQuaternion(0,0,0,1) , btVector3(0,-1,0) ) );

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI( 0 , &groundMotionState, &groundShape , btVector3(0,0,0) );
    btRigidBody groundRigidBody( groundRigidBodyCI );


    dynWorld.addRigidBody(&groundRigidBody);

    btDefaultMotionState fallMotionState( btTransform(btQuaternion(0,0,0,-1) , btVector3(0,50,0) ) );

    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallShape.calculateLocalInertia(mass,fallInertia);

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass , &fallMotionState , &fallShape , fallInertia );
    btRigidBody fallRigidBody( fallRigidBodyCI );

    dynWorld.addRigidBody(&fallRigidBody);

    btDefaultMotionState fallMotionState2( btTransform(btQuaternion(0,0,0,1) , btVector3(0,40,0) ) );

    btVector3 fallInertia2(0,0,0);
    fallShape2.calculateLocalInertia(mass,fallInertia2);

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2( mass , &fallMotionState2 , &fallShape2 , fallInertia2 );
    btRigidBody fallRigidBody2( fallRigidBodyCI2 );

    dynWorld.addRigidBody(&fallRigidBody2);

    GraphicsObject cubeObj;
    cubeObj.setMesh( cube );
    GraphicsObject cubeObj2;
    cubeObj2.setMesh( cube );




    bool running = true;
    while( running )
    {
        frameTime = clock.getElapsedTime();
        totalTime += frameTime;
        clock.restart();


        m_lua.set<float>("time",totalTime.asSeconds());
        m_lua.callFunction<int>("update");

        sf::Event event;

        while( m_app.pollEvent(event) )
        {
            switch( event.type )
            {
                case sf::Event::Closed:
                {
                    running = false;
                }
                break;

                case sf::Event::Resized:
                {
                    initProjectionMatrix(projection);
                }
                break;

                case sf::Event::KeyPressed:
                {

                    switch( event.key.code )
                    {
                        case sf::Keyboard::I :
                        {
                            cout << cam.getMatrix().toString();
                        }
                        break;

                        case sf::Keyboard::B:
                        {
                            m_gl.setClearColor( Color<float,4>(  0.5f + (float) (rand()%20) / 40 ,
                                                              0.25f + (float) (rand()%20) / 40 ,
                                                              0.f ,
                                                              1.f ) );
                        }
                        break;

                        case sf::Keyboard::G:
                        {

                        showGrid = !showGrid;
                        }
                        break;

                    default:break;

                    }

                }
                break;

                default:break;
            }
        }


        sf::Vector2i vec2iMousePos = sf::Mouse::getPosition();
        mousePos = m_app.mapPixelToCoords(vec2iMousePos,m_view);

        m_lua.set<float>("char_x",mousePos.x*50);
        m_lua.set<float>("char_z",mousePos.y*50);

        if( sf::Mouse::isButtonPressed( sf::Mouse::Right ))
        {
            //mousePos = app.convertCoords( vec2iMousePos , view );

            vec2iMousePos = m_app.mapCoordsToPixel( Vector2D(0.f,0.f) );

            sf::Mouse::setPosition( vec2iMousePos );

            if( wasMousePressed )
            {

                cam.yaw( -mousePos.x );
                cam.pitch( -mousePos.y );
            }

            wasMousePressed = true;
        }
        else
            wasMousePressed = false;


        static float speed = 0.1f;
        static float dspeed = frameTime.asSeconds() * speed;

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ))
            speed = std::max( speed + dspeed , 0.5f ) ;
        else
            speed = 0.1f;

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ))
                cam.move(Vector3D(0,0,speed));

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ))
                cam.move(Vector3D(0,0,-speed));

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ))
                cam.move(Vector3D(speed,0,0));

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ))
                cam.move(Vector3D(-speed,0,0));

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::E ))
                cam.move(Vector3D(0,speed,0));

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ))
                cam.move(Vector3D(0,-speed,0));

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ))
                cam.yaw( -0.05 );

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ))
                cam.yaw( 0.05 );

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ))
                cam.pitch( 0.05 );

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ))
                cam.pitch( -0.05 );

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ))
                cam.roll( 0.05 );

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::T ))
                cam.roll( -0.05 );

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::C ))
        {
            fallRigidBody.activate(true);
            fallRigidBody.applyCentralImpulse( btVector3(0,1,0));
            fallRigidBody2.activate(true);
            fallRigidBody2.applyCentralImpulse( btVector3(1,2,0));
        }

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ))
                running = false;




        cam.update();

        m_gl.clear();
        renderer->uniProjection = projection ;

        if( showGrid )
        {

            model.loadIdentity();
            renderer->uniModelView = cam.getMatrix() * model;
            axis->draw();

            //draw grid
            const float gridResolution = 50 ;
            model.loadIdentity();
            model.translate(Vector3D( -gridResolution /2 , 0.f , -gridResolution /2 ));
            for( int x = 0 ; x < gridResolution ; ++x )
            {
                for( int y = 0 ; y < gridResolution ; ++y )
                {
                    renderer->uniModelView = cam.getMatrix() * model;
                    grid->draw();

                    model.translate(Vector3D( 1.f , 0.f , 0.f ));
                }
                model.translate(Vector3D( -gridResolution , 0.f , 1.f ));
            }
        }

        uniLightOn = 0;


        dynWorld.stepSimulation(frameTime.asSeconds() , 2 );

        btTransform trans;
        fallRigidBody.getMotionState()->getWorldTransform(trans);
        cubeObj.getTransform() = Matrix4x4(trans);
        cubeObj.render(*renderer,cam);

        btTransform trans2;
        fallRigidBody2.getMotionState()->getWorldTransform(trans2);
        cubeObj2.getTransform() = Matrix4x4(trans2);
        cubeObj2.render(*renderer,cam);


        /*particles.setTime(totalTime.asSeconds());
        particles.simulate();
        particles.render(*renderer,cam);
        */
        /*for( int i = 0 ; i < box->boxCount ; ++i )
        {

            model.loadIdentity();
            model.translate(box->pos[i]);
            modelView = cam.getMatrix() * model;
            uniModelView = modelView;


            cube->draw();

        }*/


        m_app.display();
    }

    return true;
}


void Game::initProjectionMatrix( Matrix4x4& mat )
{
    float aspect = (float)m_app.getSize().x / (float)m_app.getSize().y;
    m_gl.setViewport( m_app.getSize().x ,m_app.getSize().y );
    mat.loadPerspectivProjection( aspect , -aspect , 1.f , -1.f , 1.f , 1000.f  );
}

#endif // GAME_HPP
