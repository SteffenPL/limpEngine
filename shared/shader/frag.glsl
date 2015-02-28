#version 130

//uniform sampler2D map_d;

smooth in vec4 colorOut;
//smooth in vec2 texCoordOut;

out vec4 fragColor;

void main()
{
    fragColor = colorOut;
}
