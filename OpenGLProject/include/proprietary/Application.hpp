#pragma once

//cpp standard libraries
#include <iostream>
#include <vector>

//Window libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Program.hpp>


class Application{
public:
    Application();
    ~Application();
    GLFWmonitor* get_monitor();
    GLFWwindow* get_window();
    const GLFWvidmode* getMode();
    void set_user(void* pointer);

    void set_scroll_callback(void (*func)(GLFWwindow* window, double xPos, double yPos));
    void set_mouse_callback(void (*func)(GLFWwindow* window, double xOffset, double yOffset));

    void get_cursor_position(double* x,double* y);
    
    bool main_loop();
    void enable(const std::vector<unsigned int>& toEnable);
    void blend_function(unsigned int sfactor, unsigned int dfactor);
    void clear(float r, float g, float b, float w, unsigned int clearBuffer);
    void creat_grid(const Program& gridProgram);

private:

    typedef void (*ScrollCallback)(GLFWwindow* window, double xpos, double ypos);
    ScrollCallback scrollCallback;

    typedef void (*CursorPosCallback)(GLFWwindow* window, double x_offset, double y_offset);
    CursorPosCallback cursorCallback;

    bool windowInFocus;

    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    GLFWwindow* window;
    unsigned int VAO;
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
        glViewport(0,0,width,height);
    }
};