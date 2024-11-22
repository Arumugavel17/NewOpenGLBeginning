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
#include <FrameBuffer.hpp>

std::array<float, 20> vertices_ = {
    // Positions         // Texture Coords
    -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, // Top Left
    -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // Bottom Left
     1.0f, -1.0f,  0.0f,  1.0f, 0.0f, // Bottom Right
     1.0f,  1.0f,  0.0f,  1.0f, 1.0f  // Top Right
};

std::array<unsigned int, 36> indices = {
    // Front face
    0, 1, 2,
    0, 2, 3
};

int main() {

    double x = 0, y = 0;

    Application applicationInstance;
    Camera cameraInstance(applicationInstance.getMode()->width, applicationInstance.getMode()->height);
    applicationInstance.setUser(&cameraInstance);

    applicationInstance.setMouseCallback(Camera::process_mouse_input);
    applicationInstance.setScrollCallback(Camera::process_mouse_scroll);

    applicationInstance.getCursorPosition(&x, &y);
    cameraInstance.process_mouse_input_impl(x, y);

    std::string window_vertex_shader_source = "shaders/window_shaders/vertexshader.glsl";
    std::string window_fragment_shader_source = "shaders/window_shaders/fragmentshader.glsl";

    std::string cube_vertex_shader_source = "shaders/model_shaders/vertexshader.glsl";
    std::string cube_fragment_shader_source = "shaders/model_shaders/fragmentshader.glsl";
    
    std::string fragment_vertex_shader_source = "shaders/fragment_shaders/vertexshader.glsl";
    std::string fragment_fragment_shader_source = "shaders/fragment_shaders/fragmentshader.glsl";

    std::string skybox_vertex_shader_source = "shaders/Skybox_shaders/vertexshader.glsl";
    std::string skybox_fragment_shader_source = "shaders/Skybox_shaders/fragmentshader.glsl";

    Program skyBox;
    skyBox.setup(skybox_vertex_shader_source, skybox_fragment_shader_source);

    Program cubeProgram;
    cubeProgram.setup(cube_vertex_shader_source, cube_fragment_shader_source);

    Model cubeModel("assets/Models/backpack/backpack.obj");
    Program windowProgram;
    windowProgram.setup(window_vertex_shader_source, window_fragment_shader_source);
    windowProgram.use();
    windowProgram.add_texture("assets/textures/window.png", GL_RGBA, GL_RGBA, GL_TEXTURE0);
    windowProgram.set_uniform_1i("hello", 0);

    Model windowModel(vertices_, indices, true);
    windowModel.setup(0, 3, GL_FALSE, 5, (void*)0);
    windowModel.setup(1, 2, GL_FALSE, 5, (void*)(3 * sizeof(float)));

    Program fragmentProgram;
    fragmentProgram.setup(fragment_vertex_shader_source, fragment_fragment_shader_source);

    glm::mat4 model_coord = glm::mat4(1.0f);
    glm::mat4 projection_coord;
    glm::mat4 view_coord;

    glm::vec4 outline = glm::vec4(1.0f);
    glm::vec4 no_outline = glm::vec4(0.0f);
    glm::mat4 scaled_model = glm::scale(model_coord, glm::vec3(1.1f));

    std::vector<glm::vec3> position;
    std::vector<glm::mat4> distance;

    for (int i = 1;i <= 5;i++) {
        position.push_back(glm::vec3(0.0f, 0.0f, i));
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FrameBuffer frameBuffer(applicationInstance.getMode()->width, applicationInstance.getMode()->height);

    while (!applicationInstance.windowShouldClose()) {
        
        frameBuffer.bind();
        // Clear color, depth, and stencil buffers
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // Get projection and view matrices from the camera
        projection_coord = cameraInstance.get_projection(applicationInstance.getMode());
        view_coord = cameraInstance.process_key_input(applicationInstance.getWindow());

        cubeProgram.use();
        cubeProgram.set_uniform_4fv("outline_color", no_outline);
        cubeProgram.set_uniform_1f("outline", 0.0f);
        cubeProgram.set_uniform_mat_4fv("projection", projection_coord); 
        cubeProgram.set_uniform_mat_4fv("view", view_coord); 
        cubeProgram.set_uniform_mat_4fv("model", model_coord); 
        cubeModel.draw_model(cubeProgram,true);
        cubeProgram.stop_using();

        windowProgram.use();
        windowModel.use_VAO();
        windowProgram.set_uniform_mat_4fv("projection", projection_coord);
        windowProgram.set_uniform_mat_4fv("view", view_coord);

        glm::vec3 pos = cameraInstance.get_camera_pos();
        std::sort(position.begin(), position.end(), [pos](const glm::vec3& a, const glm::vec3& b) {
            return glm::length(pos - a) > glm::length(pos - b);
            });

        for (int i = 0;i < 5;i++) {
            windowProgram.set_uniform_mat_4fv("model", glm::translate(glm::mat4(1.0f), position[i]));
            windowModel.draw_elements(6, 1);
        }
        windowProgram.stop_using();
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        frameBuffer.un_bind();
        
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        fragmentProgram.use();
        windowModel.use_VAO();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D,frameBuffer.get_tex_color_buffer());
        windowProgram.add_texture(GL_TEXTURE0, frameBuffer.get_tex_color_buffer(),true);
        windowModel.draw_elements(6, 1);

        glfwSwapBuffers(applicationInstance.getWindow()); 
        glfwPollEvents();
    }

    return 1;
}