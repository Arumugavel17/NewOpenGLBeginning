
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

    std::array<float, 288> vertices_cube = {
        // Vertices                Color                Texture Coords
        // Front face
         0.5f,  0.5f,  0.5f,      0.0f, 0.0f, 1.0f,    1.0f, 1.0f,  // Top Right
         0.5f, -0.5f,  0.5f,      0.0f, 0.0f, 1.0f,    1.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f,  0.5f,      0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // Bottom Left
        -0.5f,  0.5f,  0.5f,      0.0f, 0.0f, 1.0f,    0.0f, 1.0f,  // Top Left

        // Back face
       -0.5f,  0.5f, -0.5f,      0.0f, 0.0f,-1.0f,    1.0f, 1.0f,  // Top Right
       -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,-1.0f,    1.0f, 0.0f,  // Bottom Right
        0.5f, -0.5f, -0.5f,      0.0f, 0.0f,-1.0f,    0.0f, 0.0f,  // Bottom Left
        0.5f,  0.5f, -0.5f,      0.0f, 0.0f,-1.0f,    0.0f, 1.0f,  // Top Left

         // Left face
       -0.5f,  0.5f,  0.5f,     -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  // Top Right
       -0.5f, -0.5f,  0.5f,     -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,  // Bottom Right
       -0.5f, -0.5f, -0.5f,     -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  // Bottom Left
       -0.5f,  0.5f, -0.5f,     -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,  // Top Left

        // Right face
        0.5f,  0.5f, -0.5f,      1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  // Top Right
        0.5f, -0.5f, -0.5f,      1.0f, 0.0f, 0.0f,    1.0f, 0.0f,  // Bottom Right
        0.5f, -0.5f,  0.5f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  // Bottom Left
        0.5f,  0.5f,  0.5f,      1.0f, 0.0f, 0.0f,    0.0f, 1.0f,  // Top Left

        // Top face
       -0.5f,  0.5f, -0.5f,      0.0f, 1.0f, 0.0f,    1.0f, 1.0f,  // Top Right
       -0.5f,  0.5f,  0.5f,      0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // Bottom Right
        0.5f,  0.5f,  0.5f,      0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  // Bottom Left
        0.5f,  0.5f, -0.5f,      0.0f, 1.0f, 0.0f,    0.0f, 1.0f,  // Top Left

       // Bottom face
        0.5f, -0.5f, -0.5f,      0.0f,-1.0f, 0.0f,    1.0f, 1.0f,  // Top Right
        0.5f, -0.5f,  0.5f,      0.0f,-1.0f, 0.0f,    1.0f, 0.0f,  // Bottom Right
       -0.5f, -0.5f,  0.5f,      0.0f,-1.0f, 0.0f,    0.0f, 0.0f,  // Bottom Left
       -0.5f, -0.5f, -0.5f,      0.0f,-1.0f, 0.0f,    0.0f, 1.0f   // Top Left
    };

    std::array<unsigned int, 36> indices_cube = {
        // Front face
        0, 1, 3,   // First triangle
        1, 2, 3,   // Second triangle

        // Back face
        4, 5, 7,   // First triangle
        5, 6, 7,   // Second triangle

        // Left face
        8, 9, 11,  // First triangle
        9, 10, 11, // Second triangle

        // Right face

        12, 13, 15, // First triangle
        13, 14, 15, // Second triangle

        // Top face
        16, 17, 19, // First triangle
        17, 18, 19, // Second triangle

        // Bottom face
        20, 21, 23, // First triangle
        21, 22, 23  // Second triangle
    };

    std::array<float, 32> vertices_window = {
        // Positions            // Texture Coords   // Normals
        -1.0f,  1.0f,  0.0f,    0.0f, 1.0f,         // Top Left
        -1.0f, -1.0f,  0.0f,    0.0f, 0.0f,         // Bottom Left
         1.0f, -1.0f,  0.0f,    1.0f, 0.0f,         // Bottom Right
         1.0f,  1.0f,  0.0f,    1.0f, 1.0f          // Top Right
    };

    std::array<float, 32> vertices_floor = {
        // Positions            // Normals          // Texture Coords   
        -1.0f,  0.0f, -1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,         // Top Left
        -1.0f,  0.0f,  1.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,         // Bottom Left
         1.0f,  0.0f,  1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,         // Bottom Right
         1.0f,  0.0f, -1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f          // Top Right
    };

    std::array<unsigned int, 36> indices_plane = {
        // Front face
        0, 1, 2,
        0, 2, 3
    };

    double cursor_x = 0, cursor_y = 0;
    glm::vec3 lightPos(0.0f, 1.0f, 0.0f);
    glm::vec3 scale(0.5f);
    glm::vec4 outline(1.0f);
    glm::vec4 no_outline(0.0f);

    glm::mat4 floor_model_coord = glm::mat4(1.0f);
    glm::mat4 cube_model_coord(1.0f);
    cube_model_coord = glm::scale(cube_model_coord, glm::vec3(5.0f));
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

    std::string primitive_vertex_shader_source = "shaders/primitive_shaders/vertexshader.glsl";
    std::string primitive_fragment_shader_source = "shaders/primitive_shaders/fragmentshader.glsl";

    std::string blinn_phong_vertex_shader_source = "shaders/blinn_phong_shaders/vertexshader.glsl";
    std::string blinn_phong_fragment_shader_source = "shaders/blinn_phong_shaders/fragmentshader.glsl";

    Application applicationInstance(1.0, 1.0, 1.0, 1.0, true);
    Camera cameraInstance(applicationInstance.get_screen_width(), applicationInstance.get_screen_height());
    applicationInstance.set_user(&cameraInstance);

    applicationInstance.set_mouse_callback(Camera::process_mouse_input);
    applicationInstance.set_scroll_callback(Camera::process_mouse_scroll);
    applicationInstance.set_key_call_back(Camera::process_key_input);
