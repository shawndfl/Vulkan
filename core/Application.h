#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "IVulkanApplication.h"
#include "core/InputManager.h"

/**
* Main application that runs everything
*/
class Application {
private:
	Application();
	/**
	* Single instance of the class
	*/
	static Application* _instance;

	std::unique_ptr<IVulkanApplication> _internal;

	std::unique_ptr<InputManager> _inputManager;

public:
	/**
	* Create the only instance of this application
	*/
	static void create();

	/**
	* Get the only instance of this applicaiton
	*/
	static Application& get();
	
	const std::unique_ptr<InputManager>& getInputManager() const;
public: 
	GLFWwindow* getWindow() const;

	VkDevice getDevice() const;
	
	void run();

	/**
	* The glf callback that calls onKey
	*/
	friend void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:

	void initialize();

};
