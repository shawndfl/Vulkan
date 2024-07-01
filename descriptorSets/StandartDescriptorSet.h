#pragma once
#include <vector>
#include <vulkan/vulkan.h>

class StandartDescriptorSet {
public:
	void initialize();

private:
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

};

