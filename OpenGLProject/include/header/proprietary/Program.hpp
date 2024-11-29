#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Program{
public:
    Program();
    ~Program();
    
    void set_uniform_1i(const std::string& unifromName, int value) const ;
    void set_uniform_1f(const std::string& unifromName, float value) const;
    void set_uniform_3fv(const std::string& name,const glm::vec3& vector) const;
    void set_uniform_4fv(const std::string& name,const glm::vec4& vecto) const;
    void set_uniform_mat_3fv(const std::string& uniformName,const glm::mat3& mat3) const;
    void set_uniform_mat_4fv(const std::string& uniformName,const glm::mat4& mat4) const;

    void add_texture(const std::string& path, int fileFormat, int dataFormat,unsigned int channel);
    void add_texture(unsigned int channel, unsigned int textureID,bool overrideTexture = false);

    unsigned int get_id() const ;
    
    void setup(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void parse_shader(int shaderType, const std::string& path);
    void link();
    void use();
    void stop_using();
private:

    glm::vec4 m_outline_color = glm::vec4(1.0f);
    float m_outline_width = 0.08f;
    int shaderCount = 0;
    unsigned int ID;
    unsigned int attachedShaders[3] = {
        0,0,0
    };
    std::unordered_map<unsigned int,unsigned int> textures;
};