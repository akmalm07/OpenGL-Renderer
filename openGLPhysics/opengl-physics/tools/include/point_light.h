#pragma once

#include "config.h"

#include "tools/include/point_light_bundle.h"
#include "glUtil/include/uniform_buffer.h"


namespace tools
{

    class PointLight 
    {
    public:
        PointLight() = default;
        PointLight(unsigned int programID, bool debug);
        PointLight(const PointLightBundle& bundle, unsigned int programID, bool debug);

        void set_position(const glm::vec3& pos);
        void set_color(const glm::vec3& col);
        void set_ambient(const glm::vec3& amb);
        void set_diffuse(const glm::vec3& diff);
        void set_specular(const glm::vec3& spec);

        void set_attenuation(float constant, float linear, float quadratic);

        void set_amb_intensity(float val);
        void set_diff_intensity(float val);
        void set_spec_intensity(float val);

        void bind() const;
        void unbind() const;

        void update_uniform_buffer();

    private:
        PointLightBundle _light;
        glUtil::UniformBuffer _uniformBuffer;

        void clamp_light_values();
    };

}
