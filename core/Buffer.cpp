#include "Buffer.h"
#include "utilities/Log.h"
#include "core/Application.h"

/**********************************************************************/
Buffer::Buffer() {
    m_buffer = nullptr;
    m_bufferData = nullptr;
    m_bufferMemory = nullptr;
}

/**********************************************************************/
Buffer::~Buffer() {
    dispose();
}

/**********************************************************************/
void Buffer::dispose() {
    const Application& app = Application::get();
    vkDestroyBuffer(app.getDevice(), m_buffer, nullptr);
    vkUnmapMemory(app.getDevice(), m_bufferMemory);
    vkFreeMemory(app.getDevice(), m_bufferMemory, nullptr);
    m_bufferData = nullptr;
}

/**********************************************************************/
void Buffer::copyHostToBufferOnce(void* data, VkDeviceSize size) {
    const Application& app = Application::get();

    void* tmp = nullptr;
    vkMapMemory(app.getDevice(), m_bufferMemory, 0, size, 0, &tmp);
    memcpy(tmp, data, (size_t)size);
    vkUnmapMemory(app.getDevice(), m_bufferMemory);
}

/**********************************************************************/
void Buffer::copyHostToBuffer(void* data, VkDeviceSize size) {
    const Application& app = Application::get();

    vkMapMemory(app.getDevice(), m_bufferMemory, 0, size, 0, &m_bufferData);
    memcpy(m_bufferData, data, (size_t)size);
}

/**********************************************************************/
void Buffer::copyToBuffer(VkBuffer dstBuffer, VkDeviceSize size) {
    const Application& app = Application::get();
    VkCommandBuffer commandBuffer = app.beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, m_buffer, dstBuffer, 1, &copyRegion);

    app.endSingleTimeCommands(commandBuffer);
}

/**********************************************************************/
void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    const Application& app = Application::get();
    VkBufferCreateInfo bufferInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(app.getDevice(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        LOGE("failed to create buffer! size: " << size);
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(app.getDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = app.findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(app.getDevice(), &allocInfo, nullptr, &m_bufferMemory) != VK_SUCCESS) {
        LOGE("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(app.getDevice(), m_buffer, m_bufferMemory, 0);
}