#ifndef LIMP_MESH_HPP
#define LIMP_MESH_HPP

#include "GL/glew.h"

#include "limpVertex.hpp"
#include "limpBufferObject.hpp"
#include "limpMaterial.hpp"
#include "limpColor.hpp"
#include "limpResourceManager.hpp"
#include "limpPath.hpp"
#include "limpLog.hpp"

#include "limpDefaultGraphicTypes.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

//#include "assimp\Importer.hpp"

namespace Limp
{
    struct BaseMesh;

    using uint8 = unsigned char;
    using MeshMap = ResourceManager<std::string,BaseMesh>;
    using MaterialMap = ResourceManager<std::string,BaseMaterial>;


    enum class PrimitivType : GLenum
    {
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS
    };



struct BaseMesh
{
    public:
        BaseMesh(){}

        virtual void draw(){}

        virtual void setType( PrimitivType t ){}

        virtual void createVertexArray( int size ){}

        virtual void setVertexArray( AbstractVertex* pVertices , int size ){}
        virtual void clearVertexArray(){}


        virtual void createIndexArray( int size ){}
        virtual void setIndexArray( uint8* pIndices , int size ){}
        virtual void clearIndexArray(){}

        virtual void createVertexBuffer( GLenum target = GL_STATIC_DRAW ){}
        virtual void clearVertexBuffer(){}
        virtual void createIndexBuffer( GLenum target = GL_STATIC_DRAW ){}
        virtual void clearIndexBuffer(){}
        virtual void clear(){}

        virtual void setMaterial( BaseMaterial* pMat ){}

        static BaseMesh* loadFromFile( Path path );

        virtual ~BaseMesh(){}
};


template< typename VertexType >
struct Mesh : BaseMesh
{
    VertexType* vertexArray;
    uint8*		indexArray;

    int  	vertexCount;
    int  	indexCount;

    BaseMaterial* 	pMaterial;

    BufferObject	vertexBuffer;
    BufferObject	indexBuffer;

    bool	useVertexArray;
    bool 	useIndexArray;

    PrimitivType	type;


    public:

    Mesh():
        vertexArray(nullptr),
        indexArray(nullptr),
        vertexCount(0),
        indexCount(0),
        pMaterial(NULL),
        vertexBuffer(),
        indexBuffer(),
        useVertexArray(false),
        useIndexArray(false),
        type( PrimitivType::Triangles )
    {}

    void draw()
    {
        if( this == NULL )
            return;

        if( !vertexBuffer.isInit() )
            return;

        if( pMaterial != NULL )
            pMaterial->enable();


         glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getID() );

        if( indexBuffer.isInit() )
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , indexBuffer.getID() );

            VertexType::enable();
            VertexType::setPointers();

            glDrawElements( (GLenum)type , indexCount , GL_UNSIGNED_BYTE, 0);

