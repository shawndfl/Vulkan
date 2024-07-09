#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>
#include <chrono>
#include <optional>

#include "core/IScene.h"
#include "utilities/Performance.h"
#include "core/MeshBuffer.h"
#include "systems/CommandManager.h"
#include "systems/DescriptorManager.h"
#include "systems/FontManager.h"
#include "systems/InputManager.h"
#include "systems/PipelineManager.h"
#include "systems/RenderPassManager.h"
#include "systems/TextureManager.h"


#include "core/SwapChain.h"
#include "core/DeviceImage.h"
#include "renderPasses/RenderPass.h"
#include "core/Texture.h"

class StandardGraphicPipeline;
class DescriptorPool;
class StandardDescriptorSet;
class CameraUi;
class CameraFPS;

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

/**
* Main application that runs everything
*/
class Application {
public:
	/**
	* Create the only instance of this application
	*/
	static void create();

	/**
	* Get the only instance of this applicaiton
	*/
	static Application& get();

    /**
    * Initialize the application. Create the window, initialize Vulkan, initialize systems
    */
    void initialize();
	
	std::unique_ptr<InputManager>& getInputManager();

	GLFWwindow* getWindow() const;

	VkDevice getDevice() const;

	VkPhysicalDevice getPhysicalDevice() const;

	VkRenderPass getRenderPass() const;

	uint16_t maxFramesInFlight() const;

    void dispose();

    VkSampleCountFlagBits getmMsaaSamples() const;

	int getWidth() const;

	int getHeight() const;

	VkFormat findDepthFormat() const;

	VkSampleCountFlagBits getMsaaSamples() const;

	VkFormat getSwapChainImageFormat() const;

	VkSurfaceKHR getSurface() const;

	VkQueue getGraphicsQueue() const;

    uint16_t getCurrentFrame() const;

	/**
	* Creates a buffers used for images, verteices, indices, and other things in video memory
	*/
	void createBuffer(VkDeviceSize size, 
		VkBufferUsageFlags usage, 
		VkMemoryPropertyFlags properties, 
		VkBuffer& buffer, 
		VkDeviceMemory& bufferMemory) const;

	/**
	* Finds a memory type for a buffer
	*/
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

	void run();

    CommandManager& getCommandManager() const;

    VkCommandBuffer beginSingleTimeCommands() const;

    void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

    VkFramebuffer getFrameBuffer(int index) const;

    VkExtent2D getExtend2D() const;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
private:
    /**
    * Singletop use get()
    */
	Application();

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

    void createGeometryBuffer();

    void recreateSwapChain();
    /**
    * Create the vulkan instance
    */
    void createInstance();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    void setupDebugMessenger();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSwapChain();

    void createFrameBuffer();

    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipeline();

    void createCommandPool();

    bool hasStencilComponent(VkFormat format);

    void createTextureImage();

    VkSampleCountFlagBits getMaxUsableSampleCount();

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createUniformBuffers();

    void createDescriptorPool();

    void createDescriptorSets();

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();

    void updateUniformBuffer(uint32_t currentImage);

    void drawFrame();

    VkShaderModule createShaderModule(const std::vector<char>& code);

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

    bool checkValidationLayerSupport();

    // callbacks
    static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void onCursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType, 
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
    /**
    * Single instance of the class
    */
    static Application* m_instance;

    std::unique_ptr<InputManager> _inputManager;
    std::unique_ptr<MeshBuffer> m_meshBuffer;
    std::unique_ptr<MeshBuffer> m_uiMeshBuffer;
    std::unique_ptr<CameraFPS> m_camera;
    std::unique_ptr<CameraUi> m_cameraUi;
    std::unique_ptr<RenderPass> m_renderPass;
    std::unique_ptr<SwapChain> m_swapChain;
    std::unique_ptr<IScene> m_scene;
    std::unique_ptr<CommandManager> m_commandManager;
    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<StandardGraphicPipeline> m_standardPipeline;
    std::unique_ptr<DescriptorPool> m_descriptorPool;
    std::unique_ptr<StandardDescriptorSet> m_descriptorSceneSet;
    std::unique_ptr<StandardDescriptorSet> m_descriptorUiSet;


    struct Performance _performance;

    std::chrono::steady_clock::time_point lastTime;

    GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t m_currentFrame = 0;

    bool framebufferResized = false;
};
