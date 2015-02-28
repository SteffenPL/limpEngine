#ifndef LIMP_OPENGL_HPP
#define LIMP_OPENGL_HPP

#include <memory>

#include "GL/glew.h"
#include "SFML/OpenGL.hpp"
#include "SFML/Window.hpp"

#include "limpMesh.hpp"
#include "limpMath.hpp"
#include "limpColor.hpp"
#include "limpMaterial.hpp"
#include "limpTexture.hpp"
#include "limpShader.hpp"
#include "limpDefaultGraphicTypes.hpp"
#include "limpLight.hpp"

#include "limpCamera.hpp"
#include "limpAnimation.hpp"

#include "limpLog.hpp"
#include "limpConfigManager.hpp"
#include "limpResourceManager.hpp"
#include "limpPath.hpp"


namespace Limp_Helpers
{
    using uint32 = unsigned int;
    void createWindow(sf::Window& app, Limp::ConfigManager& config);
}


namespace Limp
{

using std::string;
using std::map;

class OpenGL
{
    public:
        OpenGL():
            m_log(Log::getInstance())
        {}


        void init();
        void init(sf::Window& app , ConfigManager& config );

        void clear(GLbitfield bits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        void setViewport( int w , int h );
        void setProjection(float width , float height );

        void setClearColor( const Color<float,4>& c );

        void useProgram( Program p )
        {
            m_programID = p.getID();
            glUseProgram( m_programID );

        }

        void draw();

        std::shared_ptr<Texture2D> 	loadTexture( const Path& path );

        template< typename MeshType , typename MaterialType >
        MeshType* 	loadMesh( const Path& path , const string& objName = "." , bool useArray = false);

        template< typename MaterialType >
        MaterialType* loadMaterial( const Path& path , const string& matName = ".");

        ~OpenGL();

        private:

            Log&            m_log;

            string 									m_dataPath;
            GLuint									m_programID;


            ResourceManager<string,BaseMesh> 		m_mpMeshes;
            ResourceManager<string,Texture2D>		m_mpTextures;
            ResourceManager<string,BaseMaterial>	m_mpMaterials;

};

}




void Limp_Helpers::createWindow(sf::Window& app, Limp::ConfigManager& config)
{
    sf::VideoMode videoMode( config.get<uint32>("width",0) , config.get<uint32>("height",0) , 32 );

    if( videoMode.width == 0 || videoMode.height == 0 )
        videoMode = sf::VideoMode::getDesktopMode();

    sf::ContextSettings settings;
    settings.depthBits         = 24; // 24 Bits Depth Buffer
    settings.stencilBits       = 8;  // 8 Bits Stenil Buffer
    settings.antialiasingLevel = config.get<uint32>("antialiasing",0);
    settings.majorVersion = 3;

    if( config.get<bool>("fullscreen",false) )
        app.create( videoMode , "LimpEngine - OpenGL/SFML" ,
                    sf::Style::Fullscreen , settings);
    else
        app.create( videoMode , "LimpEngine - OpenGL/SFML" ,
                    sf::Style::Resize | sf::Style::Close | sf::Style::Titlebar , settings);

    app.setVerticalSyncEnabled( config.get<bool>("vsync",false) );

}


void Limp::OpenGL::init()
{
    glewInit();

    // Z Buffer aktivieren
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_log.write("OpenGL","OpenGL initialisiert.");
}

void Limp::OpenGL::init(sf::Window& app , ConfigManager& config )
{
    Limp_Helpers::createWindow(app,config);
    m_log.write("SFML","Fenster erstellt.");

    {
        std::stringstream ss;
        ss << app.getSize().x << " x " << app.getSize().y;
        m_log.write( "SFML" ,  "Auflösung: " + ss.str() );
    }

    init();

    {
        std::stringstream ss;
        ss << "OpenGL Version:\t" << app.getSettings().majorVersion << "."
            << app.getSettings().minorVersion ;
        m_log.write("OpenGL" , ss.str() );
    }
}

void Limp::OpenGL::setClearColor( const Color<float,4>& c )
{
    glClearDepth(1.f);
    glClearColor( c.r , c.g , c.b , c.a );
}

void Limp::OpenGL::setViewport( int w , int h )
{
    glViewport(0,0,w,h);
}

void Limp::OpenGL::setProjection(float width , float height )
{
    glViewport( 0 , 0 , (GLsizei) width , (GLsizei) height );

    float aspect = width / height;

    float f = 500.f;
    float n = 1.f;

    const float t =  fabsf( tan( 45.f * 0.5f) * n );
    const float r = t * aspect;
    const float l = -r;
    const float b = -t;

    float m[16] =
        {
            2*n / (r - l),	0.f,			(r+l)/(r-l),	0.f,
            0.f	,			2*n / (t - b), 	(t+b)/(t-b), 	0.f,
            0.f	,			0.f, 			(n+f)/(n-f), 	2*n*f/(n-f),
            0.f	,			0.f, 			-1.f, 			0.f
        };

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m);
}

