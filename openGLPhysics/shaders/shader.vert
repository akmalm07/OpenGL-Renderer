#version 460 core

layout(location = 0) in vec3 aPos;  
layout(location = 1) in vec3 aCol;  
layout(location = 2) in vec3 aNorm;  

uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView;

uniform vec3 uCameraPos; 

out vec3 fragColor;  

layout(std140) uniform LightData 
{
    vec3 lightDir;      
    vec3 lightColor;   
    vec3 lightAmbient;  
    vec3 lightDiffuse;  
    vec3 lightSpecular; 
};

vec3 calculate_directional_light(vec3 norm, vec3 fragColor) {
    // Normalize the normal and light direction
    vec3 lightDirNormalized = normalize(lightDir);
    vec3 normNormalized = normalize(norm);

    // Ambient light calculation
    vec3 ambient = 0.5 * lightAmbient; // Weak ambient light

    // Diffuse light calculation (Lambertian reflectance model)
    float diff = max(dot(normNormalized, lightDirNormalized), 0.0);
    vec3 diffuse = diff * lightDiffuse; // Diffuse contribution

    // Specular lighting calculation
    vec3 viewDir = normalize(uCameraPos - aPos);  // Correct view direction using camera position
    vec3 reflectDir = reflect(-lightDirNormalized, normNormalized); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); 
    vec3 specular = lightSpecular * spec;

    // Final lighting result
    vec3 finalColor = (ambient + diffuse + specular) * fragColor;
    return finalColor;
}

void main()
{
    mat4 MVP = uProjection * uView * uModel;
    gl_Position = MVP * vec4(aPos, 1.0); 

    vec3 finalColor = calculate_directional_light(aNorm, aCol);

    // Set fragment color (final lighting result)
    fragColor = finalColor; 
}
