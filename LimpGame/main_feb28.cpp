#include "limpEngine.hpp"

using namespace Limp;

#include <iostream>
using namespace std;


int main()
{
    Log log("log.txt");
    ConfigManager config("config.txt");
    sf::RenderWindow app;


    log.write("C++","Programm gestartet.");

    // init OpenGL
    OpenGL gl( app , config , log );
    app.setFramerateLimit(60);

    sf::View view;
    view.setSize( Vector2D( 2 , 2 ) );
    view.setCenter( Vector2D( 0 , 0 ) );

    app.setView( view );

    // set Background Color
    gl.setClearColor( Color<float,3>( 0.f , 0.f , 0.f ) );

    // load Shaders
    Limp::Shader vertexShader( ShaderType::VertexShader , "shader/vert.glsl" , log );
    Limp::Shader fragShader( ShaderType::FragmentShader , "shader/frag.glsl" , log );

    // Create Shader List
    std::list<Shader> shaderList;
    shaderList.push_back( vertexShader );
    shaderList.push_back( fragShader );

    // Bind Shaderprogram
    Program program( shaderList , log );
    gl.useProgram(program);

    // Create Camera
    Limp::Camera cam;
    cam.setPosition(Vector3D(0,20,20));
    cam.lookAt(Vector3D(0,0,0));


    // Init Matrices
    Limp::Matrix4x4 model;
    Limp::Matrix4x4 projection;

    Uniform<Matrix4x4> uniModelView( program , "matModelView"  , 1);
    Uniform<Matrix4x4> uniProjection(program , "matNormal"     , 1);
    Uniform<Matrix4x4> uniProjection(program , "matProjection" , 1);


    // set Viewport
    float aspect = (float)app.getSize().x / (float)app.getSize().y;
    projection.loadPerspectivProjection_fov( 60.f , aspect , 1.f , 500.f );
    gl.setViewport( (int)app.getSize().x , (int)app.getSize().y );



    Vertex_Colored v;
    v.color = Limp::Color4f(1.f,1.f,1.f,0.5f);

    Mesh<Vertex_Colored> grid;
    {
    grid.type = Limp::PrimitivType::Quads;

    grid.createVertexArray(4);

    v.normal = Vector3D( 0.f , 0.f , 1.f );
    v.position = Vector3D(0.f   , 0.f   , 0.f );
    grid.vertexArray[0] = v;

    v.position = Vector3D(0.f   , 1.f , 0.f );
    grid.vertexArray[1] = v;

    v.position = Vector3D(1.f , 1.f   , 0.f );
    grid.vertexArray[2] = v;

    v.position = Vector3D(1.f , 0.f   , 0.f );
    grid.vertexArray[3] = v;

    grid.createIndexArray(5);
    for( int i = 0 ; i < 5 ; ++i )
        grid.indexArray[i] = i % 4;

    grid.createVertexBuffer();
    grid.createIndexBuffer();
    }

    Mesh<Vertex_Colored> axis;
    {
    axis.type = PrimitivType::Lines;

    axis.createVertexArray(4);

    v.position = Vector3D( 0.f , 0.f , 0.f );
    v.color = Limp::Color4f( 0.5f , 0.5f , 0.5f , 0.f );
    axis.vertexArray[0] = v;

    v.position = Vector3D( 1.f , 0.f , 0.f );
    v.color = Limp::Color4f( 1.f , 0.f , 0.f , 1.f );
    axis.vertexArray[1] = v;

    v.position = Vector3D( 0.f , 1.f , 0.f );
    v.color = Limp::Color4f( 0.f , 1.f , 0.f , 1.f );
    axis.vertexArray[2] = v;

    v.position = Vector3D( 0.f , 0.f , 1.f );
    v.color = Limp::Color4f( 0.f , 0.f , 1.f , 1.f );
    axis.vertexArray[3] = v;

    axis.createVertexBuffer();
    axis.createIndexArray(6);

    for( int i = 0 ; i < 6 ; ++i )
        if( i % 2 == 0 )
            axis.indexArray[i] = i / 2 + 1 ;
        else
            axis.indexArray[i] = 0;

    axis.createIndexBuffer();
    }

    Limp::Mesh<Limp::Vertex_Colored> quad;
    {
        Vertex_Colored defaultVertex;
        defaultVertex.position  = Vector4D();
        defaultVertex.normal    = Vector4D( 0.f , 0.f , 1.f );
        defaultVertex.color     = Limp::Color4f( 0.1f , .3f , .9f , 0.8f ) ;
        quad.createQuad_xy( Vector2D(1,1) , defaultVertex );

    }
/*
    Mesh<Vertex_Textured> cursor;
    {
        Vertex_Textured defaultVertex;
        defaultVertex.position = Vector4D();
        defaultVertex.normal = Vector4D( 0.f , 0.f , 1.f );
        cursor.createQuad_xy( Vector2D(1,1) , defaultVertex );

    }

    Limp::Texture2D cursorTexture;
    cursorTexture.loadFromFile( "data/textures/cursor.png" );
*/
/*
    Mesh<Vertex_Colored> cube;
    {
        cube.type = Limp::PrimitivType::Quads;

        cube.createVertexArray(8);



        for( float x = -1.f ; x < 2.f ; x += 2.f )
            for( float y = -1.f ; y < 2.f ; y += 2.f )
                for( float z = -1.f ; z < 2.f ; z += 2.f )
                {
                    v.position = Vector3D( x , y , z );
                    v.normal = v.position;
                    v.normal.normalize();
                    v.color = Color4f( 1 + x  /2.f ,
                                       1 + y  /2.f ,
                                       1 + z  /2.f ,
                                       1.f  );

                    cube.vertexArray[(int)x + 2*(int)y + 4*(int)z] = v;
                }


        cube.createIndexArray(4 * 2);
        for( int i = 0 ; i < 8 ; ++i )
            cube.indexArray[i] = i;

        cube.createVertexBuffer();
        cube.createIndexBuffer();


    }
*/


    PointLight light;

    light.position = Vector3D( 0.f , 1.f , 0.f );
    light.linear = 1.f;
    light.square = 0.f;
    light.range = 5.f;
    light.diffuse = Color3f( 1.f , 1.f , 1.f , 1.f );
    light.specular = Color3f( 0.f , 0.f , 1.f , 1.f );


    log.write("OpenGL" , "Start rendering...");



    Vector3D quadPos;
    auto relFunction = [](Vector2D v){ return Vector3D(v.x,v.y,2);};
    DirectRelation<Vector2D,Vector3D> rel( relFunction );

    float animationPos = 0;
    auto relFunction2 = [](float a){return Vector3D( 3 * a * ( 2 - a )  ,
                                                     1 * a * a * ( 2 - a ) * ( 2 - a ),
                                                     2 * a * ( 2 - a ) );};
    DirectRelation<float,Vector3D> rel2( relFunction2 );

    gl.clear();

    bool     wasMousePressed = false;
    Vector2D mousePos;
    sf::Clock clock;
    sf::Time frameTime;


    bool running = true;
    while( running )
    {
        frameTime = clock.getElapsedTime();
        clock.restart();
        animationPos += frameTime.asSeconds() / 2 ;
        if( animationPos >= 2)
            animationPos = 0;

        sf::Event event;

        while( app.pollEvent(event) )
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
                    aspect = (float)app.getSize().x / (float)app.getSize().y;
                    gl.setViewport( app.getSize().x ,app.getSize().y );
                    //projection.loadPerspectivProjection_fov(45.f , aspect , 1.f , 500.f );
                    projection.loadPerspectivProjection( aspect , -aspect , 1.f , -1.f , 1.f , 500.f  );
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
                            gl.setClearColor( Color<float,4>(  0.5f + (float) (rand()%20) / 40 ,
                                                              0.25f + (float) (rand()%20) / 40 ,
                                                              0.f ,
                                                              1.f ) );
                        }
                        break;

                        default:break;
                    }

                }
                break;

                default:break;
            }
        }


        if( sf::Mouse::isButtonPressed( sf::Mouse::Right ))
        {


            sf::Vector2i vec2iMousePos = sf::Mouse::getPosition();
            mousePos = app.mapPixelToCoords(vec2iMousePos,view);
            //mousePos = app.convertCoords( vec2iMousePos , view );

            vec2iMousePos = app.mapCoordsToPixel( Vector2D(0.f,0.f) );

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
            speed = max( speed + dspeed , 0.5f ) ;
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



        cam.update();

        gl.clear();


        model.loadIdentity();
        uniModelView = cam.getMatrix() * model;
        uniProjection = projection ;


        axis.draw();


        const float gridResolution = 50 ;
        model.loadIdentity();
        model.rotateX( pi/2 );
        model.translate(Vector3D( -gridResolution /2 , 0.f , -gridResolution /2 ));
        for( int x = 0 ; x < gridResolution ; ++x )
        {
            for( int y = 0 ; y < gridResolution ; ++y )
            {
                //float sin_x = 5 * sinf(x);
                //float cos_y = 5 * cosf(y);

                //model.translate( Vector3D( 0.f , 0.1 * (sin_x + cos_y) , 0.f ));

                uniModelView = cam.getMatrix() * model;
                grid.draw();
                //model.translate( Vector3D( 0.f ,  0.1 * (-sin_x - cos_y)  , 0.f ));

                model.translate(Vector3D( 1.f , 0.f , 0.f ));

            }
            model.translate(Vector3D( -gridResolution , 0.f , 1.f ));
        }



        const int res = 200;
        for( int i = 0 ; i < res ; ++i )
        {
            float tmp = animationPos + (float)i / res * 2;
            while( tmp > 2 )
                tmp -= 2;


            rel2.update(tmp,quadPos);
            model.loadIdentity();
            model.scale( 20*1/(float)res );
            quadPos.x += 10*(float)i/(float)res;
            model.translate(quadPos);

            uniModelView = cam.getMatrix() * model;
            quad.draw();

        }

        //model.loadIdentity();
        //model.scale(5.f);
        //uniModelView = cam.getMatrix() * model;
        //cube.draw();


        model.loadIdentity();
        model.scale( Vector3D( 1.9f , 1.9f , 1.f ) );
        model.translate( Vector3D( 0.f , 0.f , -2.f ) );
        uniModelView = model;
        quad.draw();

        app.display();
    }

    app.close();

    log.write("C++","Programm beendet.");

    return 0;
}
