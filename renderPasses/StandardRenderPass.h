#pragma once
#include <vulkan/vulkan.h>
#include "core/IDisposable.h"

class StandardRenderPass: public IDisposable
{
public:
	void Initialize();
	VkRenderPass getRenderPass() const;

	void dispose();
private:
	VkRenderPass m_renderPass;
};

