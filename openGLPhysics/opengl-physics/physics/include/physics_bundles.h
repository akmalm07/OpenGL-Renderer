#pragma once

#include "config.h"

namespace physics
{
    struct MovementFactors
    {
        glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 volocity = glm::vec3(0.0f, 0.0f, 0.0f);
    };


    struct MoveibleBundle
    {
        glm::vec3 gravity = glm::vec3(0.0f, 0.0f, 0.0f);

        float mass = 1.0f;

        bool gravityAffected = true;

        MovementFactors movementFactors = { glm::vec3(0.0f), glm::vec3(0.0f) };
    };


    struct RigidBundle
    {
        glm::vec3 min = glm::vec3(0.0f);
        glm::vec3 max = glm::vec3(0.0f);
    };

}