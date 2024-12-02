#include <Application.hpp>

Application::Application(float r, float g, float b, float a) {
    
    if(!glfwInit()) {
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

    if( window == NULL ) {
        std::cout <<  stderr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" ;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    bg_r = r;
    bg_g = g;
    bg_b = b;
    bg_a = a;
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);  
    glGenVertexArrays(1, &VAO);

}

void Application::set_user(void* pointer) {
    cameraInstance = (Camera*)pointer;
    glfwSetWindowUserPointer(window,pointer);
}

GLFWmonitor* Application::get_monitor() {
    return monitor;
}

GLFWwindow* Application::get_window() {
    return window;
}

const GLFWvidmode* Application::getMode() {
    return mode;
}

void Application::set_scroll_callback(void (*func)(GLFWwindow* window, double xpos, double ypos)) {
    scrollCallback = func;
    glfwSetScrollCallback(window, scrollCallback);
}

void Application::set_mouse_callback(void (*func)(GLFWwindow* window, double x_offset, double y_offset)) {
    cursorCallback = func;
    glfwSetCursorPosCallback(window, cursorCallback);
}

void Application::get_cursor_position(double* x,double* y) {
    glfwGetCursorPos(window,x,y);
}

bool Application::main_loop() {
    glfwSwapBuffers(get_window());
    glfwPollEvents();
    clear(bg_r, bg_g, bg_b, bg_a, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    return true;
}

Application::~Application() {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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

void Application::creat_grid(const Program& gridProgram) {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); 
    glBindVertexArray(0);
}

void Application::enable_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}