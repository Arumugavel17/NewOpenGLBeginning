#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aModel;

out vec2 TexCoords;
out vec3 Normal;
out vec4 Position;

//uniform mat4 aModel;
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
    Normal = mat3(transpose(inverse(aModel))) * aNormal;
    Position = aModel * vec4(aPos + (aNormal * outline),1.0);
    /*vec3 temp = aPos;
    temp += (vec3(1.0f,0.0f,0.0f) * gl_InstanceID);*/
    gl_Position = projection * view * aModel * vec4(aPos + (aNormal * outline),1.0);
}