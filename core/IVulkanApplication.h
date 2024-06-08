#pragma once
#include <vulkan/vulkan.h>

class IVulkanApplication {
public:
	virtual VkDevice getDevice() const = 0;

	virtual void init() = 0;

	virtual void run() = 0;

	virtual GLFWwindow* getWindow() const = 0;
};