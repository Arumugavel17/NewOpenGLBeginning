
//cpp standard libraries
#include <iostream>
#include <string>
#include <array>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
#include <Skybox.hpp>

int main() {

    std::array<float, 32> vertices_ = {
        // Positions            // Texture Coords   // Normals
        -1.0f,  1.0f,  0.0f,    0.0f, 1.0f,         // Top Left
        -1.0f, -1.0f,  0.0f,    0.0f, 0.0f,         // Bottom Left
         1.0f, -1.0f,  0.0f,    1.0f, 0.0f,         // Bottom Right
         1.0f,  1.0f,  0.0f,    1.0f, 1.0f          // Top Right
    };

    std::array<float, 32> vertices_floor = {
        // Positions            // Normals          // Texture Coords   
        -1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,         // Top Left
        -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,         // Bottom Left
         1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,         // Bottom Right
         1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f          // Top Right
    };

    std::array<unsigned int, 36> indices = {
        // Front face
        0, 1, 2,
        0, 2, 3
    };

    double x = 0, y = 0;

    glm::vec4 outline = glm::vec4(1.0f);
    glm::vec4 no_outline = glm::vec4(0.0f);

    glm::mat4 model_coord = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model_coord = glm::translate(model_coord, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 projection_coord;
    glm::mat4 view_coord;

    float gridSize = 50.0f;
    float gridCellSize = 0.1f;
    bool draw = false;

    std::vector<unsigned int> EnableList = {
        GL_DEPTH_TEST,
        GL_BLEND,
        GL_MULTISAMPLE
    };
    std::string floorTexture = "assets/textures/wood.png";

    std::string frame_buffer_vertex_shader_source = "shaders/frame_buffer_shaders/vertexshader.glsl";
    std::string frame_buffer_fragment_shader_source = "shaders/frame_buffer_shaders/fragmentshader.glsl";

    std::string skybox_vertex_shader_source = "shaders/Skybox_shaders/vertexshader.glsl";
    std::string skybox_fragment_shader_source = "shaders/Skybox_shaders/fragmentshader.glsl";

    std::string grid_vertex_shader_source = "shaders/grid_shaders/vertexshader.glsl";
    std::string grid_fragment_shader_source = "shaders/grid_shaders/fragmentshader.glsl";

    std::string floor_vertex_shader_source = "shaders/floor_shaders/vertexshader.glsl";
    std::string floor_fragment_shader_source = "shaders/floor_shaders/fragmentshader.glsl";

    Application applicationInstance(1.0, 1.0, 1.0, 1.0);
    Camera cameraInstance(applicationInstance.get_screen_width(), applicationInstance.get_screen_height());
    applicationInstance.set_user(&cameraInstance);

    applicationInstance.set_mouse_callback(Camera::process_mouse_input);
    applicationInstance.set_scroll_callback(Camera::process_mouse_scroll);

    applicationInstance.get_cursor_position(&x, &y);
    cameraInstance.process_mouse_input_impl(applicationInstance.get_window(), x, y);
    
    applicationInstance.enable(EnableList);
    applicationInstance.blend_function(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    applicationInstance.enable_imgui();

    Skybox skyBox;

    Program gridProgram;
    gridProgram.setup(grid_vertex_shader_source, grid_fragment_shader_source);
    
    Program fragmentProgram;
    fragmentProgram.setup(frame_buffer_vertex_shader_source, frame_buffer_fragment_shader_source);

    Program floorProgram;
    floorProgram.setup(floor_vertex_shader_source, floor_fragment_shader_source);
    floorProgram.add_texture(floorTexture, GL_RGB, GL_RGB, GL_TEXTURE0);
    floorProgram.use();
    floorProgram.set_uniform_1i("floorTexture", 0);
    floorProgram.stop_using();
    
    Model windowModel(vertices_, indices, true);
    windowModel.setup(0, 3, GL_FALSE, 5, (void*)0);
    windowModel.setup(1, 2, GL_FALSE, 5, (void*)(3 * sizeof(float)));

    Model floorModel(vertices_floor, indices, true);
    floorModel.setup(0, 3, GL_FALSE, 8, (void*)0);
    floorModel.setup(1, 3, GL_FALSE, 8, (void*)(3 * sizeof(float)));
    floorModel.setup(2, 2, GL_FALSE, 8, (void*)(6 * sizeof(float)));


    FrameBuffer frameBuffer(applicationInstance.get_screen_width(), applicationInstance.get_screen_height());

    while (applicationInstance.main_loop()){

        frameBuffer.bind();
        applicationInstance.clear(0.2f, 0.2f, 0.2f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        projection_coord = cameraInstance.get_projection();
        view_coord = cameraInstance.process_key_input(applicationInstance.get_window());
        //skyBox.draw_skybox(projection_coord, glm::mat4(glm::mat3(view_coord)));

        floorProgram.use();
        floorProgram.set_uniform_mat_4fv("projection", projection_coord);
        floorProgram.set_uniform_mat_4fv("view", view_coord);
        floorProgram.set_uniform_3fv("viewPos", cameraInstance.get_camera_pos());
        floorProgram.set_uniform_mat_4fv("model", model_coord);
        floorModel.draw_elements(6, 1, "floor");
        floorProgram.stop_using();
        
        gridProgram.use();
        gridProgram.set_uniform_3fv("gCameraWorldPos", cameraInstance.get_camera_pos());
        gridProgram.set_uniform_mat_4fv("projection", projection_coord);
        gridProgram.set_uniform_mat_4fv("view", view_coord);
        gridProgram.set_uniform_3fv("cameraPos", cameraInstance.get_camera_pos());
        applicationInstance.creat_grid(gridProgram);
        gridProgram.stop_using();

        frameBuffer.un_bind();

        fragmentProgram.use();
        fragmentProgram.add_texture(GL_TEXTURE0, frameBuffer.get_tex_color_buffer(), true);
        windowModel.draw_elements(6, 1,"frame window");
    }
    return 1;
}