#version 460 core


in vec3 gNormal;
in vec3 gFragPos;
in vec3 gVertexColor;
uniform mat3 uNormalMatrix;

out vec4 fragColor;


uniform vec3 uCameraPos;

uniform sampler2D tShadowMap;
uniform mat4 uLightSpaceMatrix;


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
 

float shadowCalculation(vec4 fragPosLightSpace) 
{

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    projCoords = projCoords * 0.5 + 0.5;
    
    projCoords = clamp(projCoords, 0.0, 1.0);
    
    
    float closestDepth = texture(tShadowMap, projCoords.xy).r;
    
    float currentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = 0.0;

    if (projCoords.z > 0.0 && projCoords.z < 1.0)
    {
        shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }
    
    float kernelSize = 3.0;
    float texelSize = 1.0 / textureSize(tShadowMap, 0).x;
    for(float x = -kernelSize / 2.0; x < kernelSize / 2.0; x += 1.0)
    {
        for(float y = -kernelSize / 2.0; y < kernelSize / 2.0; y += 1.0)
        {
            vec2 offset = vec2(x, y) * texelSize;
            float sampleDepth = texture(tShadowMap, projCoords.xy + offset).r;
            shadow += currentDepth - bias > sampleDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (kernelSize * kernelSize);
    
    
    return shadow;
}


vec3 calculate_directional_light(vec3 norm, vec3 fragColor) 
{
    vec3 lightDirNormalized = normalize(lightDir);


    vec3 normNormalized = normalize(norm);


    vec3 ambient = ambIntensity * lightAmbient * lightColor;


    float diff = max(dot(normNormalized, lightDirNormalized), 0.0);
    vec3 diffuse = diffIntensity * diff * lightDiffuse * lightColor;


    vec3 viewDir = normalize(uCameraPos - gFragPos);
    vec3 reflectDir = reflect(-lightDirNormalized, normNormalized);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specIntensity * lightSpecular * spec * lightColor;


    vec3 finalColor = (ambient + diffuse + specular) * fragColor;
    return finalColor;
}


void main() 
{
    vec3 finalColor = calculate_directional_light(gNormal, gVertexColor);

    vec4 fragPosLightSpace = uLightSpaceMatrix * vec4(gFragPos, 1.0);

    float shadow = shadowCalculation(fragPosLightSpace);
    
    finalColor = mix(finalColor, lightAmbient, shadow); 

    fragColor = vec4(finalColor, 1.0);
}