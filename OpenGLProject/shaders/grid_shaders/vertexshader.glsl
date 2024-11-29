// Vertex Shader (triangle_shader.vert)
#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragColor;  // Output color to the fragment shader

void main() {
    // Directly specify triangle vertex data inside the shader (no VBO/EBO)
    vec3 vertices[3];  // Array of 3 vertices for the triangle
    vertices[0] = vec3(-0.5f, -0.5f, 0.0f); // First vertex
    vertices[1] = vec3( 0.5f, -0.5f, 0.0f); // Second vertex
    vertices[2] = vec3( 0.0f,  0.5f, 0.0f); // Third vertex

    // Use gl_VertexID to select the current vertex from the array
    gl_Position = vec4(vertices[gl_VertexID], 1.0);

    // Set color for the fragment shader
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Red color
}
