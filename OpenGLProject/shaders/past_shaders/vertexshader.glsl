#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
out vec3 Normal;
out vec3 FragPos;
out vec2 TextCoord;

void main() {

    gl_Position  = projection * view * model * vec4(position, 1.0);
    Normal = normal;
    vec4 temp = model * vec4(position,1.0f);
    FragPos = temp.xyz;
    TextCoord = texture;
}