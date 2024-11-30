#include<Camera.hpp>

Camera::Camera(int screenWidth, int screenHeight) {
    lastX = screenWidth / 2.0;
    lastY = screenHeight / 2.0;
    yaw = -90.0f;  // Starting yaw so the camera points forward
    pitch = 0.0f;  // Starting pitch level
}

void Camera::process_mouse_scroll_impl(double x_offset, double y_offset){
    zoom -= (float)y_offset;
    if(zoom < 1.0f){
        zoom = 1.0f;
    }
    if(zoom > 45.0f){
        zoom = 45.0f;
    }
}   

void Camera::process_mouse_input_impl(double xpos, double ypos) {

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    double sensitivity = 0.1f;
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

glm::mat4 Camera::process_key_input(GLFWwindow* window){

    currenttime = (float)glfwGetTime();
    float deltatime = currenttime - lasttime;
    float speed_factor;
    lasttime = currenttime;
    
    if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        speed_factor = 5;
    }else{
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

    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::process_mouse_input(GLFWwindow* window, double xpos, double ypos) {
    // Obtain the camera instance from the GLFW window user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        // Call the non-static method using the camera instance
        camera->process_mouse_input_impl(xpos, ypos);
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

glm::mat4 Camera::get_projection(const GLFWvidmode* mode){
    return glm::perspective(glm::radians(zoom),(float) mode->width / (float) mode->height , 0.1f,100.0f);
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
