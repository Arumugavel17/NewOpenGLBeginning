#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 viewPos;
} vs_out;

uniform vec3 viewPos;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    
    vs_out.FragPos = aPos;
    vs_out.Normal = aNormal ;
    vs_out.TexCoords = aTexCoords;
    vs_out.viewPos = viewPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}