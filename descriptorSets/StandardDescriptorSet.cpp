#include "StandardDescriptorSet.h"
#include "core/Application.h"
#include "descriptorSets/DescriptorPool.h"
#include "core/Texture.h"
#include "descriptorSets/UniformBufferObject.h"
#include <array>

/**********************************************************************/
void StandardDescriptorSet::dispose() {
    const Application& app = Application::get();
    auto device = app.getDevice();

    for (size_t i = 0; i < app.maxFramesInFlight(); i++) {
        vkDestroyBuffer(device, m_uniformBuffers[i], nullptr);
        vkFreeMemory(device, m_uniformBuffersMemory[i], nullptr);
    }
}

/**********************************************************************/
void StandardDescriptorSet::createUniformBuffers() {
    const Application& app = Application::get();
    auto device = app.getDevice();

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_uniformBuffers.resize(app.maxFramesInFlight());
    m_uniformBuffersMemory.resize(app.maxFramesInFlight());
    m_uniformBuffersMapped.resize(app.maxFramesInFlight());

    for (size_t i = 0; i < app.maxFramesInFlight(); i++) {
        app.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);

        vkMapMemory(device, m_uniformBuffersMemory[i], 0, bufferSize, 0, &m_uniformBuffersMapped[i]);
    }
}

/**********************************************************************/
void StandardDescriptorSet::createDescriptorSets(const DescriptorPool& pool, const Texture& texture) {
    const Application& app = Application::get();
    auto device = app.getDevice();

    std::vector<VkDescriptorSetLayout> layouts(app.maxFramesInFlight(), pool.getDescriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool.getDescriptorPool();
    allocInfo.descriptorSetCount = static_cast<uint32_t>(app.maxFramesInFlight());
    allocInfo.pSetLayouts = layouts.data();

    m_descriptorSets.resize(app.maxFramesInFlight());
    if (vkAllocateDescriptorSets(device, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
        LOGE("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < app.maxFramesInFlight(); i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture.getTextureImageView();
        imageInfo.sampler = texture.getTextureSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = m_descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = m_descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

/**********************************************************************/
void* StandardDescriptorSet::getUniformBuffersMapped(uint32_t index) const {
    return m_uniformBuffersMapped[index];
}

/**********************************************************************/
VkDescriptorSet* StandardDescriptorSet::getDescriptorSet(uint32_t index){
    return &m_descriptorSets[index];
}
