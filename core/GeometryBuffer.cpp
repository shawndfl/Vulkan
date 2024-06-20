#include "GeometryBuffer.h"
#include "core/Application.h"
#include "utilities/Log.h"


/**
* Internal copy buffer for the device memory
*/
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    const Application& app = Application::get();
    VkCommandBuffer commandBuffer = app.beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    app.endSingleTimeCommands(commandBuffer);
}

/**********************************************************************/
VkBuffer GeometryBuffer::getVertexBuffer() const {
    return vertexBuffer;
}

/**********************************************************************/
VkBuffer GeometryBuffer::getIndexBuffer() const {
    return indexBuffer;
}

/**********************************************************************/
void GeometryBuffer::dispose() {
    const Application& app = Application::get();
    vkDestroyBuffer(app.getDevice(), indexBuffer, nullptr);
    vkFreeMemory(app.getDevice(), indexBufferMemory, nullptr);

    vkDestroyBuffer(app.getDevice(), vertexBuffer, nullptr);
    vkFreeMemory(app.getDevice(), vertexBufferMemory, nullptr);
}

/**********************************************************************/
void GeometryBuffer::createVertexBuffer(const void* vertexData, const int dataSize, size_t elementCount) {
    const Application& app = Application::get();

    VkDeviceSize bufferSize = dataSize * elementCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(app.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertexData, (size_t)bufferSize);
    vkUnmapMemory(app.getDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(app.getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(app.getDevice(), stagingBufferMemory, nullptr);
}

/**********************************************************************/
void GeometryBuffer::createIndexBuffer(const void* indexData, const int dataSize, size_t elementCount) {
    const Application& app = Application::get();
    VkDeviceSize bufferSize = dataSize * elementCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(app.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indexData, (size_t)bufferSize);
    vkUnmapMemory(app.getDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(app.getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(app.getDevice(), stagingBufferMemory, nullptr);
}

/**********************************************************************/
void GeometryBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    const Application& app = Application::get();
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(app.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        LOGE("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(app.getDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = app.findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(app.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(app.getDevice(), buffer, bufferMemory, 0);
}
