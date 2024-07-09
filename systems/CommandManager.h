#pragma once
#include "core/ISystemManager.h"
#include <vulkan/vulkan.h>
#include <vector>

class CommandManager: public ISystemManager {
public:
	void createCommandPool();

	void dispose();

	VkCommandBuffer beginSingleTimeCommands() const;

    void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

	void createCommandBuffers();

	const std::vector<VkCommandBuffer>& getDrawingCommandBuffers() const;

	VkCommandBuffer getActiveDrawingCommand() const;

private:
	VkCommandPool m_commandPool;

	std::vector<VkCommandBuffer> m_commandBuffers;
};

