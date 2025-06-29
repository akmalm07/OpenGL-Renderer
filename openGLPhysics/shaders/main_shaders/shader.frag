#version 460 core


in vec3 gNormal;
in vec3 gFragPos;
in vec3 gVertexColor;

out vec4 fragColor;

uniform mat3 uNormalMatrix;
uniform vec3 uCameraPos;


struct DirLight
{
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;


    float ambIntensity;
    float diffIntensity;
    float specIntensity;
};

layout(binding = 0) uniform DirectionalLightData 
{
    DirLight dirLight;
};


vec3 calculate_directional_light(vec3 norm, vec3 fragColor) 
{
    vec3 lightDirNormalized = normalize(dirLight.direction);


    vec3 normNormalized = normalize(norm);


    vec3 ambient = dirLight.ambIntensity * dirLight.ambient * dirLight.color;


    float diff = max(dot(normNormalized, lightDirNormalized), 0.0);
    vec3 diffuse = dirLight.diffIntensity * diff * dirLight.diffuse * dirLight.color;


    vec3 viewDir = normalize(uCameraPos - gFragPos);
    vec3 reflectDir = reflect(-lightDirNormalized, normNormalized);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = dirLight.specIntensity * dirLight.specular * spec * dirLight.color;


    vec3 finalColor = (ambient + diffuse + specular) * fragColor;
    return finalColor;
}


void main() 
{
    vec3 norm = normalize(uNormalMatrix * gNormal);

    vec3 finalColor = calculate_directional_light(norm, gVertexColor);

    fragColor = vec4(vec3(1.0), 1.0);
}