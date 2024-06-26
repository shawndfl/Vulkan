#pragma once
#include <vulkan/vulkan.h>

class DeviceImage {
public:

    static VkImageView createImageView(
        VkImage image, 
        VkFormat format, 
        VkImageAspectFlags aspectFlags,
        uint32_t mipLevels);

    static void createImage(
        uint32_t width,
        uint32_t height, 
        uint32_t mipLevels, 
        VkSampleCountFlagBits numSamples, 
        VkFormat format, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkImage& image, 
        VkDeviceMemory& imageMemory);
private:

};

