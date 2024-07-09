#include "GameScene.h"
#include "core/IScene.h"
#include "utilities/Log.h"
#include "systems/FontManager.h"
#include "core/Application.h"
#include<array>

/**********************************************************************/
GameScene::GameScene() {
	m_font = std::make_unique<FontManager>();
}

/**********************************************************************/
void GameScene::initialize() {
	LOGI("initializing the scene");
	m_font->initialize();
	m_cube.initialize();
}

/**********************************************************************/
void GameScene::updateUniformBuffer(uint32_t currentFrame) {

}

/**********************************************************************/
const std::vector<VkCommandBuffer>& GameScene::recordToCommandBuffers2(uint32_t currentFrame, uint32_t imageIndex) {
   
	const Application& app = Application::get();
	auto drawingCommand = app.getCommandManager().getActiveDrawingCommand();
    /*
	// draw environment
	// draw terrain
	// draw cube_blocks ( collection of 16x16X256 cubes )
	// draw NPC
	// draw particles (instanced)
	// draw items (instanced)
	// draw ui images (instanced)
	// draw ui text (dynamic)

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(drawingCommand, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = app.getRenderPass();
    renderPassInfo.framebuffer = app.getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = app.getExtend2D();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_standardPipeline->getPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)m_swapChain->getWidth();
    viewport.height = (float)m_swapChain->getHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = m_swapChain->getExtend2D();
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    //
    // 3d environment
    //
    vkCmdSetDepthTestEnable(commandBuffer, true);
    VkBuffer vertexBuffers[] = { m_meshBuffer->getVertexBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_meshBuffer->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_standardPipeline->getPipelineLayout(),
        0,
        1,
        m_descriptorSceneSet->getDescriptorSet(m_currentFrame),
        0,
        nullptr);
    vkCmdDrawIndexed(commandBuffer, m_meshBuffer->getIndexCount(), 1, 0, 0, 0);

    //
    // ui commands
    //
    vkCmdSetDepthTestEnable(commandBuffer, false);
    VkBuffer vertexBuffersUi[] = { m_uiMeshBuffer->getVertexBuffer() };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffersUi, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_uiMeshBuffer->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_standardPipeline->getPipelineLayout(),
        0,
        1,
        m_descriptorUiSet->getDescriptorSet(m_currentFrame),
        0,
        nullptr);
    vkCmdDrawIndexed(commandBuffer, m_uiMeshBuffer->getIndexCount(), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
    */
	return m_commandBuffers;
    
}

/**********************************************************************/
void GameScene::nonGraphicsUpdate(float dt) {
	m_font->update(dt);
}

/**********************************************************************/
void GameScene::dispose() {
	m_font->dispose();
}
