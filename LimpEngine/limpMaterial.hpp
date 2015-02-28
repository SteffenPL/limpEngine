#ifndef LIMP_MATERIAL_HPP
#define LIMP_MATERIAL_HPP

#include <string>
#include <fstream>
#include <memory>

#include "limpResourceManager.hpp"
#include "limpTexture.hpp"
#include "limpColor.hpp"
#include "limpShader.hpp"
#include "limpUniform.hpp"

namespace Limp
{
using std::shared_ptr;

    struct BaseMaterial
    {
        public:


            virtual void enable(){}
            virtual void disable(){}

            virtual void setDiffuse( const Color4f& color ){}
            virtual void setAmbient( const Color4f& color ){}
            virtual void setSpecular( const Color4f& color ){}
            virtual void setAlpha( const float a ){}
            virtual void setShininess( const float e ){}

            virtual void setDiffuseMap( shared_ptr<Texture2D> pTex ){}
            virtual void setAmbientMap( shared_ptr<Texture2D> pTex ){}
            virtual void setSpecularMap( shared_ptr<Texture2D> pTex ){}
            virtual void setAlphaMap( shared_ptr<Texture2D> pTex ){}
            virtual void setBumpMap( shared_ptr<Texture2D> pTex ){}

            virtual void initUniforms( Program& p ){}

            virtual ~BaseMaterial(){}
    };
}

#endif //LIMP_MATERIAL_HPP
