#version 460 core


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 1) in vec2 aTexCoord;


out vec3 vPos;
out vec3 vertexColor;

out vec3 vColor;
out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

//uniform sampler2D uTexture;


void main() 
{
    gl_Position =  uProjection * uView * uModel * vec4(aPos, 1.0);
    vPos = vec3(uModel * vec4(aPos, 1.0));

    vColor = aCol;
    vTexCoord = aTexCoord;

    vertexColor = aCol;
}