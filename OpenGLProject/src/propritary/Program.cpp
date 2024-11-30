#include "Program.hpp"
#include <iostream>

Program::Program(){
    ID = glCreateProgram();
}

Program::~Program(){
    glDeleteProgram(ID);
}

void Program::setup(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    parse_shader(GL_VERTEX_SHADER, vertexShaderPath);
    parse_shader(GL_FRAGMENT_SHADER, fragmentShaderPath);
    link();
}

void Program::parse_shader(GLint shader_type,const std::string &path){
    
    unsigned int shader_id;
    shader_id = glCreateShader(shader_type);
    
    std::ifstream shader_file(path);
    std::stringstream buffer;
    
    if(!shader_file.is_open())
    {
        std::cerr << "File Unable to open \n returned -1" << std::endl;
        return;
    }

    buffer << shader_file.rdbuf();
    std::cout << buffer.str() << "\n";
    std::string str = buffer.str();
    const char* c_str = str.c_str();
    
    /*
    you can join above two lines and write 
    const char* c_str = buffer.str().c_str();
    However, there's a caveat: when using buffer.str().c_str(), str() returns a temporary 
    std::string object, and calling c_str() on it will give a pointer to this temporary object, 
    which might be destroyed after the statement, leading to undefined behavior. Instead, you should 
    store the result of buffer.str() in a separate variable to ensure the pointer remains valid.
    */
    
    
    glShaderSource(shader_id,1,&c_str,NULL);
    
    glCompileShader(shader_id);

    int success;
    char infoLog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
     
    if(!success)
    {
        glGetShaderInfoLog(shader_id,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::" << shader_type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(ID,shader_id);

    shader_file.close();
}

void Program::set_uniform_1i(const std::string& unifrom_name, int value) const {
    int currently_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currently_bound_program);
    if(currently_bound_program != ID){
        throw std::runtime_error("Currently bound Program ID does not match Program ID this object is responsible for !");
        return;
    }
    glUniform1i(glGetUniformLocation(ID,unifrom_name.c_str()),value);
}

void Program::set_uniform_1f(const std::string& unifrom_name, float value) const {
    int currently_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currently_bound_program);
    if(currently_bound_program != ID){
        throw std::runtime_error("Currently bound Program ID does not match Program ID this object is responsible for !");
        return;
    }
    glUniform1f(glGetUniformLocation(ID,unifrom_name.c_str()),value);
}

void Program::set_uniform_3fv(const std::string& name,const glm::vec3& vector) const {
    int currently_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currently_bound_program);
    if(currently_bound_program != ID){
        throw std::runtime_error("Currently bound Program ID does not match Program ID this object is responsible for !");
        return;
    }
    unsigned int location = glGetUniformLocation(ID,name.c_str());
    glUniform3fv(location,1,glm::value_ptr(vector));
}


void Program::set_uniform_4fv(const std::string& name,const glm::vec4& vector) const {
    int currently_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currently_bound_program);
    if(currently_bound_program != ID){
        throw std::runtime_error("Currently bound Program ID does not match Program ID this object is responsible for !");
        return;
    }
    unsigned int location = glGetUniformLocation(ID,name.c_str());
    glUniform4fv(location,1,glm::value_ptr(vector));
}

void Program::set_uniform_mat_3fv(const std::string& uniform_name,const glm::mat3& mat3) const {
    int currently_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currently_bound_program);
    if(currently_bound_program != ID){
        throw std::runtime_error("Currently bound Program ID does not match Program ID this object is responsible for !");
        return;
    }
    glUniformMatrix3fv(glGetUniformLocation(ID,uniform_name.c_str()), 1,GL_FALSE,glm::value_ptr(mat3));
}

void Program::set_uniform_mat_4fv(const std::string& uniform_name,const glm::mat4& mat4) const {
    
    int currently_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currently_bound_program);
    if(currently_bound_program != ID){
        throw std::runtime_error("Currently bound Program ID does not match Program ID this object is responsible for !");
        return;
    }
    glUniformMatrix4fv(glGetUniformLocation(ID,uniform_name.c_str()), 1,GL_FALSE,glm::value_ptr(mat4));
}

void Program::add_texture(const std::string& path, int file_format, int data_format,unsigned int channel) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrchannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrchannels, 0);
    unsigned int textureID;
    glGenTextures(1, &textureID);  // Generate a unique ID for each texture
    glActiveTexture(channel);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, file_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    textures[textureID] = channel;
    stbi_image_free(data);
}

void Program::add_texture(unsigned int channel, unsigned int textureID,bool overrideTexture) {

    if (!overrideTexture) {
        for (const auto& pair : textures) {
            std::cout << channel << " : " << textures[channel] << "\n";
            if (pair.first == channel) {
                std::cout << ("TEXTURE::CHANNEL_OCCUPIED");
            }
        }
    }

    glActiveTexture(channel);
    glBindTexture(GL_TEXTURE_2D, textureID);
}


void Program::use() const{

    for (const auto& pair : textures) {
        glActiveTexture(pair.second);
        glBindTexture(GL_TEXTURE_2D,pair.first);
    }
    glUseProgram(ID);
}

void Program::stop_using() const{

    int currently_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currently_bound_program);
    if(currently_bound_program == ID){
        glUseProgram(0);
    }else{
        std::cout << "Program trying to clear Program which it is not responsible of." << "\n";
    }

}

void Program::link() const{

    glLinkProgram(ID);
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
     
    if(!success)
    {
        glGetProgramInfoLog(ID,512,NULL,infoLog);
        std::cout << "ERROR::LINK::" << ID << "::LINK_FAILED\n" << infoLog << std::endl;
    }
}

unsigned int Program::get_id() const {
    return ID;
}