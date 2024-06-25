#pragma once
#include <memory>
#include "core/IScene.h"
#include "pipelines/ImagePipeline.h"
#include "core/IDisposable.h"

class ImagePipeline;

class GameScene: public IScene, public IDisposable {
private:
	std::unique_ptr<ImagePipeline> _image;

public:
	GameScene(); 
	void initialize();

	void render(float dt);

	void dispose();
};