//    applicationInstance.set_mouse_button_callback(Camera::process_mouse_button_input);

    applicationInstance.get_cursor_position(&cursor_x, &cursor_y);
    cameraInstance.process_mouse_input_impl(applicationInstance.get_window(), cursor_x, cursor_y);

    applicationInstance.enable(EnableList);
    applicationInstance.blend_function(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    applicationInstance.enable_imgui();

    Skybox skyBox;

    Program cubeProgram;
    cubeProgram.setup(grid_vertex_shader_source, grid_fragment_shader_source);

    Program gridProgram;
    gridProgram.setup(grid_vertex_shader_source, grid_fragment_shader_source);

    Program fragmentProgram;
    fragmentProgram.setup(frame_buffer_vertex_shader_source, frame_buffer_fragment_shader_source);

    Program primitiveProgram;
    primitiveProgram.setup(primitive_vertex_shader_source, primitive_fragment_shader_source);

    Program blinnPhongProgram;
    blinnPhongProgram.setup(blinn_phong_vertex_shader_source, blinn_phong_fragment_shader_source);
    blinnPhongProgram.add_texture(floorTexture, GL_RGB, GL_RGB, GL_TEXTURE0);
    blinnPhongProgram.use();
    blinnPhongProgram.set_uniform_1i("floorTexture", 0);
    blinnPhongProgram.stop_using();

    Model windowModel(vertices_window, indices_plane, true);
    windowModel.setup(0, 3, GL_FALSE, 5, (void*)0);
    windowModel.setup(1, 2, GL_FALSE, 5, (void*)(3 * sizeof(float)));

    Model floorModel(vertices_floor, indices_plane, true);
    floorModel.setup(0, 3, GL_FALSE, 8, (void*)0);
    floorModel.setup(1, 3, GL_FALSE, 8, (void*)(3 * sizeof(float)));
    floorModel.setup(2, 2, GL_FALSE, 8, (void*)(6 * sizeof(float)));

    Model cubeModel(vertices_cube, indices_cube, true);
    cubeModel.setup(0, 3, GL_FALSE, 8, (void*)0);
    cubeModel.setup(1, 3, GL_FALSE, 8, (void*)(3 * sizeof(float)));
    cubeModel.setup(2, 2, GL_FALSE, 8, (void*)(6 * sizeof(float)));
    
    Model r_cubeModel("assets/Models/untitled.obj");

    FrameBuffer frameBuffer(applicationInstance.get_screen_width(), applicationInstance.get_screen_height());

    glm::mat4 model_coord(1.0f);
    glm::mat4 temp(1.0f);
    glm::mat4 temp_1(1.0f);
    while (applicationInstance.main_loop()) {

        frameBuffer.bind();
        applicationInstance.clear(0.2f, 0.2f, 0.2f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        projection_coord = cameraInstance.get_projection();
        view_coord = cameraInstance.camera_input(applicationInstance.get_window());
        //skyBox.draw_skybox(projection_coord, glm::mat4(glm::mat3(view_coord)));
        blinnPhongProgram.use();
        blinnPhongProgram.set_uniform_mat_4fv("projection", projection_coord);
        blinnPhongProgram.set_uniform_mat_4fv("view", view_coord);
        blinnPhongProgram.set_uniform_3fv("viewPos", cameraInstance.get_camera_pos());
        blinnPhongProgram.set_uniform_3fv("lightPos", lightPos); 
        blinnPhongProgram.set_uniform_mat_4fv("model", model_coord);
        cubeModel.draw_elements(36, 0, "cube");
        blinnPhongProgram.set_uniform_mat_4fv("model", cube_model_coord);
        cubeModel.draw_elements(36, 0,"cube");
        //shadowProgram.set_uniform_mat_4fv("model", floor_model_coord);
        //floorModel.draw_elements(6, 0, "plane");
        blinnPhongProgram.stop_using();

        primitiveProgram.use();
        primitiveProgram.set_uniform_mat_4fv("projection", projection_coord);
        primitiveProgram.set_uniform_mat_4fv("view", view_coord);
        primitiveProgram.set_uniform_mat_4fv("model", model_coord);
        primitiveProgram.set_uniform_3fv("color", glm::vec3(7.0f, 0.0f, 0.0f));
        primitiveProgram.set_uniform_mat_4fv("model", glm::translate(glm::mat4(1.0f),glm::vec3(7.0f,0.0f,0.0f)));
        cubeModel.draw_elements(36, 0, "cube");
        primitiveProgram.set_uniform_3fv("color", glm::vec3(0.0f, 7.0f, 0.0f));
        primitiveProgram.set_uniform_mat_4fv("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.0f, 0.0f)));
        cubeModel.draw_elements(36, 0, "cube");
        primitiveProgram.set_uniform_3fv("color", glm::vec3(0.0f, 0.0f, 7.0f));
        primitiveProgram.set_uniform_mat_4fv("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 7.0f)));
        cubeModel.draw_elements(36, 0, "cube");
        primitiveProgram.stop_using();

        cubeProgram.use();
        cubeProgram.set_uniform_mat_4fv("projection", projection_coord);
        cubeProgram.set_uniform_mat_4fv("view", view_coord);
        cubeProgram.set_uniform_3fv("viewPos", cameraInstance.get_camera_pos());
        cubeProgram.set_uniform_mat_4fv("model", floor_model_coord);
        floorModel.draw_elements(6, 0, "plane");
        cubeProgram.stop_using();

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
        windowModel.draw_elements(6, 0, "frame window");
        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);  // Position of the first panel
        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once); // Size of the first panel

        ImGui::Begin("Position Control");  // Unique name for the first panel

        ImGui::Text("Edit Position (glm::vec3)");
        if (ImGui::SliderFloat("X", &lightPos.x, -10.0f, 10.0f, "%.3f") ||
            ImGui::SliderFloat("Y", &lightPos.y, -10.0f, 10.0f, "%.3f") ||
            ImGui::SliderFloat("Z", &lightPos.z, -10.0f, 10.0f, "%.3f")) {
            model_coord = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), scale);
        }

        // Combine ImGui checks
        bool mousePressed = (glfwGetMouseButton(applicationInstance.get_window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        bool imguiAnyWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow); // Check if any ImGui window is hovered
        bool imguiAnyItemActive = ImGui::IsAnyItemActive(); // Check if any ImGui item is active (clicked or focused)

        // Only disable the camera when the user is not interacting with ImGui
        if (mousePressed && !imguiAnyWindowHovered && !imguiAnyItemActive) {
            cameraInstance.mouse_can = false;
            glfwSetInputMode(applicationInstance.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwGetCursorPos(applicationInstance.get_window(), &cursor_x, &cursor_y);
            cameraInstance.set_mouse_x_y(cursor_x, cursor_y);
            cameraInstance.process_mouse_input_impl(applicationInstance.get_window(), cursor_x, cursor_y);
        }

        ImGui::Text("Current Values: X=%.3f, Y=%.3f, Z=%.3f", lightPos.x, lightPos.y, lightPos.z);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(400, 50), ImGuiCond_Once); // Position of the second panel
        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once); // Size of the second panel

        ImGui::Begin("Scale Control");  // Unique name for the second panel
        ImGui::Text("Edit Scale (glm::vec3)");
        if (ImGui::SliderFloat("X", &scale.x, 0.1f, 10.0f, "%.3f") ||
            ImGui::SliderFloat("Y", &scale.y, 0.1f, 10.0f, "%.3f") ||
            ImGui::SliderFloat("Z", &scale.z, 0.1f, 10.0f, "%.3f")) {
            model_coord = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), scale);
        }
        ImGui::Text("Current Values: X=%.3f, Y=%.3f, Z=%.3f", scale.x, scale.y, scale.z);
        ImGui::End();

        // Combine ImGui checks
        mousePressed = (glfwGetMouseButton(applicationInstance.get_window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        imguiAnyWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow); // Check if any ImGui window is hovered
        imguiAnyItemActive = ImGui::IsAnyItemActive(); // Check if any ImGui item is active (clicked or focused)

        // Only disable the camera when the user is not interacting with ImGui
        if (mousePressed && !imguiAnyWindowHovered && !imguiAnyItemActive) {
            cameraInstance.mouse_can = false;
            glfwSetInputMode(applicationInstance.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwGetCursorPos(applicationInstance.get_window(), &cursor_x, &cursor_y);
            cameraInstance.set_mouse_x_y(cursor_x, cursor_y);
            cameraInstance.process_mouse_input_impl(applicationInstance.get_window(), cursor_x, cursor_y);
            std::cout << "t";
        }

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    return 1;
}