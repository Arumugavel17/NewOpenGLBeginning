#include<Application.hpp>

Application::Application(){
    
    if(!glfwInit()){
        std::cout << " Failed to Initialize GLFW";
        return;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
    monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(mode->width, mode->height, "Tutorial 01", monitor, NULL);

    if( window == NULL ){
        std::cout <<  stderr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" ;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);  
    
}

void Application::setUser(void* pointer){
    glfwSetWindowUserPointer(window,pointer);
}

GLFWmonitor* Application::getMonitor(){
    return monitor;
}

GLFWwindow* Application::getWindow(){
    return window;
}

const GLFWvidmode* Application::getMode(){
    return mode;
}

void Application::setScrollCallback(void (*func)(GLFWwindow* window, double xpos, double ypos)){
    glfwSetScrollCallback(window,func);
}

void Application::setMouseCallback(void (*func)(GLFWwindow* window, double x_offset, double y_offset)){
    glfwSetCursorPosCallback(window, func);
}

void Application::getCursorPosition(double* x,double* y){
    glfwGetCursorPos(window,x,y);
}

bool Application::windowShouldClose(){
    return glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

Application::~Application(){
    glfwTerminate();
}

void Application::enable(const std::vector<unsigned int>& toEnable) {
    for (const auto& val : toEnable) {
        glEnable(val);
    }
}

void Application::clear(float r, float g, float b, float w, unsigned int clearBuffer) {
    glClear(clearBuffer);
    glClearColor(r, g, b, w);
}

void Application::blend_function(unsigned int sfactor,unsigned int dfactor) {
    glBlendFunc(sfactor, dfactor);
}