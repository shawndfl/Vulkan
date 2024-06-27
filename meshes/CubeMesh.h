#pragma once
#include <vulkan/vulkan.h>

class CubeMesh {
public:
	void initialize();
	void record(VkCommandBuffer command);
};

