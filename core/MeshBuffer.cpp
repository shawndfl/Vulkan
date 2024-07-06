#include "MeshBuffer.h"
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
VkBuffer MeshBuffer::getVertexBuffer() const {
    return vertexBuffer;
}

/**********************************************************************/
VkBuffer MeshBuffer::getIndexBuffer() const {
    return indexBuffer;
}

/**********************************************************************/
uint32_t MeshBuffer::getIndexCount() const {
    return indexCount;
}

/**********************************************************************/
void MeshBuffer::dispose() {
    const Application& app = Application::get();
    vkDestroyBuffer(app.getDevice(), indexBuffer, nullptr);
    vkFreeMemory(app.getDevice(), indexBufferMemory, nullptr);

    vkDestroyBuffer(app.getDevice(), vertexBuffer, nullptr);
    vkFreeMemory(app.getDevice(), vertexBufferMemory, nullptr);
}

/**********************************************************************/
void MeshBuffer::createVertexBuffer(const void* vertexData, const int dataSize, size_t elementCount) {
    const Application& app = Application::get();

    VkDeviceSize bufferSize = dataSize * elementCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    app.createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(app.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertexData, (size_t)bufferSize);
    vkUnmapMemory(app.getDevice(), stagingBufferMemory);

    app.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(app.getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(app.getDevice(), stagingBufferMemory, nullptr);
}

/**********************************************************************/
void MeshBuffer::createIndexBuffer(const void* indexData, const int dataSize, size_t elementCount) {
    const Application& app = Application::get();
    VkDeviceSize bufferSize = dataSize * elementCount;
    indexCount = (int)elementCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    app.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(app.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indexData, (size_t)bufferSize);
    vkUnmapMemory(app.getDevice(), stagingBufferMemory);

    app.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(app.getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(app.getDevice(), stagingBufferMemory, nullptr);
}