#include "Buffer.h"
#include "utilities/Log.h"
#include "core/Application.h"

/**********************************************************************/
Buffer::Buffer() {
    m_buffer = nullptr;
    m_bufferData = nullptr;
    m_bufferMemory = nullptr;
    m_size = 0;
}

/**********************************************************************/
Buffer::~Buffer() {
    dispose();
}

/**********************************************************************/
void Buffer::dispose() {
    const Application& app = Application::get();
    if (m_buffer) {
        vkDestroyBuffer(app.getDevice(), m_buffer, nullptr);
    }
    if (m_bufferData) {
        vkUnmapMemory(app.getDevice(), m_bufferMemory);
        m_bufferData = nullptr;
    }
    if (m_bufferMemory) {
        vkFreeMemory(app.getDevice(), m_bufferMemory, nullptr);
    }
    m_size = 0;
}

/**********************************************************************/
VkBuffer Buffer::getVkBuffer() const {
    return m_buffer;
}

/**********************************************************************/
void Buffer::copyHostToBufferOnce(const void* data) {
    const Application& app = Application::get();

    void* tmp = nullptr;
    vkMapMemory(app.getDevice(), m_bufferMemory, 0, m_size, 0, &tmp);
    memcpy(tmp, data, (size_t)m_size);
    vkUnmapMemory(app.getDevice(), m_bufferMemory);
}

/**********************************************************************/
void Buffer::mapBuffer() {
    const Application& app = Application::get();

    // map it if we need to
    if (!m_bufferData) {
        vkMapMemory(app.getDevice(), m_bufferMemory, 0, m_size, 0, &m_bufferData);
    }
}

/**********************************************************************/
void Buffer::copyHostToBuffer(const void* data) {
    const Application& app = Application::get();

    // map it if we need to
    if (!m_bufferData) {
        vkMapMemory(app.getDevice(), m_bufferMemory, 0, m_size, 0, &m_bufferData);
    }

    // copy the memory
    memcpy(m_bufferData, data, (size_t)m_size);
}

/**********************************************************************/
void Buffer::copyToBuffer(VkBuffer dstBuffer) {
    const Application& app = Application::get();
    VkCommandBuffer commandBuffer = app.beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = m_size;
    vkCmdCopyBuffer(commandBuffer, m_buffer, dstBuffer, 1, &copyRegion);

    app.endSingleTimeCommands(commandBuffer);
}

/**********************************************************************/
void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    const Application& app = Application::get();
    VkBufferCreateInfo bufferInfo{};

    // just in case we already created one
    dispose();

    // save the size
    m_size = size;

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