std::shared_ptr<Limp::Texture2D> 	Limp::OpenGL::loadTexture( const Path& path )
{
    if( !m_mpTextures.exists( path.filename().string() ) )
    {
        Texture2D* pTex = new Texture2D();
        if( pTex->loadFromFile( path.string() ) )
        {
            m_log.write("OpenGL" , path.filename().string() + " erfolgreich geladen.");
            return m_mpTextures.set( path.filename().string() , pTex );

        }
        else
        {
            m_log.write("OpenGL", path.filename().string() + " nicht erfolgreich geladen.");
            return NULL;
        }
    }
    return m_mpTextures.get( path.filename().string() );
}

template< typename MeshType , typename MaterialType >
MeshType* 	Limp::OpenGL::loadMesh( const Path& path , const std::string& objName , bool useArray )
{
    if( !m_mpMeshes.exists( path.filename().string() + "." + objName  ) )
    {
        if( m_mpMeshes.exists( path.filename().string() + "." ) )
        {
            m_log.write("Limp:", "Objekte '" + objName + "' in '" + (std::string)path + "' nicht vorhanden.");
            return NULL;
        }

        m_mpMeshes[ path.filename().string() + "." ] = NULL;

        std::ifstream file;
        file.open( path.c_str() );

        if( !file.is_open() )
        {
            m_log.write("Error" , "OpenGL.loadMesh  konnte '" + (string)path + "' nicht oeffnen." );
        }

        std::vector<Vector3D> 	vec_v;
        std::vector<Vector3D> 	vec_vn;
        std::vector<Vector2D> 	vec_vt;
        std::vector<uint8> 		vec_indices;
        PrimitivType 			type;

        std::vector<std::string> vec_mtllibs;

        MeshType* 	pMesh 	= NULL;
        BaseMaterial*		pMat 	= NULL;

        bool empty = true;

        while( file.good() )
        {
            std::string mode;
            std::string line;
            std::stringstream ss;
            std::getline(file,line);

            ss << line;
            if( ss.good() )
                ss >> mode;
            else
                mode = "#";

            if( mode == "#" )
            {}
            else if( mode == "v" )
            {
                Vector4D v;
                ss >> v.x >> v.y >> v.z;
                vec_v.push_back(v);
            }
            else if( mode == "vt" )
            {
                Vector2D vt;
                ss >> vt.x >> vt.y;
                vec_vt.push_back(vt);
            }
            else if( mode == "vn" )
            {
                Vector3D vn;
                ss >> vn.x >> vn.y >> vn.z;
                vn /= sqrtf( vn.x * vn.x + vn.y * vn.y + vn.z * vn.z );
                vec_vn.push_back(vn);
            }
            else if( mode == "p" )
            {
                float index;
                ss >> index;
                vec_indices.push_back(index);

                type = Limp::PrimitivType::Points;
            }
            else if( mode == "l" )
            {
                float i1, i2;
                ss >> i1 >> i2;
                vec_indices.push_back( i1 );
                vec_indices.push_back( i2 );

                type = Limp::PrimitivType::Lines;
            }
            else if( mode == "f" )
            {
                int c = 0;
                while( ss.good() )
                {
                    float index;
                    ss >> index;
                    vec_indices.push_back(index);
                    ++c;
                }

                if( c == 3 )
                    type = Limp::PrimitivType::Triangles;
                else
                    type = Limp::PrimitivType::Quads;
            }
            else if( mode == "mtllib" )
            {
                std::string matFile;
                ss >> matFile;
                matFile = path.parent_path().string() + matFile;
                loadMaterial<MaterialType>( Path(matFile) );

                vec_mtllibs.push_back( matFile );
            }
            else if( mode == "usemtl" )
            {
                std::string matName;
                ss >> matName;
                for( std::string& matFile : vec_mtllibs )
                {
                    BaseMaterial* pTmp = loadMaterial<MaterialType>( Path(matFile) , matName );
                    if( pTmp != NULL )
                    {
                        pMat = pTmp;
                        break;
                    }
                }
            }

            empty = ( vec_v.size() == 0
                        && vec_vn.size() == 0
                        && vec_vt.size() == 0
                        && vec_indices.size() == 0
                        && pMat == NULL );


            if( !file.good() || mode == "o" )
            {
                if( !empty && pMesh != NULL )
                {
                    size_t vertexCount = vec_v.size();

                    if( vertexCount > 0 && pMesh != NULL)
                    {
                        bool useNormalData = ( vec_vn.size() > 0 );
                        bool useTextureData = ( vec_vt.size() > 0 );

                        if(    	( vertexCount != vec_vn.size() && useNormalData )
                            || 	( vertexCount != vec_vt.size() && useTextureData ) )
                        {
                            m_log << "Limp\tloadFromFile(...): Unvollständige VertexDaten.\n";
                            m_log << "\t\t(v: " << vec_v.size() << "; vn: " << vec_vn.size() << "; vt: " << vec_vt.size() << ")\n";
                        }

                        pMesh->createVertexArray( (int)vertexCount );

                        for( size_t i = 0 ; i < vertexCount ; ++i )
                        {
                            pMesh->vertexArray[i].setPosition( vec_v[i] );
                            if( useNormalData ) pMesh->vertexArray[i].setNormal( vec_vn[i] );
                            if( useTextureData ) pMesh->vertexArray[i].setTexCoord( vec_vt[i] );
                        }

                        pMesh->createIndexArray( (int)vec_indices.size() );

                        int i = 0;
                        for( uint8 index : vec_indices )
                        {
                            pMesh->indexArray[i] = index;
                            ++i;
                        }

                        pMesh->setType( type );

                        pMesh->setMaterial( pMat );

                        pMesh->createVertexBuffer();
                        pMesh->createIndexBuffer();

                        if( !useArray )
                        {
                            pMesh->clearVertexArray();
                            pMesh->clearIndexArray();
                        }
                    }
                }


                if( mode == "o")
                {
                    std::string objName;
                    ss >> objName;
                    pMesh = new MeshType;
                    m_mpMeshes[ path.filename().string() + "." + objName] = pMesh;
                }
            }
        }
        file.close();
        m_log.write("Limp", "'" + path.string() + "' erfolgreich geladen.");

    }
    if( m_mpMeshes.exists( path.filename().string() + "." + objName) )
        return (MeshType*)m_mpMeshes.get( path.filename().string() + "." + objName );
    else
        return NULL;
}

