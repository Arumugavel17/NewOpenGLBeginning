// Fragment Shader (triangle_shader.frag)
#version 330 core

in vec3 worldPos;
out vec4 FragColor; // Final output color of the fragment

uniform float gridCellSize = 0.01;
uniform vec4 gridColorThick = vec4(vec3(0.0f),1.0f);
uniform vec4 gridColorThin = vec4(vec3(0.5f),1.0f);

void main() {
    
    float Loda = mod(worldPos.z,gridCellSize);
    vec4 color =  vec4(gridColorThick.rgb,gridColorThick.a * Loda);
    FragColor = color;
}
