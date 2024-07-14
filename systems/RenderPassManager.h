#pragma once
#include "renderPasses/RenderPass.h"
#include "core/ISystemManager.h"

class RenderPassManager: public ISystemManager {
public:
	RenderPassManager();

	void initialize();

	RenderPass& getRenderPass();

	void dispose();
private:
	std::unique_ptr<RenderPass> m_renderPass;
};

