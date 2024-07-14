#pragma once
#include "core/ISystemManager.h"
#include "pipelines/StandardGraphicPipeline.h"
#include <vulkan/vulkan.h>
#include <memory>

class PipelineManager : public ISystemManager {
public:
	PipelineManager();

	void initialize();
	void dispose();

	StandardGraphicPipeline& getGraphicPipeline();
private:
	std::unique_ptr<StandardGraphicPipeline> m_standardPipeline;
};

