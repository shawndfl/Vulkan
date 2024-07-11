#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "core/IDisposable.h"
#include "core/Buffer.h"

class DescriptorPool;
class Texture;

class StandardDescriptorSet {
public:
	void dispose();
	void createDescriptorSets(const DescriptorPool& pool, const Texture& texture);
	void createUniformBuffers();

	void setData(uint32_t index, const void* src);
	VkDescriptorSet* getDescriptorSet(uint32_t index) ;
private:
	std::vector<VkDescriptorSet> m_descriptorSets;
	std::vector<Buffer> m_uniformBuffers;

};

