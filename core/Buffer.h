#pragma once
#include <vulkan/vulkan.h>

class Buffer {
public:
	Buffer();
	~Buffer();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

	void copyHostToBufferOnce(const void* data);
	void copyHostToBuffer(const void* data);
	void copyToBuffer(VkBuffer dstBuffer);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void dispose();

	void mapBuffer();
	VkBuffer getVkBuffer() const;

private:
	VkBuffer m_buffer;
	VkDeviceMemory m_bufferMemory;
	void* m_bufferData;
	VkDeviceSize m_size;
};

