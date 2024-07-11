#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>

#include "utilities/Log.h"
#include "application.h"

#include "scenes/GameScene.h"
#include "utilities/Assets.h"

#include "geometry/VertexTypes.h"
#include "geometry/PlaneGeo.h"
#include "geometry/BoxGeo.h"

#include "cameras/CameraFPS.h"
#include "cameras/CameraUi.h"
#include "renderPasses/RenderPass.h"
#include "utilities/Log.h"
#include "systems/FontManager.h"
#include "pipelines/StandardGraphicPipeline.h"
#include "descriptorSets/DescriptorPool.h"
#include "descriptorSets/StandardDescriptorSet.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::string MODEL_PATH = "./assets/viking_room.obj";
const std::string TEXTURE_PATH = "./assets/tiles.png";

/**
* Duble buffering
*/
const int MAX_FRAMES_IN_FLIGHT = 2;

//
//Input events from GLFW
//

/**********************************************************************/
void Application::onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->getInputManager()->onKey(window, key, scancode, action, mods);
}

/**********************************************************************/
void Application::onCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->getInputManager()->onCursorPos(window, (int)xpos, (int)ypos);
}

/**********************************************************************/
void Application::onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->getInputManager()->onMouseButton(window, button, action, mods);
}

/**********************************************************************/
void Application::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

/**********************************************************************/
VKAPI_ATTR VkBool32 VKAPI_CALL Application::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cout << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

//
// Validation layers
//
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


VkResult CreateDebugUtilsMessengerEXT2(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
void DestroyDebugUtilsMessengerEXT2(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

VkDevice Application::getDevice() const {
    return device;
}

GLFWwindow* Application::getWindow() const {
    return window;
}

VkRenderPass Application::getRenderPass() const {
    return m_renderPass->getRenderPass();
}

VkFramebuffer Application::getFrameBuffer(int index) const {
    return m_swapChain->getFrameBuffer(index);
}

VkExtent2D Application::getExtend2D() const {
    return m_swapChain->getExtend2D();
}

VkPhysicalDevice Application::getPhysicalDevice() const {
    return physicalDevice;
}

int Application::getWidth() const {
    return m_swapChain->getWidth();
}

int Application::getHeight() const {
    return m_swapChain->getHeight();
}

void Application::dispose() {
    cleanup();
}

VkSampleCountFlagBits Application::getmMsaaSamples() const {
    return msaaSamples;
}

VkFormat Application::getSwapChainImageFormat() const {
    return m_swapChain->getSwapChainImageFormat();
}

VkSurfaceKHR Application::getSurface() const {
    return surface;
}

VkQueue Application::getGraphicsQueue() const {
    return graphicsQueue;
}

void Application::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    m_camera->registerForInput();
}

void Application::initVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
        
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createCommandPool();
    createFrameBuffer();

    createTextureImage();

    createGeometryBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createSyncObjects();
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device);
}

void Application::cleanup() {
    m_swapChain->dispose();

    m_standardPipeline->dispose();

    m_renderPass->dispose();

    m_descriptorSceneSet->dispose();
    m_descriptorUiSet->dispose();

    m_descriptorPool->dispose();

    m_texture->dispose();

    m_meshBuffer->dispose();
    m_uiMeshBuffer->dispose();
        
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }

    m_commandManager->dispose();

    vkDestroyDevice(device, nullptr);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT2(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void Application::createGeometryBuffer() {
    std::vector< VertexTextureColor> verts;
    std::vector<uint16_t> indices;

    float scale = 1;
    int floor = -2;
    glm::mat4 transform;

    // ground
    glm::mat3 uvTransform = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0.0f, 0.f, 1.f));
    uvTransform[0].x *= 1.f / 120.f;
    uvTransform[1].y *= 1.f / 60.f;
    uvTransform[2] = glm::vec3(0, 0, 1);
    transform = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
    transform = glm::scale(transform, glm::vec3(50));
    transform[3] = glm::vec4(0, floor, 0, 1);
    PlaneGeoData groundData{ transform, uvTransform };
    PlaneGeo::buildPlan(verts, indices, groundData);

    uvTransform = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0.0f, 0.f, 1.f));
    uvTransform[0].x *= 1.f / 120.f;
    uvTransform[1].y *= 1.f / 60.f;
    uvTransform[2] = glm::vec3(0, 0, 1);

    // some boxes
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            transform = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0));
            transform = glm::scale(transform, glm::vec3(1));
            transform[3] = glm::vec4(i, floor, j, 1);
            BoxGeoData cube1Data{ transform, uvTransform };
            BoxGeo::buildBox(verts, indices, cube1Data);
        }
    }
    m_meshBuffer->createBuffers<VertexTextureColor>(verts, indices);

    // ui
    std::vector< VertexTextureColor> verts2;
    std::vector<uint16_t> indices2;
    glm::mat3 uvTransform2 = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0.0f, 0.f, 1.f));
    uvTransform2[0].x *= 1.f / 120.f;
    uvTransform2[1].y *= 1.f / 60.f;
    uvTransform2[2] = glm::vec3(2.f/120.f, 0, 1);
    transform = glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0));
    transform = glm::scale(transform, glm::vec3(1));
    transform[3] = glm::vec4(0, 0, .5, 1);
    PlaneGeoData uiData{ transform, uvTransform2 };
    PlaneGeo::buildPlan(verts2, indices2, uiData);

    m_uiMeshBuffer->createBuffers<VertexTextureColor>(verts2, indices2);
}

