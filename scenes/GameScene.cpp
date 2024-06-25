#include "GameScene.h"
#include "core/IScene.h"
#include "pipelines/StandardGraphicPipeline.h"
#include "utilities/Log.h"

GameScene::GameScene() {
	_image = std::make_unique<ImagePipeline>();
}

/**********************************************************************/
void GameScene::initialize() {
	LOGI("initializing the scene");
	_image->initialize();
	//StandardGraphicPipelineData data;
	//StandardGraphicPipeline pipeline(data);

}

/**********************************************************************/
void GameScene::render(float dt) {
	_image->update(dt);
	//LOGI("update " << dt);
}

/**********************************************************************/
void GameScene::dispose() {

}
