//Vendor Libraries
#include <stb_image.h>

//cpp standard libraries
#include <iostream>
#include <string>
#include <array>

//Window libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//glm Maths Librariess
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

//Proprietory Libraries
#include <Camera.hpp>
#include <Program.hpp>
#include <Model.hpp>
#include <Application.hpp>

int main(){
    
    double x = 0,y = 0;
    
    Application ApplicationInstance;
    Camera cameraInstance(ApplicationInstance.getMode()->width,ApplicationInstance.getMode()->height);
    ApplicationInstance.setUser(&cameraInstance);
    
    ApplicationInstance.setMouseCallback(Camera::process_mouse_input);
    ApplicationInstance.setScrollCallback(Camera::process_mouse_scroll);

    ApplicationInstance.getCursorPosition(&x,&y);
    cameraInstance.process_mouse_input_impl(x,y);

    std::string vertex_shader_source = "C:/Users/arumu/source/repos/OpenGLProject/OpenGLProject/shaders/cube_shaders/vertexshader.glsl";
    std::string fragment_shader_source = "C:/Users/arumu/source/repos/OpenGLProject/OpenGLProject/shaders/cube_shaders/fragmentshader.glsl";
    Program cubeProgram;
    cubeProgram.parse_shader(GL_VERTEX_SHADER,vertex_shader_source);
    cubeProgram.parse_shader(GL_FRAGMENT_SHADER,fragment_shader_source);
    cubeProgram.link();
    cubeProgram.use();
    
    Model model("C:/Users/arumu/source/repos/OpenGLProject/OpenGLProject/assets/Models/backpack/backpack.obj");

    glm::mat4 model_coord = glm::mat4(1.0f);
    glm::mat4 projection_coord;
    glm::mat4 view_coord;

    cubeProgram.use();
    
    glm::vec4 outline = glm::vec4(1.0f);
    glm::vec4 no_outline = glm::vec4(0.0f);
    glm::mat4 scaled_model = glm::scale(model_coord, glm::vec3(1.1f));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 


    while (!ApplicationInstance.windowShouldClose()) {
        // Clear color, depth, and stencil buffers
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Get projection and view matrices from the camera
        projection_coord = cameraInstance.get_projection(ApplicationInstance.getMode());
        view_coord = cameraInstance.process_key_input(ApplicationInstance.getWindow());

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        cubeProgram.set_uniform_4fv("outline_color", no_outline);
        cubeProgram.set_uniform_1f("outline", 0.0f);
        cubeProgram.set_uniform_mat_4fv("projection", projection_coord); 
        cubeProgram.set_uniform_mat_4fv("view", view_coord); 
        cubeProgram.set_uniform_mat_4fv("model", model_coord); 
        model.draw_model(cubeProgram);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        cubeProgram.set_uniform_4fv("outline_color", outline);
        cubeProgram.set_uniform_1f("outline", 0.01f);
        model.draw_model(cubeProgram);
        glStencilMask(0xFF);
        glStencilFunc(GL_EQUAL, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(ApplicationInstance.getWindow()); 
        glfwPollEvents();
    }
    model.clear_VAO();
    return 1;
}