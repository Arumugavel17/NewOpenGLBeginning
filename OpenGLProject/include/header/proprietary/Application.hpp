#pragma once

//cpp standard libraries
#include <iostream>
#include <vector>

//Window libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Application{
public:
    Application();
    ~Application();
    GLFWmonitor* getMonitor();
    GLFWwindow* getWindow();
    const GLFWvidmode* getMode();
    void setUser(void* pointer);
    void setScrollCallback(void (*func)(GLFWwindow* window, double xPos, double yPos));
    void setMouseCallback(void (*func)(GLFWwindow* window, double xOffset, double yOffset));
    void getCursorPosition(double* x,double* y);
    bool windowShouldClose();
    void enable(const std::vector<unsigned int>& toEnable);
    void blend_function(unsigned int sfactor, unsigned int dfactor);
    void clear(float r, float g, float b, float w, unsigned int clearBuffer);

private:
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    GLFWwindow* window;
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
        glViewport(0,0,width,height);
    }
};