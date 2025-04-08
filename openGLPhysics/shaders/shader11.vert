#version 460 core

layout(location = 0) in vec3 aPos;  
layout(location = 1) in vec3 aCol;  
layout(location = 2) in vec3 aNorm;  

uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;

uniform vec3 uCameraPos; 
uniform mat3 uNormalMatrix;

out vec3 fragColor;  

layout(std140) uniform LightData 
{
    vec3 lightDir;      
    vec3 lightColor;   
    vec3 lightAmbient;  
    vec3 lightDiffuse;  
    vec3 lightSpecular; 
    float ambIntensity;
    float diffIntensity;
    float specIntensity;
};

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

    // Debugging purpose: simply output the camera position as color
    fragColor = vec3(abs(uCameraPos.x), abs(uCameraPos.y), abs(uCameraPos.z));
}