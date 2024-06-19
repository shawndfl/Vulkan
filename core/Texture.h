#pragma once
#include <vulkan/vulkan.h>
#include <string>

class Texture {
private: 
	VkImageView textureImageView;
	VkSampler textureSampler;

	int texWidth;
	int texHeight;
	int texChannels;

	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	
public:
	void initialize(const std::string& filename);
	VkImageView getTextureImageView() const {
		return textureImageView;
	}
	VkSampler getTextureSampler() const {
		return textureSampler;
	}

private:
	void createTextureSampler();
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
};

