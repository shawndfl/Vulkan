#include "GameScene.h"
#include "core/IScene.h"
#include "utilities/Log.h"
#include "systems/FontManager.h"
#include "core/Application.h"

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
const std::vector<VkCommandBuffer>& GameScene::recordToCommandBuffers(uint32_t currentFrame, uint32_t imageIndex) {
	const Application& app = Application::get();
	auto drawingCommand = app.getCommandManager().getActiveDrawingCommand();
	// draw environment
	// draw terrain
	// draw cubes
	// draw NPC
	// draw particles (instanced)
	// draw items (instanced)
	// draw ui images (instanced)
	// draw ui text (dynamic)

	// set up the commands
	//vkResetCommandBuffer(drawingCommand, /*VkCommandBufferResetFlagBits*/ 0);

	//m_cube.record();
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
