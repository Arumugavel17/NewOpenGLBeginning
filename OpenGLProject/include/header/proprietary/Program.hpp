#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Program{
public:
    Program();
    ~Program();
    
    void parse_shader(int shader_type,const std::string &path);
    void set_uniform_1i(const std::string& unifrom_name, int value) const ;
    void set_uniform_1f(const std::string& unifrom_name, float value) const;
    void set_uniform_3fv(const std::string& name,const glm::vec3& vector) const;
    void set_uniform_4fv(const std::string& name,const glm::vec4& vecto) const;
    void set_uniform_mat_3fv(const std::string& uniform_name,const glm::mat3& mat3) const;
    void set_uniform_mat_4fv(const std::string& uniform_name,const glm::mat4& mat4) const;

    void add_texture(const std::string& path, int file_format, int data_format, int channel, GLuint& textureID);
    unsigned int get_id() const ;
    void link();
    void use();
    void stop_using();
private:
    int shader_count = 0;
    unsigned int ID;
    unsigned int attached_shaders[3] = {
        0,0,0
    };
};