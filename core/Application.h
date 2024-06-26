#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <chrono>

#include "core/InputManager.h"
#include "core/IScene.h"
#include "IVulkanApplication.h"
#include "utilities/Performance.h"
#include <optional>

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

/**
* Main application that runs everything
*/
class Application {
public:
	/**
	* Create the only instance of this application
	*/
	static void create();

	/**
	* Get the only instance of this applicaiton
	*/
	static Application& get();
	
	std::unique_ptr<InputManager>& getInputManager();

public: 
	GLFWwindow* getWindow() const;

	VkDevice getDevice() const;

	VkPhysicalDevice getPhysicalDevice() const;

	VkRenderPass getRenderPass() const;

	uint16_t maxFramesInFlight() const;

	VkCommandBuffer beginSingleTimeCommands() const;
	void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

	int getWidth() const;

	int getHeight() const;

	VkFormat findDepthFormat() const;

	VkSampleCountFlagBits getMsaaSamples() const;

	VkFormat getSwapChainImageFormat() const;

	VkSurfaceKHR getSurface() const;

	QueueFamilyIndices findQueueFamilies() const;

	virtual VkQueue getGraphicsQueue() const;

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

private:
	Application();
	/**
	* Single instance of the class
	*/
	static Application* m_instance;

	std::unique_ptr<IVulkanApplication> m_internal;

	std::unique_ptr<InputManager> _inputManager;

	struct Performance _performance;

	std::unique_ptr<IScene> _scene;

	std::chrono::steady_clock::time_point lastTime;


};
