#pragma once
#include <vulkan/vulkan.h>
#include "core/IDisposable.h"
#include <vector>

class CommandManager: public IDisposable {
public:
	void createCommandPool();
	void dispose();

	VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void createCommandBuffers();

	const std::vector<VkCommandBuffer>& getDrawingCommandBuffers() const;

private:
	VkCommandPool m_commandPool;

	std::vector<VkCommandBuffer> m_commandBuffers;
};
