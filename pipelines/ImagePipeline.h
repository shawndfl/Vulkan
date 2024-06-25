#pragma once
#include "core/Texture.h"
#include "geometry/PlaneGeo.h"
#include "core/GeometryBuffer.h"
#include "cameras/CameraFPS.h"

/*
* TODO add shader, uniform for camera matrices, buffers, quad geometry, texture, setup pipline
* 
*/


class ImagePipeline {
private:
	Texture _texture;
	VkPipeline graphicsPipeline;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
public: 
	void initialize();

	void update(float dt);

private:
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createGraphicsPipeline();
	void createDescriptorSetLayout();
};

