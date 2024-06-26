#pragma once
#include <memory>
#include "core/IScene.h"
#include "core/IDisposable.h"

class FontManager;

class GameScene: public IScene, public IDisposable {
private:
	std::unique_ptr<FontManager> m_font;

public:
	GameScene();

	void initialize();

	void update(float dt);

	void dispose();
};

