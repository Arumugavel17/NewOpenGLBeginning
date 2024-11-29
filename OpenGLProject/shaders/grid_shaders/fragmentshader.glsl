// Fragment Shader (triangle_shader.frag)
#version 330 core

in vec4 fragColor;  // Color passed from the vertex shader
out vec4 FragColor; // Final output color of the fragment

void main() {
    FragColor = fragColor;  // Output the color
}
