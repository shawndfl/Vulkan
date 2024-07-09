#pragma once
#include <memory>
#include "core/IScene.h"
#include "core/IDisposable.h"
#include "meshes/CubeMesh.h"

class FontManager;

class GameScene: public IScene, public IDisposable {

public:
	GameScene();

	void initialize();

	/**
	* Update uniform buffers
	*/
	void updateUniformBuffer(uint32_t currentFrame);

	const std::vector<VkCommandBuffer>& recordToCommandBuffers2(uint32_t currentFrame, uint32_t imageIndex);

	/**
	* This is called before fencing so you can run application side logic but don't update
	* commandBuffers or Uniforms or textures.
	*/
	void nonGraphicsUpdate(float dt);

	void dispose();
private:
	std::unique_ptr<FontManager> m_font;
	std::vector<VkCommandBuffer> m_commandBuffers;

	CubeMesh m_cube;
};