template< typename MaterialType >
MaterialType* Limp::OpenGL::loadMaterial( const Path& path , const std::string& matName )
{
    if( !m_mpMaterials.exists( path.filename().string() + "." + matName  ) )
    {
        if( m_mpMaterials.exists( path.filename().string() + "." ) )
            return NULL;

        std::ifstream file( path );

        if( !file.is_open() )
            return NULL;

        using std::string;

        Color4f kd , ka , ks;
        float tr , ns;
        int ilum = 0;
        string map_kd, map_ka , map_ks , map_tr , map_bump;

        BaseMaterial* pMat;

        while( file.good() )
        {
            std::string mode;
            std::string line;
            std::stringstream ss;
            std::getline(file,line);

            ss << line;
            if( ss.good() )
                ss >> mode;
            else
                mode = "#";

            if( mode == "#" )
            {}
            else if( mode == "Kd" )
            {
                ss >> kd.r >> kd.g >> kd.b >> kd.a;
            }
            else if( mode == "Ka" )
            {
                ss >> ka.r >> ka.g >> ka.b >> ka.a;
            }
            else if( mode == "Ks" )
            {
                ss >> ks.r >> ks.g >> ks.b >> ks.a;
            }
            else if( mode == "Tr" || mode == "d" )
            {
                ss >> tr;
            }
            else if( mode == "Ns" )
            {
                ss >> ns;
            }
            else if( mode == "ilum" )
            {
                ss >> ilum;
            }
            else if( mode == "map_Kd" )
            {
                ss >> map_kd;
            }
            else if( mode == "map_Ka" )
            {
                ss >> map_ka;
            }
            else if( mode == "map_Ks" )
            {
                ss >> map_ks;
            }
            else if( mode == "map_tr" || mode == "map_d" )
            {
                ss >> map_tr;
            }
            else if( mode == "map_bump" || mode == "bump" )
            {
                ss >> map_bump;
            }


            if( !file.good() || mode == "newmtl" )
            {
                if( pMat != NULL )
                {
                    pMat->setDiffuse(kd);
                    pMat->setAmbient(ka);
                    pMat->setSpecular(ks);
                    pMat->setAlpha( tr );
                    pMat->setShininess( ns );
                    pMat->setDiffuseMap( loadTexture(map_kd) );
                    pMat->setAmbientMap( loadTexture(map_ka) );
                    pMat->setSpecularMap( loadTexture(map_ks) );
                    pMat->setAlphaMap( loadTexture(map_tr) );
                    pMat->setBumpMap( loadTexture(map_bump) );
                }

                if( mode == "newmtl")
                {
                    std::string matName;
                    ss >> matName;
                    pMat = new MaterialType();
                    m_mpMaterials[ path.filename().string() + "." + matName ] = pMat;
                }
            }
        }

        file.close();
        m_log.write("Limp", "'" + path.filename().string() + "' erfolgreich geladen.");
    }

    if( m_mpMaterials.exists(path.filename().string() + "." + matName) )
        return (MaterialType*)m_mpMaterials.get( path.filename().string() + "." + matName );
    else
        return NULL;
}

void Limp::OpenGL::clear( GLbitfield bits )
{
    glClear( bits );
}


Limp::OpenGL::~OpenGL()
{
}

#endif //LIMP_OPENGL_HPP
