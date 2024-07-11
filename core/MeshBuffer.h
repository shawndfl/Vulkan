#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "core/Buffer.h"

/**
* This class manages an index and vertex buffer. Remember to call dispose when you are
* done with the buffers.
*/
class MeshBuffer {

public:
	VkBuffer getVertexBuffer() const;
	VkBuffer getIndexBuffer() const;
	uint32_t getIndexCount() const;

	template<class T>
	void createBuffers(const std::vector<T>& vertexData, const std::vector<unsigned short>& indexData);

	void dispose();
private:
	void createVertexBuffer(const void* vertexData, const int dataSize, size_t elementCount);
	void createIndexBuffer(const void* indexData, const int dataSize, size_t elementCount);

private:
private:
	
	uint32_t indexCount;

	Buffer m_indexBuffer;
	Buffer m_vertexBuffer;
	
};

/**********************************************************************/
template<class T>
void MeshBuffer::createBuffers(const std::vector<T>& vertexData, const std::vector<uint16_t>& indexData) {
	createVertexBuffer(vertexData.data(), sizeof(T), vertexData.size());
	createIndexBuffer(indexData.data(), sizeof(unsigned short), indexData.size());
}
