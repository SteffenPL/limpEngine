#ifndef LIMP_LIGHT_HPP
#define LIMP_LIGHT_HPP

#include "limpObject.hpp"
#include "limpUniform.hpp"

namespace Limp
{

struct PointLight
{
    Vector4D position;

    float linear;
    float square;
    float range;

    Color4f diffuse;
    Color4f specular;

public:


    using UniformPointLight = UniformStruct<Vector4D,float,float,float,Color4f,Color4f> ;

    UniformPointLight initUniform( Program& p , const std::string& name )
    {
        UniformPointLight unif;
        std::vector<std::string> memberNames{"position","linear","square","range","diffuse","specular"};
        unif.init( p , memberNames , name + "." );
        return unif;
    }

    void updateUniform( UniformPointLight& unif )
    {
        unif.set( position , linear , square , range , diffuse , specular );
    }

};

struct DirectionalLight
{
    Vector3D direction;

    Color4f diffuse;
    Color4f specular;
};



}
#endif //LIMP_LIGHT_HPP