void Application::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);

    m_swapChain->dispose();

    createSwapChain();
    createFrameBuffer();
}

void Application::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void Application::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void Application::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT2(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void Application::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void Application::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            msaaSamples = getMaxUsableSampleCount();
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void Application::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void Application::createSwapChain() {
    m_swapChain->initialize();
    // initailze the camera now that we know the swapChainExtent
    m_camera->initialize();
}

void Application::createFrameBuffer() {
    m_swapChain->createFramebuffers();
}

void Application::createRenderPass() {
    m_renderPass->Initialize();
}

void Application::createDescriptorSetLayout() {
    m_descriptorPool->createDescriptorSetLayout();
}

void Application::createGraphicsPipeline() {
    
    m_standardPipeline->initialize(m_descriptorPool->getDescriptorSetLayoutPtr(), *m_renderPass);
}

void Application::createCommandPool() {
    m_commandManager->createCommandPool();
}

VkFormat Application::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

/**********************************************************************/
VkFormat Application::findDepthFormat() const {
    return findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

/**********************************************************************/
VkSampleCountFlagBits Application::getMsaaSamples() const {
    return msaaSamples;
}

/**********************************************************************/
bool Application::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

/**********************************************************************/
void Application::createTextureImage() {
    m_texture->initialize(TEXTURE_PATH.c_str());
}

/**********************************************************************/
VkSampleCountFlagBits Application::getMaxUsableSampleCount() {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}

/**********************************************************************/
void Application::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    endSingleTimeCommands(commandBuffer);
}

/**********************************************************************/
void Application::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
}


/**********************************************************************/
void Application::createUniformBuffers() {
    m_descriptorSceneSet->createUniformBuffers();
    m_descriptorUiSet->createUniformBuffers();
}

/**********************************************************************/
void Application::createDescriptorPool() {
    m_descriptorPool->createDescriptorPool();
}

/**********************************************************************/
void Application::createDescriptorSets() {
    m_descriptorSceneSet->createDescriptorSets(*m_descriptorPool, *m_texture);
    m_descriptorUiSet->createDescriptorSets(*m_descriptorPool, *m_texture);
}

/**********************************************************************/
VkCommandBuffer Application::beginSingleTimeCommands() const {
    return m_commandManager->beginSingleTimeCommands();
}

/**********************************************************************/
void Application::endSingleTimeCommands(VkCommandBuffer commandBuffer) const {
    m_commandManager->endSingleTimeCommands(commandBuffer);
}

/**********************************************************************/
uint32_t Application::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

