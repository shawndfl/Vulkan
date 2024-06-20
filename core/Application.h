#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>

#include "core/InputManager.h"
#include "core/IScene.h"
#include "IVulkanApplication.h"
#include "utilities/Performance.h"

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

	struct Performance _performance;

	std::unique_ptr<IScene> _scene;



public:
	/**
	* Create the only instance of this application
	*/
	static void create();

	/**
	* Get the only instance of this applicaiton
	*/
	static Application& get();
	
	const std::unique_ptr<InputManager>& getInputManager();

public: 
	GLFWwindow* getWindow() const;

	VkDevice getDevice() const;

	VkPhysicalDevice getPhysicalDevice() const;

	VkRenderPass getStandardRenderPass() const;

	uint16_t maxFramesInFlight() const;

	VkCommandBuffer beginSingleTimeCommands() const;
	void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

	/**
	* Creates a buffers used for images, verteices, indices, and other things in video memory
	*/
	void createBuffer(VkDeviceSize size, 
		VkBufferUsageFlags usage, 
		VkMemoryPropertyFlags properties, 
		VkBuffer& buffer, 
		VkDeviceMemory& bufferMemory) const;

	/**
	* Finds a 
	*/
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	
	void run();

	/**
	* The glf callback that calls onKey
	*/
	friend void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:

	void initialize();

};
