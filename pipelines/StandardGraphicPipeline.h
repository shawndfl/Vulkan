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
#include "core/IDisposable.h"

/**
* Create args for the pipeline
*/
typedef struct StandardGraphicPipelineData {
	int empty;
} StandardGraphicPipelineData;

class StandardGraphicPipeline: public IDisposable {

public:
	StandardGraphicPipeline();

    void initialize(const  StandardGraphicPipelineData& data);

    void dispose();
private :
	VkShaderModule createShaderModule(const std::vector<char>& code);

    void createDescriptorSetLayout();

    void createDescriptorPool();

    void createDescriptorSets();

	

private:
    
    VkPipelineLayout pipelineLayout;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPipeline graphicsPipeline;

    //TODO move this somewhere else
    Texture sampleTexture;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};

