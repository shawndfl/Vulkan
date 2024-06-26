#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "core/IDisposable.h"
#include <optional>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain: IDisposable {
public:
    void initialize();

    void createFramebuffers();

    int getWidth() const;

    int getHeight() const;

    VkFormat getSwapChainImageFormat() const;

    void dispose();

    VkFramebuffer getFrameBuffer(int index) ;

    VkExtent2D getExtend2D() const;

    VkSwapchainKHR getSwapChain();

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

private:
    void createSwapChain();
    void createImageViews();
    void createColorResources();
    void createDepthResources();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
private:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
};

