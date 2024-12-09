#include <Application.hpp>

Application::Application(float r, float g, float b, float a,bool fullScreen, int width, int height) {
    
    this->fullScreen = fullScreen;
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

    if (this->fullScreen) {
        this->screenWidth = mode->width;
        this->screenHeight = mode->height;
        window = glfwCreateWindow(this->screenWidth, this->screenHeight, "Tutorial 01", monitor, NULL);
    }
    else {
        this->screenWidth = width;
        this->screenHeight = height;
        window = glfwCreateWindow(this->screenWidth, this->screenHeight, "Tutorial 01", NULL, NULL);
    }

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);  
    glGenVertexArrays(1, &VAO);
    glfwSetWindowShouldClose(window, GL_FALSE);
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

const GLFWvidmode* Application::get_mode() {
    return mode;
}


int Application::get_screen_width() {
    return this->screenWidth;
}

int Application::get_screen_height() {
    return this->screenHeight;
}

void Application::set_scroll_callback(void (*func)(GLFWwindow* window, double xpos, double ypos)) {
    this->scrollCallback = func;
    glfwSetScrollCallback(window, this->scrollCallback);
}

void Application::set_mouse_callback(void (*func)(GLFWwindow* window, double x_offset, double y_offset)) {
    this->cursorCallback = func;
    glfwSetCursorPosCallback(window, this->cursorCallback);
}


void Application::set_key_call_back(void (*func)(GLFWwindow* window, int key, int scancode, int action, int mods)) {
   
    this->keyCallback = func;
    glfwSetKeyCallback(window, this->keyCallback);
}

void Application::set_mouse_button_callback(void (*func)(GLFWwindow* window, int button, int action, int mods)) {

    this->mouseButtonCallback = func;
    glfwSetMouseButtonCallback(window, this->mouseButtonCallback);
}


void Application::get_cursor_position(double* x,double* y) {
    glfwGetCursorPos(window,x,y);
}

bool Application::main_loop() {
    glfwSwapBuffers(get_window());
    glfwPollEvents();
    clear(bg_r, bg_g, bg_b, bg_a, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    return !glfwWindowShouldClose(window);
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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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