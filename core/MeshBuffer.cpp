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
    return m_vertexBuffer.getVkBuffer();
}

/**********************************************************************/
VkBuffer MeshBuffer::getIndexBuffer() const {
    return m_indexBuffer.getVkBuffer();
}

/**********************************************************************/
uint32_t MeshBuffer::getIndexCount() const {
    return indexCount;
}

/**********************************************************************/
void MeshBuffer::dispose() {
    m_indexBuffer.dispose();
    m_vertexBuffer.dispose();
}

/**********************************************************************/
void MeshBuffer::createVertexBuffer(const void* vertexData, const int dataSize, size_t elementCount) {

    VkDeviceSize bufferSize = dataSize * elementCount;

    Buffer staging;
    staging.createBuffer(bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    staging.copyHostToBufferOnce(vertexData);

    m_vertexBuffer.createBuffer(bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    staging.copyToBuffer(m_vertexBuffer.getVkBuffer());
}

/**********************************************************************/
void MeshBuffer::createIndexBuffer(const void* indexData, const int dataSize, size_t elementCount) {
    
    VkDeviceSize bufferSize = dataSize * elementCount;
    indexCount = (int)elementCount;

    Buffer staging;
    staging.createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    staging.copyHostToBufferOnce(indexData);

    m_indexBuffer.createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    staging.copyToBuffer(m_indexBuffer.getVkBuffer());
}