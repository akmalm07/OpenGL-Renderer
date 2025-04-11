#version 460 core
 

 in vec3 gNormal;
 in vec3 gFragPos;
 in vec3 gVertexColor;
 

 out vec4 fragColor;
 

 uniform vec3 uCameraPos;
 

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
 

     vec3 normNormalized = normalize(norm);
 

     // Ambient lighting (scaled by ambient intensity)
     vec3 ambient = ambIntensity * lightAmbient * lightColor;
 

     // Diffuse lighting (Lambertian, scaled by diffuse intensity)
     float diff = max(dot(normNormalized, lightDirNormalized), 0.0);
     vec3 diffuse = diffIntensity * diff * lightDiffuse * lightColor;
     


     // Specular lighting
     vec3 viewDir = normalize(uCameraPos - gFragPos);
     vec3 reflectDir = reflect(-lightDirNormalized, normNormalized);
     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
     vec3 specular = specIntensity * lightSpecular * spec * lightColor;
 

     // Combine ambient, diffuse, and specular lighting
     vec3 finalColor = (ambient + diffuse + specular) * fragColor;
     return finalColor;
 }
 

 void main() {
     vec3 finalColor = calculate_directional_light(gNormal, gVertexColor);
     fragColor = vec4(finalColor, 1.0);
 }