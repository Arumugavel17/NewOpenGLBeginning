
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

    double x = 0, y = 0;

    glm::vec4 outline = glm::vec4(1.0f);
    glm::vec4 no_outline = glm::vec4(0.0f);

    //glm::mat4 window_model_coord = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 5.0f));
    glm::mat4 back_pack_model_coord = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 5.0f, 0.0f));
    glm::mat4 planet_model_coord = glm::translate(glm::mat4(1.0f), glm::vec3 ( 0.0f, 0.0f, 0.0f));
    glm::mat4 rock_model_coord = glm::translate(glm::mat4(1.0f), glm::vec3( 0.0f, 0.0f, 0.0f));


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

    std::string frame_buffer_vertex_shader_source = "shaders/frame_buffer_shaders/vertexshader.glsl";
    std::string frame_buffer_fragment_shader_source = "shaders/frame_buffer_shaders/fragmentshader.glsl";

    std::string window_vertex_shader_source = "shaders/window_shaders/vertexshader.glsl";
    std::string window_fragment_shader_source = "shaders/window_shaders/fragmentshader.glsl";

    std::string skybox_vertex_shader_source = "shaders/Skybox_shaders/vertexshader.glsl";
    std::string skybox_fragment_shader_source = "shaders/Skybox_shaders/fragmentshader.glsl";

    std::string grid_vertex_shader_source = "shaders/grid_shaders/vertexshader.glsl";
    std::string grid_fragment_shader_source = "shaders/grid_shaders/fragmentshader.glsl";

    std::string instanced_rendering_vertex_shader_source = "shaders/instanced_model_shaders/vertexshader.glsl";
    std::string instanced_rendering_fragment_shader_source = "shaders/instanced_model_shaders/fragmentshader.glsl";

    std::string backpack_vertex_shader_source = "shaders/model_shaders/vertexshader.glsl";
    std::string backpack_fragment_shader_source = "shaders/model_shaders/fragmentshader.glsl";
    //std::string backpack_geometry_shader_source = "shaders/model_shaders/geometryshader.glsl";

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

    Program windowProgram;
    windowProgram.setup(window_vertex_shader_source, window_fragment_shader_source);
    windowProgram.use();
    windowProgram.add_texture("assets/textures/window.png", GL_RGBA, GL_RGBA, GL_TEXTURE0);
    windowProgram.set_uniform_1i("hello", 0);

    Program fragmentProgram;
    fragmentProgram.setup(frame_buffer_vertex_shader_source, frame_buffer_fragment_shader_source);

    Program modelProgram;
    modelProgram.setup(backpack_vertex_shader_source, backpack_fragment_shader_source/*, backpack_geometry_shader_source*/);

    Program instancedProgram;
    instancedProgram.setup(instanced_rendering_vertex_shader_source, instanced_rendering_fragment_shader_source/*, backpack_geometry_shader_source*/);

    Model windowModel(vertices_, indices, true);
    windowModel.setup(0, 3, GL_FALSE, 5, (void*)0);
    windowModel.setup(1, 2, GL_FALSE, 5, (void*)(3 * sizeof(float)));

    Model backpackModel("assets/Models/backpack/backpack.obj");
    Model marsModel("assets/Models/Mars/planet.obj");
    Model rockModel("assets/Models/rock/rock.obj");

    FrameBuffer frameBuffer(applicationInstance.get_screen_width(), applicationInstance.get_screen_height());

    unsigned int amount = 100000;
    glm::mat4* modelMatrices;   
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed
    float radius = 50.0;
    float offset = 10.0f;

    for (unsigned int i = 0;i < amount;i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        //1.translation:displacealongcirclewithradius[-offset,offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;//keepheightoffieldsmallerthanx/z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        //2.scale:scalebetween0.05and0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
        //3.rotation:addrandomrotationarounda(semi)randomrotationaxis
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        //4.nowaddtolistofmatrices
        modelMatrices[i] = model;
    }

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    std::vector<Mesh>* meshes = rockModel.get_mesh();
    for (unsigned int i = 0;i < meshes->size();i++)
    {
        unsigned int VAO = meshes->at(i).get_VAO();
        glBindVertexArray(VAO);
        std::size_t v4s = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void*)(1 * v4s));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void*)(2 * v4s));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void*)(3 * v4s));
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glBindVertexArray(0);
    }

    while (applicationInstance.main_loop()){

        frameBuffer.bind();
        applicationInstance.clear(0.2f, 0.2f, 0.2f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        projection_coord = cameraInstance.get_projection();
        view_coord = cameraInstance.process_key_input(applicationInstance.get_window());
        //skyBox.draw_skybox(projection_coord, glm::mat4(glm::mat3(view_coord)));
        
        modelProgram.use();
        modelProgram.set_uniform_4fv("outline_color", no_outline);
        modelProgram.set_uniform_1f("outline", 0.0f);
        modelProgram.set_uniform_mat_4fv("projection", projection_coord);
        modelProgram.set_uniform_mat_4fv("view", view_coord);
        modelProgram.set_uniform_mat_4fv("model", planet_model_coord);
        marsModel.draw_model(modelProgram, true);
        modelProgram.stop_using();

        instancedProgram.use();
        instancedProgram.set_uniform_4fv("outline_color", no_outline);
        instancedProgram.set_uniform_1f("outline", 0.0f);
        instancedProgram.set_uniform_mat_4fv("projection", projection_coord);
        instancedProgram.set_uniform_mat_4fv("view", view_coord);
        rockModel.draw_model(instancedProgram,false,amount);
        instancedProgram.stop_using();

        //windowProgram.use();
        //windowModel.use_VAO();
        //windowProgram.set_uniform_mat_4fv("projection", projection_coord);
        //windowProgram.set_uniform_mat_4fv("view", view_coord);
        //windowProgram.set_uniform_mat_4fv("model", window_model_coord);
        //windowModel.draw_elements(6, 1, "window");
        //windowProgram.stop_using();

        gridProgram.use();
        gridProgram.set_uniform_3fv("gCameraWorldPos", cameraInstance.get_camera_pos());
        //gridProgram.set_uniform_1f("gridCellSize", gridCellSize);
        //gridProgram.set_uniform_1f("gridSize", gridSize);
        gridProgram.set_uniform_mat_4fv("projection", projection_coord);
        gridProgram.set_uniform_mat_4fv("view", view_coord);
        gridProgram.set_uniform_3fv("cameraPos", cameraInstance.get_camera_pos());
        applicationInstance.creat_grid(gridProgram);
        gridProgram.stop_using();

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

        fragmentProgram.use();
        //windowModel.use_VAO();
        fragmentProgram.add_texture(GL_TEXTURE0, frameBuffer.get_tex_color_buffer(), true);
        windowModel.draw_elements(6, 1,"frame window");
    }
    return 1;
}