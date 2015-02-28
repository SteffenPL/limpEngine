#version 130

//uniform sampler2D texture;

#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 color;
//layout(location = 2) in vec2 texCoord;

smooth out vec4 colorOut;
//smooth out vec2 texCoordOut;

uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat4 matNormal;

struct Light_point
{
    vec4 position;

    float linear;
    float range;
    float square;

    vec4 diffuse;
    vec4 specular;
};

struct Material_colored
{
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 alpha;
    vec4 shininess;
};


uniform Material_colored mat;
uniform Light_point      light[8];

uniform int lightOn;

void main()
{

    vec4 pos = matModelView * position;
    gl_Position = matProjection * pos;

    if( lightOn == 1 )
    {
        vec4 lightPos = matModelView * light[0].position;
        vec4 normCamSpace = normalize( matNormal * normal);
        vec4 dirToLight = normalize( lightPos - pos );
        float cosAngIncidence = dot(normCamSpace, dirToLight);
              cosAngIncidence = clamp(cosAngIncidence, 0, 1);

        colorOut = 1 * dirToLight;

    }
    else
    {

        colorOut = color;
    }



}
