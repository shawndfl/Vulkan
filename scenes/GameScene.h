#pragma once
#include "core/IScene.h"
class GameScene: public IScene {
public:
	void initialize();

	void render(float dt);

	void dispose();
};

