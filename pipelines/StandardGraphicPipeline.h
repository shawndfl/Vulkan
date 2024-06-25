#pragma once
#include <vulkan/vulkan.h>
#include "core/Application.h"
#include "core/IGraphicPipeline.h"
#include <vector>
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include "core/Texture.h"


/**
* Create args for the pipeline
*/
typedef struct StandardGraphicPipelineData {
	int empty;
} StandardGraphicPipelineData;

class StandardGraphicPipeline {
private:
    Texture sampleTexture;
	VkPipelineLayout pipelineLayout;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipeline graphicsPipeline;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

public:
	StandardGraphicPipeline(const StandardGraphicPipelineData& data);

private :
	VkShaderModule createShaderModule(const std::vector<char>& code);

    void createDescriptorSetLayout();

    void createDescriptorPool();

    void createDescriptorSets();

	void initialize(const  StandardGraphicPipelineData& data);
};

