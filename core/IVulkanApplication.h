#pragma once
#include <vulkan/vulkan.h>

class IVulkanApplication {
public:
	virtual VkDevice getDevice() const = 0;

	virtual VkRenderPass getStandardRenderPass() const = 0;

	virtual void init() = 0;

	virtual void run() = 0;

	virtual GLFWwindow* getWindow() const = 0;

	virtual VkPhysicalDevice getPhysicalDevice() const = 0;

	virtual VkCommandBuffer beginSingleTimeCommands() = 0;

	virtual void endSingleTimeCommands(VkCommandBuffer commandBuffer) = 0;
};