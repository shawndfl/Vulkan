#pragma once
#include <vulkan/vulkan.h>

class Buffer {
public:
	Buffer();
	~Buffer();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

	void copyHostToBufferOnce(void* data, VkDeviceSize size);
	void copyHostToBuffer(void* data, VkDeviceSize size);
	void copyToBuffer(VkBuffer dstBuffer, VkDeviceSize size);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void dispose();

private:
	VkBuffer m_buffer;
	VkDeviceMemory m_bufferMemory;
	void* m_bufferData;
};

