#pragma once

#include "config.h"

namespace tools
{
    struct PointLightBundle 
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 color = glm::vec3(1.0f);

        glm::vec3 ambient = glm::vec3(0.05f);
        glm::vec3 diffuse = glm::vec3(0.8f);
        glm::vec3 specular = glm::vec3(1.0f);

        float ambInstensity = 1.0f;
        float diffInstensity = 1.0f;
        float specInstensity = 1.0f;

        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
    };
}
