#version 460 core

layout(location = 0) in vec3 aPos;  
layout(location = 1) in vec3 aCol;  
layout(location = 2) in vec3 aNorm;  

uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;

//uniform vec3 uCameraPos; 
//uniform mat3 uNormalMatrix;

out vec3 fragColor;  

layout(binding = 0) uniform LightData
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

vec3 calculate_directional_light(vec3 norm, vec3 fragColor) 
{
    // Normalize light direction
    vec3 lightDirNormalized = normalize(lightDir);
    vec3 normNormalized = normalize(norm);  // Use transformed normal here

    // Ambient lighting (scaled by ambient intensity)

    vec3 ambient = ambIntensity * lightAmbient * lightColor;

    return ambient * fragColor;

//    // Diffuse lighting (Lambertian, scaled by diffuse intensity)
//    float diff = max(dot(normNormalized, lightDirNormalized), 0.0);
//    vec3 diffuse = diffIntensity * diff * lightDiffuse * lightColor;
//
//    // Specular lighting
//    vec3 viewDir = normalize(uCameraPos - aPos);  // Use camera position
//    vec3 reflectDir = reflect(-lightDirNormalized, normNormalized);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
//    vec3 specular = specIntensity * lightSpecular * spec * lightColor;
//
//    // Combine ambient, diffuse, and specular lighting
//    vec3 finalColor = (ambient + diffuse + specular) * fragColor;
//    return finalColor;
}

void main()
{
    mat4 MVP = uProjection * uView * uModel;
    gl_Position = MVP * vec4(aPos, 1.0); 

    // Transform the normal using the normal matrix
    //vec3 transformedNormal = uNormalMatrix * aNorm;

    // Calculate the final color with transformed normal
    vec3 finalColor = calculate_directional_light(/*transformedNormal*/ vec3(0, 0, 0), aCol);

    fragColor = finalColor; 
}
