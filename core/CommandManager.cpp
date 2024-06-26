#include "CommandManager.h"
#include "core/Application.h"
#include "utilities/Log.h"

/**********************************************************************/
void CommandManager::createCommandPool() {
    const Application& app = Application::get();
    const VkDevice device = app.getDevice();

    QueueFamilyIndices queueFamilyIndices = app.findQueueFamilies();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        LOGE("failed to create graphics command pool!");
    }

    createCommandBuffers();
}

/**********************************************************************/
void CommandManager::dispose() {
    const Application& app = Application::get();
    const VkDevice device = app.getDevice();
    vkDestroyCommandPool(device, m_commandPool, nullptr);
}

/**********************************************************************/
VkCommandBuffer CommandManager::beginSingleTimeCommands() {
    const Application& app = Application::get();
    const VkDevice device = app.getDevice();

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

/**********************************************************************/
void CommandManager::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    const Application& app = Application::get();
    const VkDevice device = app.getDevice();

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(app.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(app.getGraphicsQueue());

    vkFreeCommandBuffers(device, m_commandPool, 1, &commandBuffer);
}

/**********************************************************************/
void CommandManager::createCommandBuffers() {
    const Application& app = Application::get();
    const VkDevice device = app.getDevice();

    m_commandBuffers.resize(app.maxFramesInFlight());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
        LOGE("failed to allocate command buffers!");
    }
}

/**********************************************************************/
const std::vector<VkCommandBuffer>& CommandManager::getDrawingCommandBuffers() const {
    return m_commandBuffers;
}