            VertexType::disable();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , 0 );
        }
        else
        {
            VertexType::enable();
            VertexType::setPointers();

            glDrawArrays( (GLenum)type , 0 , (GLsizei)vertexCount );

            VertexType::disable();
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void setType( PrimitivType t )
    {
        type = t;
    }

    void setMaterial( BaseMaterial* pMat )
    {
        pMaterial = pMat;
    }

    //--------------------------------//
    //----------VertexArray-----------//
    //--------------------------------//

    void createVertexArray( int size )
    {
        clearVertexArray();

        vertexCount = size;
        vertexArray = new VertexType[ vertexCount ];
        useVertexArray = true;
    }

    void setVertexArray( AbstractVertex* pVertices , int size )
    {
        clearVertexArray();

        vertexCount = size;
        vertexArray = (VertexType*)pVertices;
        useVertexArray = true;
    }

    void clearVertexArray()
    {
        if( useVertexArray )
        {
            delete[] vertexArray;
            vertexArray = nullptr;
            useVertexArray = false;
        }
    }

    //--------------------------------//
    //----------IndexArray------------//
    //--------------------------------//


    void createIndexArray( int size )
    {
        clearIndexArray();

        indexCount = size;
        indexArray = new uint8[ indexCount ];
        useIndexArray = true;
    }

    void setIndexArray( uint8* pIndices , int size )
    {
        clearIndexArray();

        indexCount = size;
        indexArray = pIndices;
        useIndexArray = true;
    }

    void clearIndexArray()
    {
        if( useIndexArray )
        {
            delete[] indexArray;
            indexArray = nullptr;
            useIndexArray = false;
        }
    }



    //--------------------------------//
    //---------VertexBuffer-----------//
    //--------------------------------//

    void createVertexBuffer( GLenum target = GL_STATIC_DRAW )
    {
        if( useVertexArray )
            vertexBuffer.create<VertexType>( vertexArray , vertexCount , target);
    }

    void clearVertexBuffer()
    {
        vertexBuffer.clear();
    }

    void createIndexBuffer( GLenum target = GL_STATIC_DRAW )
    {
        indexBuffer.create<uint8>( indexArray , indexCount , target);

    }

    void clearIndexBuffer()
    {
        indexBuffer.clear();
    }

    void clear()
    {
        clearVertexArray();
        clearIndexArray();
        clearVertexBuffer();
        clearIndexBuffer();
        vertexCount = 0;
        indexCount = 0;
    }

    void createQuad( Vector3D v , Vector3D u ,
                     VertexType defaultVertex )
    {
        clear();

        type = PrimitivType::Quads;

        createVertexArray(4);

        Vector3D pos = defaultVertex.position - 0.5f*v - 0.5f*u;
        Vector2D texCoord = Vector2D( 0 , 0 );
        defaultVertex.setPosition( pos );
        defaultVertex.setTexCoord( texCoord );
        vertexArray[0] = defaultVertex;

        pos += u;
        texCoord.x = 1.f;
        defaultVertex.setPosition( pos );
        defaultVertex.setTexCoord( texCoord );
        vertexArray[1] = defaultVertex;

        pos += v;
        texCoord.y = 1.f;
        defaultVertex.setPosition( pos );
        defaultVertex.setTexCoord( texCoord );
        vertexArray[2] = defaultVertex;

        pos -= u;
        texCoord.x = 0.f;
        defaultVertex.setPosition( pos );
        defaultVertex.setTexCoord( texCoord );
        vertexArray[3] = defaultVertex;

        createVertexBuffer();
    }

    void createCube( Vector3D size , VertexType defaultVertex )
    {
        clear();

        type = Limp::PrimitivType::Quads;
        createVertexArray(8);

        float dx = defaultVertex.position.x;
        float dy = defaultVertex.position.y;
        float dz = defaultVertex.position.z;

        int i = 0;
        for( int j = 0 ; j < 2 ; ++j  )
            for( int k = 0 ; k < 2 ; ++k  )
                for( int l = 0 ; l < 2 ; ++l  )
                {
                    float x = dx -size.x/2.f + j*size.x;
                    float y = dy -size.y/2.f + k*size.y;
                    float z = dz -size.z/2.f + l*size.z;

                    defaultVertex.position = Vector3D( x,y,z);
                    defaultVertex.normal = defaultVertex.position;
                    defaultVertex.normal.normalize();

                    vertexArray[i++] = defaultVertex;
                }


        createIndexArray(4 * 6);
        int indices[] = {0,2,3,1, 4,6,7,5, 0,4,5,1, 2,6,7,3, 0,2,6,4, 1,5,7,3 };

        for( int i = 0 ; i < 4*6  ; ++i )
            indexArray[i] = indices[i];

        createVertexBuffer();
        createIndexBuffer();
    }

    ~Mesh()
    {
        clearVertexArray();
        clearIndexArray();
    }

};


BaseMesh* BaseMesh::loadFromFile( Path path )
{
    BaseMesh* pMesh = NULL;

    return pMesh;
}

}

#endif //LIMP_MESH_HPP

