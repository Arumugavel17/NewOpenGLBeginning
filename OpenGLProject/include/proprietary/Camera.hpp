#pragma once

//cpp standard libraries
#include <iostream>
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



class Camera{
public:
    Camera(int screenWidth, int screenHeight);
    glm::mat4 camera_input(GLFWwindow* window);
    glm::mat4 get_projection();
    glm::mat4 get_view();

    void process_mouse_input_impl(GLFWwindow* window, double xpos, double ypos);
    void process_mouse_scroll_impl(double xpos, double ypos);
    void process_key_input_impl(GLFWwindow* window, int key, int scancode, int action, int mods);
    void process_mouse_button_impl(GLFWwindow* window, int button, int action, int mods);

    static void process_mouse_scroll(GLFWwindow* window, double x_offset, double y_offset);
    static void process_mouse_input(GLFWwindow* window, double xpos, double ypos);
    static void process_key_input(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void process_mouse_button_input(GLFWwindow* window, int button, int action, int mods);

    void set_mouse_x_y(double x, double y);
    glm::vec3 get_camera_pos();
    glm::vec3 get_camera_front();

    bool mouse_can = false;
private:
    float lasttime = 0;
    float currenttime = 0;
    float zoom = 45.0f;
    int screenWidth,screenHeight;
    double lastX = 0,lastY = 0,yaw=-90,pitch;
    
    glm::vec3 nu = glm::vec3(0.0f);
    glm::vec3 cameraPos = glm::vec3(0.029147f, 2.74325f, 3.53213f);
    glm::vec3 cameraFront = glm::vec3(-0.0271985f, - 0.626604f, -0.778863f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
};