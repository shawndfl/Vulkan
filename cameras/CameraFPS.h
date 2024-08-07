#pragma once
#include <vulkan/vulkan.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "descriptorSets/UniformBufferObject.h"

class CameraFPS {
private:
	UniformBufferObject ubo;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 angles;
	glm::vec2 lastMousePos;
	float m_near;
	float m_far;

public:

	void initialize(float fov = 45.f, float near =.1f , float far = 100.0f);

	void registerForInput();

	const UniformBufferObject& getUbo() const;

	void update(float dt);

private:
	void handleMouse( int button, int action, int mods, int x, int y);
	void handleKeyboard( int key, int scancode, int action, int mods);
};

