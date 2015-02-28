#ifndef DEFAULT_GRAPHIC_TYPES_HPP
#define DEFAULT_GRAPHIC_TYPES_HPP

#include "limpMaterial.hpp"
#include "limpVertex.hpp"
#include "limpColor.hpp"
#include "limpMath.hpp"

namespace Limp
{

    struct Material_colored : public BaseMaterial
    {
        Color4f diffuse;
        Color4f ambient;
        Color4f specular;
        float 	alpha;
        float shininess;

        Uniform<Color4f> diffuseUniform;
        Uniform<Color4f> ambientUniform;
        Uniform<Color4f> specularUniform;
        Uniform<Vector4D> restUniform;

    public:
        Material_colored():
            diffuse(1.f,1.f,1.f,1.f),
            ambient(0.2f , 0.2f , 0.2f , 1.f),
            specular(0.2f, 0.2f , 0.2f , 1.f ),
            alpha(1.f),
            shininess(0.2f)
            {}

            void enable()
            {
                diffuseUniform = diffuse;
                ambientUniform = ambient;
                specularUniform = specular;
                restUniform = Vector4D( alpha , shininess , 0.f , 0.f );
            }

            void disable(){}

            void setDiffuse( const Color4f& color )
            {
                diffuse = color;
            }

            void setAmbient( const Color4f& color )
            {
                ambient = color;
            }

            void setSpecular( const Color4f& color )
            {
                specular = color;
            }

            void setAlpha( const float a )
            {
                alpha = a;
            }

            void setShininess( const float e )
            {
                shininess = e;
            }

            void setDiffuseMap( Texture2D* pTex ){}
            void setAmbientMap( Texture2D* pTex ){}
            void setSpecularMap( Texture2D* pTex ){}
            void setAlphaMap( Texture2D* pTex ){}
            void setBumpMap( Texture2D* pTex ){}

            void initUniforms( Program& p )
            {
                diffuseUniform.init(p,"mat.diffuse");
                ambientUniform.init(p,"mat.ambient");
                specularUniform.init(p,"mat.specular");
                restUniform.init(p,"mat.rest");
            }

            ~Material_colored(){}
    };

    struct Material_textured : public BaseMaterial
    {
        Texture2D*	pDiffuseMap;
        Color4f ambient;
        Color4f specular;
        float 	alpha;
        float shininess;

        Uniform<Texture2D> diffuseMapUniform;
        Uniform<Color4f> ambientUniform;
        Uniform<Color4f> specularUniform;
        Uniform<Vector4D> restUniform;

    public:
        Material_textured():
            ambient(0.2f , 0.2f , 0.2f , 1.f),
            specular(0.2f, 0.2f , 0.2f , 1.f ),
            alpha(1.f),
            shininess(0.2f)
            {}

            void enable()
            {
                diffuseMapUniform = pDiffuseMap;
                ambientUniform = ambient;
                specularUniform = specular;
                restUniform = Vector4D( alpha , shininess , 0.f , 0.f );
            }

            void disable(){}

            void setDiffuse( const Color4f& color )
            {}

            void setAmbient( const Color4f& color )
            {
                ambient = color;
            }

            void setSpecular( const Color4f& color )
            {
                specular = color;
            }

            void setAlpha( const float a )
            {
                alpha = a;
            }

            void setShininess( const float e )
            {
                shininess = e;
            }

            void setDiffuseMap( Texture2D* pTex )
            {
                pDiffuseMap = pTex;
            }

            void setAmbientMap( Texture2D* pTex ){}
            void setSpecularMap( Texture2D* pTex ){}
            void setAlphaMap( Texture2D* pTex ){}
            void setBumpMap( Texture2D* pTex ){}

            void initUniforms( Program& p )
            {
                diffuseMapUniform.init(p,"mat.diffuseMap");
                ambientUniform.init(p,"mat.ambient");
                specularUniform.init(p,"mat.specular");
                restUniform.init(p,"mat.rest");
            }

            ~Material_textured(){}

    };

    struct Material_bumpMap : public BaseMaterial
    {
        Texture2D*	pDiffuseMap;
        Texture2D*	pBumpMap;
        Color4f ambient;
        Color4f specular;
        float 	alpha;
        float shininess;

        Uniform<Texture2D> diffuseMapUniform;
        Uniform<Texture2D> bumpMapUniform;
        Uniform<Color4f> ambientUniform;
        Uniform<Color4f> specularUniform;
        Uniform<Vector4D> restUniform;

