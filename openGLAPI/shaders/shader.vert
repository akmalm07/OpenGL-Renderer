#version 460 core

layout(location = 0) in vec3 Pos;  
layout(location = 1) in vec3 Col;  

out vec3 fragColor;  

layout(std140, binding = 0) uniform Matrices
{
    mat4 model;
    mat4 view;
    mat4 projection;
}camera;


void main()
{
    mat4 MVP = camera.projection * camera.view * camera.model;
    gl_Position = MVP * vec4(Pos, 1.0);  
    fragColor = Col; 
}
