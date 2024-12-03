#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec4 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float outline = 0.0f;

out VS_OUT{
	vec2 texCoords;
}gs_out;


void main()
{
    gs_out.texCoords = aTexCoords;
    TexCoords = aTexCoords;    
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = model * vec4(aPos + (aNormal * outline),1.0);
    gl_Position = projection * view * model * vec4(aPos + (aNormal * outline),1.0);
}