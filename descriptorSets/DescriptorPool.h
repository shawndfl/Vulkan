#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "core/IDisposable.h"

class Texture;

class DescriptorPool : public IDisposable  {
public:
	void createDescriptorPool();

	VkDescriptorPool getDescriptorPool() const;
	VkDescriptorSetLayout getDescriptorSetLayout() const;
	VkDescriptorSetLayout* getDescriptorSetLayoutPtr();

	void createDescriptorSetLayout();

	void dispose();
private:

	VkDescriptorPool m_descriptorPool;
	VkDescriptorSetLayout m_descriptorSetLayout;
};

