#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


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

    Application applicationInstance;
    Camera cameraInstance(applicationInstance.getMode()->width, applicationInstance.getMode()->height);
    applicationInstance.set_user(&cameraInstance);

    applicationInstance.set_mouse_callback(Camera::process_mouse_input);
    applicationInstance.set_scroll_callback(Camera::process_mouse_scroll);

    applicationInstance.get_cursor_position(&x, &y);
    cameraInstance.process_mouse_input_impl(x, y);
    
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); 
    ImGui_ImplGlfw_InitForOpenGL(applicationInstance.get_window(),true);
    ImGui_ImplOpenGL3_Init("#version 330");

    int i = 0;

    while (applicationInstance.main_loop()) {

        frameBuffer.bind();
        // Clear color, depth, and stencil buffers
        applicationInstance.clear(0.2f, 0.2f, 0.2f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Get projection and view matrices from the camera
        projection_coord = cameraInstance.get_projection(applicationInstance.getMode());
        view_coord = cameraInstance.process_key_input(applicationInstance.get_window());
        skyBox.draw_skybox(projection_coord, glm::mat4(glm::mat3(view_coord)));

        gridProgram.use();
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
        applicationInstance.clear(0.2f, 0.2f, 0.2f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        float cellsize = 0.5f;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Scene");
        {
            ImGui::BeginChild("GameRender");

            float width = ImGui::GetContentRegionAvail().x;
            float height = ImGui::GetContentRegionAvail().y;

            ImGui::Image(
                frameBuffer.get_tex_color_buffer(),
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );

            bool isWindowHovered = ImGui::IsWindowHovered();

            // If the mouse is clicked inside the window, hide the cursor
            if (isWindowHovered && ImGui::IsMouseClicked(0)) {
                glfwSetInputMode(applicationInstance.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                std::cout << "hide" << i << "\n";
                i++;
            }
            // If the mouse is clicked outside the window, show the cursor
            else if (!isWindowHovered && ImGui::IsMouseClicked(0)) {
                glfwSetInputMode(applicationInstance.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                std::cout << "show" << i << "\n";
                i++;
            }

        }
        ImGui::EndChild();
        ImGui::End();
         
        ImGui::Begin("Value slider");
        ImGui::Text("Grid Cell size");
        if (ImGui::SliderFloat("Slider 1", &cellsize, 0.0f, 0.3f)) {
            gridProgram.set_uniform_1f("gridCellSize",cellsize);
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 1;
}