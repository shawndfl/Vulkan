#pragma once
#include <vulkan/vulkan.h>

class IVulkanApplication {
public:
	virtual VkDevice getDevice() const = 0;

	virtual VkRenderPass getStandardRenderPass() const = 0;

	virtual void init() = 0;

	virtual GLFWwindow* getWindow() const = 0;

	virtual VkPhysicalDevice getPhysicalDevice() const = 0;

	virtual VkCommandBuffer beginSingleTimeCommands() = 0;

	virtual void endSingleTimeCommands(VkCommandBuffer commandBuffer) = 0;

	virtual int getWidth() const = 0;

	virtual int getHeight() const = 0;

	virtual void update(float dt) = 0;

	virtual void dispose() = 0;
};