/**********************************************************************/
void Application::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_renderPass->getRenderPass();
    renderPassInfo.framebuffer = m_swapChain->getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_swapChain->getExtend2D();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_standardPipeline->getPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)m_swapChain->getWidth();
    viewport.height = (float)m_swapChain->getHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = m_swapChain->getExtend2D();
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    //
    // 3d environment
    //
    vkCmdSetDepthTestEnable(commandBuffer, true);
    VkBuffer vertexBuffers[] = { m_meshBuffer->getVertexBuffer()};
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_meshBuffer->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(
        commandBuffer, 
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        m_standardPipeline->getPipelineLayout(), 
        0, 
        1, 
        m_descriptorSceneSet->getDescriptorSet(m_currentFrame),
        0,
        nullptr);
    vkCmdDrawIndexed(commandBuffer, m_meshBuffer->getIndexCount(), 1, 0, 0, 0);

    //
    // ui commands
    //
    vkCmdSetDepthTestEnable(commandBuffer, false);
    VkBuffer vertexBuffersUi[] = { m_uiMeshBuffer->getVertexBuffer() };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffersUi, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_uiMeshBuffer->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_standardPipeline->getPipelineLayout(),
        0,
        1,
        m_descriptorUiSet->getDescriptorSet(m_currentFrame),
        0,
        nullptr);
    vkCmdDrawIndexed(commandBuffer, m_uiMeshBuffer->getIndexCount(), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

/**********************************************************************/
void Application::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

/**********************************************************************/
void Application::updateUniformBuffer(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    // update the camera
    m_camera->update(time);

    m_descriptorSceneSet->setData(currentImage, &m_camera->getFPSCamera().getUbo());
    m_descriptorUiSet->setData(currentImage, &m_camera->getUiCamera().getUbo());
}

/**********************************************************************/
void Application::drawFrame() {
    vkWaitForFences(device, 1, &inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, m_swapChain->getSwapChain(), UINT64_MAX, imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // update the uniforms
    updateUniformBuffer(m_currentFrame);
    m_scene->updateUniformBuffer(m_currentFrame);

    vkResetFences(device, 1, &inFlightFences[m_currentFrame]);

    //m_scene->recordToCommandBuffers(m_currentFrame, imageIndex);
    vkResetCommandBuffer(m_commandManager->getActiveDrawingCommand(), /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(m_commandManager->getActiveDrawingCommand(), imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[m_currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandManager->getDrawingCommandBuffers()[m_currentFrame];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[m_currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[m_currentFrame]) != VK_SUCCESS) {
        LOGE("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_swapChain->getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

/**********************************************************************/
VkShaderModule Application::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

/**********************************************************************/
bool Application::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = m_swapChain->querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

/**********************************************************************/
bool Application::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

/**********************************************************************/
QueueFamilyIndices Application::findQueueFamilies(VkPhysicalDevice device) const {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

/**********************************************************************/
std::vector<const char*> Application::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

/**********************************************************************/
bool Application::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

/**********************************************************************/
Application* Application::m_instance = nullptr;

/**********************************************************************/
Application::Application() {
   
}

/**********************************************************************/
void Application::create() {
    if (!m_instance) {

        m_instance = new Application();

        m_instance->initialize();
    }
}

/**********************************************************************/
void Application::initialize() {
    _inputManager = std::make_unique<InputManager>();
    m_scene = std::make_unique<GameScene>();
    _inputManager = std::make_unique<InputManager>();

    m_meshBuffer = std::make_unique<MeshBuffer>();
    m_uiMeshBuffer = std::make_unique<MeshBuffer>();

    m_camera = std::make_unique<CameraManager>();
    
    m_renderPass = std::make_unique<RenderPass>();
    m_swapChain = std::make_unique<SwapChain>();
    m_commandManager = std::make_unique<CommandManager>();
    m_texture = std::make_unique<Texture>();
    m_standardPipeline = std::make_unique<StandardGraphicPipeline>();
    m_descriptorPool = std::make_unique<DescriptorPool>();

    m_descriptorSceneSet = std::make_unique<StandardDescriptorSet>();
    m_descriptorUiSet = std::make_unique<StandardDescriptorSet>();

    initWindow();
    initVulkan();
    
    _performance = {};

    // setup glut input
    glfwSetWindowUserPointer(m_instance->getWindow(), m_instance);
    glfwSetKeyCallback(m_instance->getWindow(), onKeyCallback);
    glfwSetCursorPosCallback(m_instance->getWindow(), onCursorPosCallback);
    glfwSetMouseButtonCallback(m_instance->getWindow(), onMouseButtonCallback);

    _inputManager->cursorShowing(false);

    m_scene->initialize();

    // initialize the time
    lastTime = std::chrono::high_resolution_clock::now();
}

/**********************************************************************/
Application& Application::get() {
    return *m_instance;
}

/**********************************************************************/
std::unique_ptr<InputManager>& Application::getInputManager()  {
    return _inputManager;
}

/**********************************************************************/
uint16_t  Application::maxFramesInFlight() const {
    return MAX_FRAMES_IN_FLIGHT;
}

/**********************************************************************/
uint16_t Application::getCurrentFrame() const {
    return m_currentFrame;
}

/**********************************************************************/
void Application::run() {

    // main loop
    while (!glfwWindowShouldClose(getWindow())) {
        _performance.preUpdate();

        // time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;

        // get input and windows events
        glfwPollEvents();

        // main scene update
        m_scene->nonGraphicsUpdate(dt);

        // do the graphics
        drawFrame();

        // performance
        _performance.update(dt);
    }

    vkDeviceWaitIdle(getDevice());

    m_scene->dispose();
    dispose();
}

/**********************************************************************/
CommandManager& Application::getCommandManager() const {
    return *m_commandManager;
}
