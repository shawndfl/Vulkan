#include "CameraFPS.h"
#include "core/Application.h"
#include <glm/gtc/matrix_transform.hpp>
#include "utilities/Log.h"

const float panSpeed = .001f;
const float moveSpeed = .001f;

/**********************************************************************/
void CameraFPS::initialize(float fov, float near, float far) {

    const Application& app = Application::get();

    glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(angles.x), glm::vec3(1, 0, 0));
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::perspective(glm::radians(fov), app.getWidth() / (float)app.getHeight(), near, far);
    ubo.proj[1][1] *= -1;

}

/**********************************************************************/
void CameraFPS::registerForInput() {
    Application& app = Application::get();
    
    using namespace std::placeholders;

    app.getInputManager()->registerKeyboardHandler("CameraFPS", std::bind(&CameraFPS::handleKeyboard, this, _1, _2, _3, _4));
    app.getInputManager()->registerMouseHandler("CameraFPS", std::bind(&CameraFPS::handleMouse, this, _1, _2, _3, _4, _5));
}

/**********************************************************************/
void CameraFPS::handleKeyboard( int key, int scancode, int action, int mods) {
    LOGI("keyboard btn: " << key << " mods: " << mods << " action: " << action);
}

/**********************************************************************/
void CameraFPS::handleMouse( int button, int action, int mods, int x, int y) {
    LOGI("mouse btn: " << button << " pos: " << x << ", " << y << " action: " << action);
}

/**********************************************************************/
void CameraFPS::update(float dt) {

    const Application& app = Application::get();

    //ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.0f, 0.0f));

    //ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.model = glm::mat4(1);// glm::translate(ubo.model, glm::vec3(0.0f, 0.0f, 0.0f));

    //glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(angles.x), glm::vec3(1, 0, 0));
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), app.getWidth() / (float)app.getHeight(), 0.1f, 100.0f);
    ubo.proj[1][1] *= -1;
}

/**********************************************************************/
const UniformBufferObject& CameraFPS::getUbo() const {
    return ubo;
}

/**********************************************************************/
void CameraFPS::pan(float dx, float dy) {
    angles.x += panSpeed * dx;
    angles.y += panSpeed * dy;
}

/**********************************************************************/
void CameraFPS::move(const glm::vec3& direction) {

}
