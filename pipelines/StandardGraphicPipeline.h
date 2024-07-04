#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include "core/Texture.h"
#include "core/IDisposable.h"

class RenderPass;

class StandardGraphicPipeline: public IDisposable {

public:
	StandardGraphicPipeline();

    void initialize(VkDescriptorSetLayout* descriptorSetLayout, const RenderPass& renderPass);

    VkPipeline getPipeline() const;

    VkPipelineLayout getPipelineLayout() const;

    void dispose();
private :
	VkShaderModule createShaderModule(const std::vector<char>& code);

private:
    
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
};

