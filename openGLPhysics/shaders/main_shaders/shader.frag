#version 460 core

in vec3 gNormal;
in vec3 gFragPos;
in vec3 gVertexColor;

out vec4 fragColor;

uniform mat3 uNormalMatrix;
uniform vec3 uCameraPos;

struct DirectionalLight 
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

struct PointLight
{
    vec3 position;

    vec3 color;

    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

    float diffIntensity;

    float ambIntensity;

    float specIntensity;

    float constant;

    float linear;

    float quadratic;
};

layout(binding = 0) uniform DirectionalLightData 
{
    DirectionalLight dirLight;
};

layout(binding = 1) uniform PointLightData 
{
    PointLight pointLight;
};

vec3 calculate_point_light(vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * pointLight.diffuse * pointLight.color * pointLight.diffIntensity;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * pointLight.specular * pointLight.color * pointLight.specIntensity;

    vec3 ambient = pointLight.ambient * pointLight.color * pointLight.ambIntensity;

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calculate_directional_light(vec3 normal, vec3 viewDir, vec3 surfaceColor) 
{
    vec3 lightDir = normalize(dirLight.direction);
    vec3 ambient = dirLight.ambIntensity * dirLight.ambient * dirLight.color;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = dirLight.diffIntensity * diff * dirLight.diffuse * dirLight.color;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = dirLight.specIntensity * dirLight.specular * spec * dirLight.color;

    return (ambient + diffuse + specular) * surfaceColor;
}

void main() 
{
    vec3 norm = normalize(uNormalMatrix * gNormal);
    vec3 viewDir = normalize(uCameraPos - gFragPos);

    vec3 colorFromDir = calculate_directional_light(norm, viewDir, gVertexColor);
    vec3 colorFromPoint = calculate_point_light(norm, gFragPos, viewDir);

    vec3 finalColor = colorFromDir + colorFromPoint;

    fragColor = vec4(finalColor, 1.0);
}
