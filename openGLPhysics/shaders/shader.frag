#version 460 core

in vec3 fragColor;  // Declare fragColor as an input from the vertex shader
out vec4 outColor;  // The final color that will be output to the framebuffer

void main() {
    outColor = vec4(fragColor, 1.0);  // Use the color from the vertex shader to set the output color
}