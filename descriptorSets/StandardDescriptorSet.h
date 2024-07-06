#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "core/IDisposable.h"

class DescriptorPool;
class Texture;

class StandardDescriptorSet {
public:
	void dispose();
	void createDescriptorSets(const DescriptorPool& pool, const Texture& texture);
	void createUniformBuffers();

	void* getUniformBuffersMapped(uint32_t index) const;
	VkDescriptorSet* getDescriptorSet(uint32_t index) ;
private:
	std::vector<VkDescriptorSet> m_descriptorSets;
	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*> m_uniformBuffersMapped;

};

