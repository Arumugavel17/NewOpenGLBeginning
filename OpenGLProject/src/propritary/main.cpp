
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

    Application applicationInstance(1.0, 1.0, 1.0, 1.0);
    Camera cameraInstance(applicationInstance.getMode()->width, applicationInstance.getMode()->height);
    applicationInstance.set_user(&cameraInstance);

    applicationInstance.set_mouse_callback(Camera::process_mouse_input);
    applicationInstance.set_scroll_callback(Camera::process_mouse_scroll);

    applicationInstance.get_cursor_position(&x, &y);
    cameraInstance.process_mouse_input_impl(applicationInstance.get_window(), x, y);
    
    std::string fragment_vertex_shader_source = "shaders/frame_buffer_shaders/vertexshader.glsl";
    std::string fragment_fragment_shader_source = "shaders/frame_buffer_shaders/fragmentshader.glsl";

    std::string window_vertex_shader_source = "shaders/window_shaders/vertexshader.glsl";
    std::string window_fragment_shader_source = "shaders/window_shaders/fragmentshader.glsl";

    std::string skybox_vertex_shader_source = "shaders/Skybox_shaders/vertexshader.glsl";
    std::string skybox_fragment_shader_source = "shaders/Skybox_shaders/fragmentshader.glsl";

    std::string grid_vertex_shader_source = "shaders/grid_shaders/vertexshader.glsl";
    std::string grid_fragment_shader_source = "shaders/grid_shaders/fragmentshader.glsl";

    Program windowProgram;
    windowProgram.setup(window_vertex_shader_source, window_fragment_shader_source);
    windowProgram.use();
    windowProgram.add_texture("assets/textures/window.png", GL_RGBA, GL_RGBA, GL_TEXTURE0);
    windowProgram.set_uniform_1i("hello", 0);

    Program fragmentProgram;
    fragmentProgram.setup(fragment_vertex_shader_source, fragment_fragment_shader_source);
    FrameBuffer frameBuffer(applicationInstance.getMode()->width, applicationInstance.getMode()->height);
    Model windowModel(vertices_, indices, true);
    windowModel.setup(0, 3, GL_FALSE, 5, (void*)0);
    windowModel.setup(1, 2, GL_FALSE, 5, (void*)(3 * sizeof(float)));

    glm::mat4 model_coord = glm::mat4(1.0f);
    glm::mat4 projection_coord;
    glm::mat4 view_coord;

    std::vector<unsigned int> EnableList = {
        GL_DEPTH_TEST,
        GL_BLEND
    };

    applicationInstance.enable(EnableList);
    applicationInstance.blend_function(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Skybox skyBox;

    glm::mat4 val = glm::mat4(1.0f);
    Program gridProgram;
    gridProgram.setup(grid_vertex_shader_source,grid_fragment_shader_source);

    float gridSize = 50.0f;
    float gridCellSize = 0.1f;
    
    unsigned int texture_color_buffer = frameBuffer.get_tex_color_buffer();
    applicationInstance.enable_imgui();
    bool draw = false;
    while (applicationInstance.main_loop()){

        //frameBuffer.bind();
        //applicationInstance.clear(0.2f, 0.2f, 0.2f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        projection_coord = cameraInstance.get_projection(applicationInstance.getMode());
        view_coord = cameraInstance.process_key_input(applicationInstance.get_window());
        //skyBox.draw_skybox(projection_coord, glm::mat4(glm::mat3(view_coord)));
        
        gridProgram.use();
        gridProgram.set_uniform_3fv("gCameraWorldPos",cameraInstance.get_camera_pos());
        //gridProgram.set_uniform_1f("gridCellSize", gridCellSize);
        //gridProgram.set_uniform_1f("gridSize", gridSize);
        gridProgram.set_uniform_mat_4fv("projection", projection_coord);
        gridProgram.set_uniform_mat_4fv("view", view_coord);
        gridProgram.set_uniform_3fv("cameraPos", cameraInstance.get_camera_pos());
        applicationInstance.creat_grid(gridProgram);
        gridProgram.stop_using();
        
        windowProgram.use();
        windowModel.use_VAO();
        windowProgram.set_uniform_mat_4fv("projection", projection_coord);
        windowProgram.set_uniform_mat_4fv("view", view_coord);
        windowProgram.set_uniform_mat_4fv("model",val);
        windowModel.draw_elements(6, 1);
        windowProgram.stop_using();
        frameBuffer.un_bind();

        /*ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Input Float Example");*/

        // Input for float value
        //ImGui::InputFloat("Float Value", &gridSize);

        // You can also use a slider to adjust the float value
        //ImGui::SliderFloat("Grid Cell Size", &gridCellSize, 0.0f, 0.5f);
        //ImGui::Text("Current value: %.3f", gridSize); // Display the value
        //ImGui::SliderFloat("Grid Size", &gridSize, 10.0f, 100.0f);
        //ImGui::Checkbox("Draw", &draw);
        //if (draw) {
        //    gridProgram.use();
        //    gridProgram.set_uniform_1i("draw", 1);
        //    gridProgram.stop_using();
        //}
        //else {
        //    gridProgram.use(); 
        //    gridProgram.set_uniform_1i("draw", 0);
        //    gridProgram.stop_using(); 
        //}
        //ImGui::End();

        //ImGui::Render();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //fragmentProgram.use();
        //windowModel.use_VAO();
        //windowProgram.add_texture(GL_TEXTURE0, frameBuffer.get_tex_color_buffer(), true);
        //windowModel.draw_elements(6, 1);
    }
    return 1;
}