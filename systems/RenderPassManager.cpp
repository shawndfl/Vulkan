#include "RenderPassManager.h"
#include "core/Application.h"

/**********************************************************************/
RenderPassManager::RenderPassManager() {
	m_renderPass = std::make_unique<RenderPass>();
}

/**********************************************************************/
void RenderPassManager::initialize() {
	const Application& app = Application::get();
	m_renderPass->Initialize();
}

/**********************************************************************/
RenderPass& RenderPassManager::getRenderPass() {
	return *m_renderPass;
}

/**********************************************************************/
void RenderPassManager::dispose() {
	m_renderPass->dispose();
}
