#ifndef LIMPOPENGLRENDERER_HPP
#define LIMPOPENGLRENDERER_HPP

#include "limpOpenGL.hpp"
#include "limpShader.hpp"
#include "limpMath.hpp"
#include "limpVertex.hpp"
#include "limpResourceManager.hpp"

namespace Limp
{

using std::string;

struct OpenGLRenderer
{
public:


    bool initProgram(const std::vector<string>& vertexShaderList ,
                     const std::vector<string>& fragShaderList)
    {
        std::list<Shader*> shaderList;
        for( const string& str : vertexShaderList )
        {
            Shader* pShader = new Shader(ShaderType::VertexShader,str);
            vecShaders.push(pShader);
            shaderList.push_back(pShader);
        }

        for( const string& str : fragShaderList )
        {
            Shader* pShader = new Shader(ShaderType::FragmentShader,str);
            vecShaders.push(pShader);
            shaderList.push_back(pShader);
        }

        program.loadShaderList(shaderList);


        uniModelView.init( program , "matModelView"  , 1);
        uniNormal.init(    program , "matNormal"     , 1);
        uniProjection.init(program , "matProjection" , 1);

        return true;
    }


    Program                         program;
    ResourceManager<int,Shader>  vecShaders;

    Uniform<Matrix4x4> uniModelView;
    Uniform<Matrix4x4> uniNormal;
    Uniform<Matrix4x4> uniProjection;

};

}

#endif // LIMPOPENGLRENDERER_HPP

