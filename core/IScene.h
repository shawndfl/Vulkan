#pragma once
#include <vector>
#include <vulkan/vulkan.h>

/**
* Manages a scene lifecycle
*/
class IScene {
protected:
	IScene() {}
public:

	/**
	* Initialize the scene
	*/
	virtual void initialize() = 0;

	/**
	* Update uniform buffers 
	*/
	virtual void updateUniformBuffer(uint32_t currentFrame) = 0;

	/**
	* Record to the command buffers
	*/
	virtual const std::vector<VkCommandBuffer>& recordToCommandBuffers2(uint32_t currentFrame, uint32_t imageIndex) = 0;

	/**
	* This is called before fencing so you can run application side logic but don't update 
	* commandBuffers or Uniforms or textures.
	*/
	virtual void nonGraphicsUpdate(float dt) = 0;

	virtual void dispose() = 0;
};