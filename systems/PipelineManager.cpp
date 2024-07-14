#include "PipelineManager.h"
#include "core/Application.h"

PipelineManager::PipelineManager() {
	m_standardPipeline = std::make_unique<StandardGraphicPipeline>();
}

void PipelineManager::initialize() {
	dispose();
	Application& app = Application::get();

	m_standardPipeline->initialize(
		app.getDescriptorManager().getDescriptorSetLayoutPtr(),
		app.getRenderPassManager().getRenderPass());
	//m_standardPipeline->initialize(m_descriptorPool->getDescriptorSetLayoutPtr(), *m_renderPass);
}

void PipelineManager::dispose() {
	m_standardPipeline->dispose();
}

StandardGraphicPipeline& PipelineManager::getGraphicPipeline() {
	return *m_standardPipeline;
}