    public:
        Material_bumpMap():
            ambient(0.2f , 0.2f , 0.2f , 1.f),
            specular(0.2f, 0.2f , 0.2f , 1.f ),
            alpha(1.f),
            shininess(0.2f)
            {}

            void enable()
            {
                diffuseMapUniform = pDiffuseMap;
                ambientUniform = ambient;
                specularUniform = specular;
                restUniform = Vector4D( alpha , shininess , 0.f , 0.f );
            }

            void disable(){}

            void setDiffuse( const Color4f& color )
            {}

            void setAmbient( const Color4f& color )
            {
                ambient = color;
            }

            void setSpecular( const Color4f& color )
            {
                specular = color;
            }

            void setAlpha( const float a )
            {
                alpha = a;
            }

            void setShininess( const float e )
            {
                shininess = e;
            }

            void setDiffuseMap( Texture2D* pTex )
            {
                pDiffuseMap = pTex;
            }

            void setAmbientMap( Texture2D* pTex ){}
            void setSpecularMap( Texture2D* pTex ){}
            void setAlphaMap( Texture2D* pTex ){}
            void setBumpMap( Texture2D* pTex )
            {
                pBumpMap = pTex;
            }

            void initUniforms( Program& p )
            {
                diffuseMapUniform.init(p,"mat.diffuseMap");
                bumpMapUniform.init(p,"mat.bumpMap");
                ambientUniform.init(p,"mat.ambient");
                specularUniform.init(p,"mat.specular");
                restUniform.init(p,"mat.rest");
            }

            ~Material_bumpMap(){}
    };

    struct Vertex_Pos : public AbstractVertex
    {
        Vector4D position;
        Vector4D normal;

        public:
            Vertex_Pos(){}
            Vertex_Pos(Vector4D pos , Vector4D n  ):
                position(pos),
                normal(n)
            {
            }

            template< typename T >
            void setPosition( const T& v )
            {
                position = v;
            }

            template< typename T >
            void setNormal( const T& v )
            {
                normal = v;
            }


            static void setPointers()
            {
                size_t offset = 0;
                const size_t size = sizeof(Vertex_Pos);
                AbstractVertex::setAttributePointer<float,4>( 0 , size , &offset );
                AbstractVertex::setAttributePointer<float,4>( 1 , size , &offset );
            }

            static void enable()
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
            }

            static void disable()
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
            }
    };

    struct Vertex_Colored : public AbstractVertex
    {
        Vector4D position;
        Vector4D normal;
        Color4f	color;

        public:
            Vertex_Colored(){}
            Vertex_Colored(Vector4D pos , Vector4D n , Color4f c ):
                position(pos),
                normal(n),
                color(c)
            {
            }

            template< typename T >
            void setPosition( const T& v )
            {
                position = v;
            }

            template< typename T >
            void setNormal( const T& v )
            {
                normal = v;
            }

            template< typename T >
            void setColor( const T& col )
            {
                color = col;
            }

            template< typename T >
            void setTexCoord( const T& v , int index = 0 )
            {}

            static void setPointers()
            {
                size_t offset = 0;
                const size_t size = sizeof(Vertex_Colored);
                AbstractVertex::setAttributePointer<float,4>( 0 , size , &offset );
                AbstractVertex::setAttributePointer<float,4>( 1 , size , &offset );
                AbstractVertex::setAttributePointer<float,4>( 2 , size , &offset );
            }

            static void enable()
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
            }

            static void disable()
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
            }
    };

    struct Vertex_Textured : public AbstractVertex
    {
        Vector4D position;
        Vector4D normal;
        Vector2D texCoord;

        public:
            Vertex_Textured(){}
            Vertex_Textured(Vector4D pos , Vector4D n , Vector2D uv ):
                position(pos),
                normal(n),
                texCoord(uv)
            {
            }

            template< typename T >
            void setPosition( const T& v )
            {
                position = v;
            }

            template< typename T >
            void setNormal( const T& v )
            {
                normal = v;
            }

            template< typename T >
            void setColor( const T& col )
            {}

            template< typename T >
            void setTexCoord( const T& v , int index = 0 )
            {
                texCoord = v;
            }

            static void setPointers()
            {
                size_t offset = 0;
                const size_t size = sizeof(Vertex_Textured);
                AbstractVertex::setAttributePointer<float,4>( 0 , size , &offset );
                AbstractVertex::setAttributePointer<float,4>( 1 , size , &offset );
                AbstractVertex::setAttributePointer<float,2>( 2 , size , &offset );
            }

            static void enable()
            {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
            }

            static void disable()
            {
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
            }
    };


}

#endif //DEFAULT_GRAPHIC_TYPES_HPP
