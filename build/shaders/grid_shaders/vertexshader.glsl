// Vertex Shader (triangle_shader.vert)
#version 330 core

out vec3 worldPos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

const vec3 vertices[6] = {
    vec3(-1.0f, 0.0f, -1.0f ),
    vec3( 1.0f, 0.0f, -1.0f ),
    vec3( 1.0f, 0.0f,  1.0f ),
    vec3(-1.0f, 0.0f, -1.0f ),
    vec3(-1.0f, 0.0f,  1.0f ),
    vec3( 1.0f, 0.0f,  1.0f )
};

void main() {
    // Use gl_VertexID to select the current vertex from the array
    worldPos = vertices[gl_VertexID];
    worldPos.x += cameraPos.x;
    worldPos.z += cameraPos.z;
    gl_Position = projection * view * vec4(worldPos, 1.0f);
}
