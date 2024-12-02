#pragma once
#include <iostream>
#include <array>
#include <glad/glad.h>
#include <Mesh.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Program.hpp>
#include <glm/glm.hpp>


class Model {
public:
    /*
        template function (in this case, the constructor Model::Model(const std::array<float, size>&)) needs to be 
        visible to every translation unit where it is instantiated.
        
        Templates must be defined in the same translation unit or in a header file because they are instantiated at compile-time.

        You need to move the definition of your template constructor to the header file (Model.hpp) instead of the .cpp file. 
        Templates can't be defined solely in .cpp files because they require the definition to be available wherever they are 
        instantiated.
    */ 
    // Template constructor should be defined in the header
    template<std::size_t size>
    Model(const std::array<float, size>& vertices,bool shouldUseVAO) : VAO(0)  {
        if(shouldUseVAO){
            use_VAO();
        }

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        
    }
    
    template<std::size_t vertex_size,std::size_t element_size>
    Model(const std::array<float, vertex_size>& vertices,const std::array<unsigned int, element_size>& elements,bool shouldUseVAO) : VAO(0)   {
        if(shouldUseVAO){
            use_VAO();
        }
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * elements.size(), elements.data(), GL_STATIC_DRAW);
    }

    // Non-template version
    // keepint the constructors in one location
    Model(const float* vertices, size_t size,bool shouldUseVAO) : VAO(0), EBO(0), VBO(0) {
        if(shouldUseVAO){
            use_VAO();
        }
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    Model(const std::string& path) {
        load_model(path);
    }
    
    void load_model(const std::string& path);
    void process_node(const aiNode* node, const aiScene* scene);
    Mesh process_mesh(const aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string typeName);
    void draw_model(const Program &program,std::string str,bool draw_outline = false);
    
    void use_VAO();
    void clear_VAO(std::string str);
    unsigned int texture_from_file(const char* path, const std::string& directory);

    void setup( int location, int vertex_attrib_size, bool normalized, int size, void* stride);
    void draw_triangles(int start_index, int num_vertex, std::string str);
    void draw_elements(int count, int offset,std::string str);
    
    ~Model();

private:

    std::vector<Mesh> meshes;
    std::vector<Texture> texturesLoaded;
    std::string directory = "";
    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;
};
