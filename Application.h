#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <memory>

#include "IVulkanApplication.h"

/**
* Main application that runs everything
*/
class Application {
private:
	Application();
	std::unique_ptr<IVulkanApplication> _internal;
	static Application* _instance;

public:
	/**
	* Create the only instance of this application
	*/
	static Application* create();

	/**
	* Get the only instance of this applicaiton
	*/
	static Application* get();

public: 
	GLFWwindow* getWindow() const;

	VkDevice getDevice() const;
	
	void run();

protected:
	/**
	* The glf callback that calls onKey
	*/
	friend void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
};
