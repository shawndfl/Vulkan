#pragma once
#include <vulkan/vulkan.h>
#include <vector>

/**
* This class manages an index and vertex buffer. Remember to call dispose when you are
* done with the buffers.
*/
class GeometryBuffer {

private :
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

public:
	VkBuffer getVertexBuffer() const;
	VkBuffer getIndexBuffer() const;

	template<class T>
	void createBuffers(const std::vector<T>& vertexData, const std::vector<unsigned short>& indexData);

	void dispose();
private:
	void createVertexBuffer(const void* vertexData, const int dataSize, size_t elementCount);
	void createIndexBuffer(const void* indexData, const int dataSize, size_t elementCount);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	
};

/**********************************************************************/
template<class T>
void GeometryBuffer::createBuffers(const std::vector<T>& vertexData, const std::vector<uint16_t>& indexData) {
	createVertexBuffer(vertexData.data(), sizeof(T), vertexData.size());
	createIndexBuffer(indexData.data(), sizeof(unsigned short), indexData.size());
}
