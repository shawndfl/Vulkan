#pragma once
#include <vulkan/vulkan.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "descriptorSets/UniformBufferObject.h"

class CameraUi {
private:
	UniformBufferObject ubo;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 angles;
	glm::vec2 lastMousePos;

public:

	void initialize(float fov = 45.f, float near = .1f, float far = 100.0f);

	const UniformBufferObject& getUbo() const;

	void update(float dt);

};

