#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "core/IDisposable.h"

class Texture;

class DescriptorPoolManager : public IDisposable  {
public:
	void createDescriptorPool();
	void createDescriptorSets(const std::vector<VkBuffer>& uniformBuffers, const Texture& texture);

	VkDescriptorPool getDescriptorPool() const;
	VkDescriptorSetLayout* getDescriptorSetLayout();
	const std::vector<VkDescriptorSet>& getdescriptorSets() const;

	void createDescriptorSetLayout();

	void dispose();
private:

	VkDescriptorPool m_descriptorPool;
	VkDescriptorSetLayout m_descriptorSetLayout;
	std::vector<VkDescriptorSet> m_descriptorSets;
};

