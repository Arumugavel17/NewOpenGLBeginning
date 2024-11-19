#pragma once

//cpp standard libraries
#include<iostream>

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
private:
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;
    GLFWwindow* window;
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
        glViewport(0,0,width,height);
    }
};