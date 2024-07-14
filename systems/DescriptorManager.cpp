#include "DescriptorManager.h"

/**********************************************************************/
DescriptorManager::DescriptorManager() {
	m_descriptorPool = std::make_unique<DescriptorPool>();
	m_descriptorSceneSet = std::make_unique<StandardDescriptorSet>();
	m_descriptorUiSet = std::make_unique<StandardDescriptorSet>();
}

/**********************************************************************/
void DescriptorManager::createDescriptorSetLayout() {
	m_descriptorPool->createDescriptorSetLayout();
}

/**********************************************************************/
void DescriptorManager::initialize(const TextureList& textures) {
	m_descriptorPool->createDescriptorPool();
	m_descriptorPool->getDescriptorSetLayoutPtr();

	m_descriptorSceneSet->createUniformBuffers();
	m_descriptorUiSet->createUniformBuffers();

	m_descriptorSceneSet->createDescriptorSets(*m_descriptorPool, textures.albedo);
	m_descriptorUiSet->createDescriptorSets(*m_descriptorPool, textures.albedo);
}

/**********************************************************************/
void DescriptorManager::dispose() {
	m_descriptorSceneSet->dispose();
	m_descriptorUiSet->dispose();

	m_descriptorPool->dispose();
}

/**********************************************************************/
VkDescriptorSetLayout* DescriptorManager::getDescriptorSetLayoutPtr() {
	return m_descriptorPool->getDescriptorSetLayoutPtr();
}

/**********************************************************************/
StandardDescriptorSet& DescriptorManager::getDescriptorSceneSet() {
	return *m_descriptorSceneSet;
}

/**********************************************************************/
StandardDescriptorSet& DescriptorManager::getDescriptorUiSet() {
	return *m_descriptorUiSet;
}

/**********************************************************************/
DescriptorPool& DescriptorManager::getDescriptorPool() {
	return *m_descriptorPool;
}
