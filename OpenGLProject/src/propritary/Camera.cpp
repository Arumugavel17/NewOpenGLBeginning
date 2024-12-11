#include<Camera.hpp>

Camera::Camera(int screenWidth, int screenHeight) {
    this->screenHeight = screenHeight;
    this->screenWidth = screenWidth;
    lastX = this->screenWidth / 2.0;
    lastY = this->screenHeight / 2.0;
    yaw = -90.0f;  // Starting yaw so the camera points forward
    pitch = 0.0f;  // Starting pitch level
}

glm::mat4 Camera::camera_input(GLFWwindow* window){

    currenttime = (float)glfwGetTime();
    float deltatime = currenttime - lasttime;
    float speed_factor = 0.5f;
    lasttime = currenttime;
    
    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        speed_factor = 10;
    }else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        speed_factor = 2.5f;
    }

    const float cameraSpeed = speed_factor * deltatime; // adjust accordingly

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -= cameraSpeed * cameraFront;
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos += cameraSpeed * cameraFront;
    }
    if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS){
        cameraPos += cameraSpeed * glm::normalize(cameraUp);
    }
    if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS){
        cameraPos += cameraSpeed * glm::normalize(-cameraUp);
    }

    //std::cout << "cameraPos: " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << "\n";
    //std::cout << "cameraFront: " << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << "\n";
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::get_projection(bool perspectiveProjection){
    if (perspectiveProjection) {
        return glm::perspective(glm::radians(zoom),(float) screenWidth / (float) screenHeight , nearPlane, farPlane);
    }
    return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
}

void Camera::set_mouse_x_y(double x , double y){
    lastX = x;
    lastY = y;
}

glm::vec3 Camera::get_camera_pos(){
    return cameraPos;
}

glm::vec3 Camera::get_camera_front(){
    return cameraFront;
}

void Camera::process_mouse_scroll_impl(double x_offset, double y_offset) {
    zoom -= (float)y_offset;
    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
    if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}

void Camera::process_mouse_input_impl(GLFWwindow* window, double xpos, double ypos) {

    if (mouse_can) {
        return;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    double sensitivity = 0.4f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

}
void Camera::process_key_input_impl(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        mouse_can = true;
        // Release and show the cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Inform ImGui about the state change
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        std::cout << " key ";
    }
}

void Camera::process_mouse_button_impl(GLFWwindow* window, int button, int action, int mods) {
   
}


void Camera::process_mouse_input(GLFWwindow* window, double xpos, double ypos) {
    // Obtain the camera instance from the GLFW window user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        // Call the non-static method using the camera instance
        camera->process_mouse_input_impl(window, xpos, ypos);
    }
}

void Camera::process_mouse_scroll(GLFWwindow* window, double xpos, double ypos) {
    // Obtain the camera instance from the GLFW window user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        // Call the non-static method using the camera instance
        camera->process_mouse_scroll_impl(xpos, ypos);
    }
}


void Camera::process_key_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Obtain the camera instance from the GLFW window user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        // Call the non-static method using the camera instance
        camera->process_key_input_impl(window, key,scancode,action,mods);
    }
}

void Camera::process_mouse_button_input(GLFWwindow* window, int button, int action, int mods) {
    // Obtain the camera instance from the GLFW window user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        // Call the non-static method using the camera instance
        camera->process_mouse_button_impl(window, button, action, mods);
    }
}