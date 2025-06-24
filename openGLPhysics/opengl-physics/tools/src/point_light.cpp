#include "headers.h"
#include "tools/include/point_light.h"


namespace tools
{
    PointLight::PointLight(unsigned int programID, bool debug)
    {
        clamp_light_values();
        _uniformBuffer = debug;
        _uniformBuffer.init(programID, "PointLightData", 1, true);
        update_uniform_buffer();
    }

    PointLight::PointLight(const PointLightBundle& bundle, unsigned int programID, bool debug)
        : _light(bundle)
    {
        clamp_light_values();
        _uniformBuffer = debug;
        _uniformBuffer.init(programID, "PointLightData", 1, true);
        update_uniform_buffer();
    }

    void PointLight::set_position(const glm::vec3& pos) 
    {
        _light.position = pos;
        _uniformBuffer.update_data(pos, "lightPos");
    }

    void PointLight::set_color(const glm::vec3& col)
    {
        _light.color = col;
        _uniformBuffer.update_data(col, "lightColor");
    }

    void PointLight::set_ambient(const glm::vec3& amb)
    {
        _light.ambient = amb;
        _uniformBuffer.update_data(amb, "lightAmbient");
    }

    void PointLight::set_diffuse(const glm::vec3& diff)
    {
        _light.diffuse = diff;
        _uniformBuffer.update_data(diff, "lightDiffuse");
    }

    void PointLight::set_specular(const glm::vec3& spec)
    {
        _light.specular = spec;
        _uniformBuffer.update_data(spec, "lightSpecular");
    }

    void PointLight::set_attenuation(float constant, float linear, float quadratic)
    {
        _light.constant = constant;
        _light.linear = linear;
        _light.quadratic = quadratic;
        update_uniform_buffer();
    }

    void PointLight::set_amb_intensity(float val) 
    {
        _light.ambInstensity = glm::clamp(val, 0.0f, 1.0f);
        _uniformBuffer.update_data(_light.ambInstensity, "ambIntensity");
    }

    void PointLight::set_diff_intensity(float val) 
    {
        _light.diffInstensity = glm::clamp(val, 0.0f, 1.0f);
        _uniformBuffer.update_data(_light.diffInstensity, "diffIntensity");
    }

    void PointLight::set_spec_intensity(float val) 
    {
        _light.specInstensity = glm::clamp(val, 0.0f, 1.0f);
        _uniformBuffer.update_data(_light.specInstensity, "specIntensity");
    }

    void PointLight::bind() const 
    {
        _uniformBuffer.bind();
    }

    void PointLight::unbind() const
    {
        _uniformBuffer.unbind();
    }

    void PointLight::update_uniform_buffer()
    {
        _uniformBuffer.update_data(_light.position, "lightPos");
        _uniformBuffer.update_data(_light.color, "lightColor");
        _uniformBuffer.update_data(_light.ambient, "lightAmbient");
        _uniformBuffer.update_data(_light.diffuse, "lightDiffuse");
        _uniformBuffer.update_data(_light.specular, "lightSpecular");
        _uniformBuffer.update_data(_light.ambInstensity, "ambIntensity");
        _uniformBuffer.update_data(_light.diffInstensity, "diffIntensity");
        _uniformBuffer.update_data(_light.specInstensity, "specIntensity");
        _uniformBuffer.update_data(_light.constant, "constant");
        _uniformBuffer.update_data(_light.linear, "linear");
        _uniformBuffer.update_data(_light.quadratic, "quadratic");
    }

    void PointLight::clamp_light_values() 
    {
        _light.ambInstensity = glm::clamp(_light.ambInstensity, 0.0f, 1.0f);
        _light.diffInstensity = glm::clamp(_light.diffInstensity, 0.0f, 1.0f);
        _light.specInstensity = glm::clamp(_light.specInstensity, 0.0f, 1.0f);
    }

}
