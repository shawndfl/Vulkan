#include "CameraFPS.h"
#include "core/Application.h"
#include <glm/gtc/matrix_transform.hpp>
#include "utilities/Log.h"

const float panSpeed = .1f;
const float moveSpeed = .01f;

/**********************************************************************/
void CameraFPS::initialize(float fov, float near, float far) {

    const Application& app = Application::get();

    glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(angles.x), glm::vec3(1, 0, 0));
    ubo.view = glm::lookAt(position, position + forward, up);
    ubo.proj = glm::perspective(glm::radians(fov), app.getWidth() / (float)app.getHeight(), near, far);
    ubo.proj[1][1] *= -1;

}

/**********************************************************************/
void CameraFPS::registerForInput() {
    Application& app = Application::get();
    
    using namespace std::placeholders;

    forward = glm::vec3(0, 0, 1);
    up = glm::vec3(0, 1, 0);
    position = glm::vec3(0, 0, 5);

    app.getInputManager()->registerKeyboardHandler("CameraFPS", std::bind(&CameraFPS::handleKeyboard, this, _1, _2, _3, _4));
    app.getInputManager()->registerMouseHandler("CameraFPS", std::bind(&CameraFPS::handleMouse, this, _1, _2, _3, _4, _5));
}

/**********************************************************************/
void CameraFPS::handleKeyboard( int key, int scancode, int action, int mods) {
    //LOGI("keyboard btn: " << (char)key << "(" << key <<") " << " mods: " << mods << " action: " << action << " scan: " << scancode);
}

/**********************************************************************/
void CameraFPS::handleMouse(int button, int action, int mods, int x, int y) {
    //LOGI("mouse btn: " << button << " pos: " << x << ", " << y << " action: " << action);

    // skip the first frame
    if (glm::length(lastMousePos) == 0) {
        lastMousePos.x = (float)x;
        lastMousePos.y = (float)y;
        return;
    }

    glm::vec2 delta(x - lastMousePos.x, y - lastMousePos.y);
    angles.x += delta.x * panSpeed;
    angles.y -= delta.y * panSpeed;

    lastMousePos.x = (float)x;
    lastMousePos.y = (float)y;
}

/**********************************************************************/
void CameraFPS::update(float dt) {

    const Application& app = Application::get();

    GLFWwindow* window = app.getWindow();
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += moveSpeed * forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= moveSpeed * forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(forward, up)) * moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(forward, up)) * moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        position += glm::vec3(0.f,1.f,0.f) * moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        position -= glm::vec3(0.f, 1.f, 0.f) * moveSpeed;

    // clamp the angle
    angles.y = glm::clamp(angles.y, -70.0f, 70.0f);
    
    ubo.model = glm::mat4(1);

    float yaw = glm::radians(angles.x);
    float pitch = glm::radians(angles.y);

    direction.x = cos(yaw) * cos(pitch);
    direction.y = sin(pitch);
    direction.z = sin(yaw) * cos(pitch);
    forward = glm::normalize(direction);

    ubo.view = glm::lookAt(position, position + forward, up);
    ubo.proj = glm::perspective(glm::radians(45.0f), app.getWidth() / (float)app.getHeight(), 0.1f, 100.0f);
    ubo.proj[1][1] *= -1;

    //LOGI("pos " << position << " dir " << forward);
}

/**********************************************************************/
const UniformBufferObject& CameraFPS::getUbo() const {
    return ubo;
}
