#include "CameraUi.h"
#include <glm/gtc/matrix_transform.hpp>
#include <functional>
#include "core/Application.h"
#include "utilities/Log.h"

const float panSpeed = .1f;
const float moveSpeed = .01f;

/**********************************************************************/
void CameraUi::initialize(float fov, float near, float far) {

    const Application& app = Application::get();

    forward = glm::vec3(0, 0, -1);
    up = glm::vec3(0, 1, 0);
    position = glm::vec3(0, 0, 5);

    glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(angles.x), glm::vec3(1, 0, 0));
    ubo.view = glm::lookAt(position, position + forward, up);
    ubo.proj = glm::perspective(glm::radians(fov), app.getWidth() / (float)app.getHeight(), near, far);
    //ubo.proj = glm::ortho(0.0f, (float)app.getWidth(), (float)app.getHeight(), 0.0f, 0.f, 1.f);
    ubo.proj[1][1] *= -1;
}

/**********************************************************************/
void CameraUi::update(float dt) {

    const Application& app = Application::get();

    ubo.model = glm::mat4(1);
    //ubo.view = glm::mat4(1);
    //ubo.view = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,1), glm::vec3(0,1,0));
    //ubo.proj = glm::ortho(0.0f, (float)app.getWidth(), (float)app.getHeight(), 0.0f, 0.1f, 100.0f);
    //ubo.proj = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, 0.1f, 100.0f);
    //ubo.proj[1][1] *= -1;

    //LOGI("pos " << position << " dir " << forward);
}

/**********************************************************************/
const UniformBufferObject& CameraUi::getUbo() const {
    return ubo;
}
