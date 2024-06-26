#include "GameScene.h"
#include "core/IScene.h"
#include "utilities/Log.h"
#include "systems/FontManager.h"

/**********************************************************************/
GameScene::GameScene() {
	m_font = std::make_unique<FontManager>();
}

/**********************************************************************/
void GameScene::initialize() {
	LOGI("initializing the scene");
	m_font->initialize();
}

/**********************************************************************/
void GameScene::update(float dt) {
	m_font->update(dt);
}

/**********************************************************************/
void GameScene::dispose() {
	m_font->dispose();
}
