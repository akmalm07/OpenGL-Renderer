#version 460 core

layout (location = 0) in vec3 fragColor; // Input from previous shader stage (usually vertex shader)

layout (location = 0) out vec4 outColor; // Output color to the framebuffer

void main()
{
    outColor = vec4(fragColor, 1.0); // Set the final color, assuming fragColor is a vec3
}