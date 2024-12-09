#pragma once

//cpp standard libraries
#include <iostream>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//Window libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Program.hpp>
#include <Camera.hpp>

class Application{
public:
    Application(float bg_r = 0.2f, float bg_g = 0.2f, float bg_b = 0.2f, float bg_a = 0.2f, bool fullScreen = false, int width = 1024, int heigth = 576);
    ~Application();
    GLFWmonitor* get_monitor();
    GLFWwindow* get_window();
    const GLFWvidmode* get_mode();
    void set_user(void* pointer);
    void set_background(float r,float g, float b, float a);
    void set_scroll_callback(void (*func)(GLFWwindow* window, double xPos, double yPos));
    void set_mouse_callback(void (*func)(GLFWwindow* window, double xOffset, double yOffset));
    void set_key_call_back(void (*func)(GLFWwindow* window, int key, int scancode, int action, int mods));
    void set_mouse_button_callback(void (*func)(GLFWwindow* window, int button, int action, int mods));

    void enable_imgui();
    void get_cursor_position(double* x,double* y);
    
    bool main_loop();
    void enable(const std::vector<unsigned int>& toEnable);
    void blend_function(unsigned int sfactor, unsigned int dfactor);
    void clear(float r, float g, float b, float w, unsigned int clearBuffer);
    void creat_grid(const Program& gridProgram);
    
    int get_screen_width();
    int get_screen_height();

    bool showCursor = true;

private:
    float bg_r, bg_g, bg_b, bg_a;

    typedef void (*ScrollCallback)(GLFWwindow* window, double xpos, double ypos);
    ScrollCallback scrollCallback;

    typedef void (*CursorPosCallback)(GLFWwindow* window, double x_offset, double y_offset);
    CursorPosCallback cursorCallback;

    typedef void (*KeyCallBack)(GLFWwindow* window, int key, int scancode, int action, int mods);
    KeyCallBack keyCallback;

    typedef void (*MouseButtonCallback)(GLFWwindow* window, int button, int action, int mods);
    MouseButtonCallback mouseButtonCallback;

    GLFWmonitor* monitor;    
    GLFWwindow* window;
    const GLFWvidmode* mode;
    
    Camera* cameraInstance;

    unsigned int VAO;
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
        glViewport(0,0,width,height);
    }

    int screenWidth, screenHeight;
    bool fullScreen = false;
};