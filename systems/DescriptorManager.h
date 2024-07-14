#pragma once
#include "core/ISystemManager.h"
#include "descriptorSets/DescriptorPool.h"
#include "descriptorSets/StandardDescriptorSet.h"
#include "core/Texture.h"
#include <map>
#include <string>



struct TextureList {
	//PBR textures
	const Texture& albedo;
	const Texture& normal;
	const Texture& metallic;
	const Texture& roughness;
	const Texture& ao;
	const Texture& environment;
};

class DescriptorManager: public ISystemManager {
public: 
	DescriptorManager();
	void createDescriptorSetLayout();
	void initialize(const TextureList& textures);
	void dispose();

	VkDescriptorSetLayout* getDescriptorSetLayoutPtr();
	StandardDescriptorSet& getDescriptorSceneSet();
	StandardDescriptorSet& getDescriptorUiSet();
	DescriptorPool& getDescriptorPool();

private:
	std::unique_ptr<StandardDescriptorSet> m_descriptorSceneSet;
	std::unique_ptr<StandardDescriptorSet> m_descriptorUiSet;
	std::unique_ptr<DescriptorPool> m_descriptorPool;
};

