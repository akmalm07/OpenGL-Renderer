#version 460 core


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;


out vec3 vPos;
out vec3 vertexColor;


uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;



void main() 
{
    gl_Position = uModel * uView * uProjection * vec4(aPos, 1.0);
    vPos = vec3(uModel * vec4(aPos, 1.0));
    vertexColor = aCol;
}