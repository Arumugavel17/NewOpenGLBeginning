#pragma once
#include <iostream>
#include <vector>
#include <Program.hpp>

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoordinates;
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path; 
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh( std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void draw(const Program &shader);

    private:
        // render data
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};