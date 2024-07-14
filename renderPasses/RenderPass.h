#pragma once
#include <vulkan/vulkan.h>
#include "core/IDisposable.h"

class RenderPass: public IDisposable
{
public:
	void Initialize();
	VkRenderPass getVkRenderPass() const;

	void dispose();
private:
	VkRenderPass m_renderPass;
};

