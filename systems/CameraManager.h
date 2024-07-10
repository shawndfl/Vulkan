#pragma once
#include "core/ISystemManager.h"
#include <vulkan/vulkan.h>
#include <vector>

class CameraFPS;
class CameraUi;

class CameraManager : public ISystemManager {
public:
	CameraManager();
	void initialize();

	void registerForInput();

	void update(float dt);
	void dispose();
	CameraFPS& getFPSCamera();
	CameraUi& getUiCamera();
private :
	std::unique_ptr<CameraFPS> m_camera;
	std::unique_ptr<CameraUi> m_cameraUi;
};

