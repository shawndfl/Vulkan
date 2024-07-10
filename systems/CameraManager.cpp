#include "CameraManager.h"
#include "cameras/CameraFPS.h"
#include "cameras/CameraUi.h"

/**********************************************************************/
CameraManager::CameraManager() {
	m_camera = std::make_unique<CameraFPS>();
	m_cameraUi = std::make_unique<CameraUi>();
}

/**********************************************************************/
void CameraManager::initialize() {
	m_camera->initialize();
	m_cameraUi->initialize();
}

/**********************************************************************/
void CameraManager::registerForInput() {
	m_camera->registerForInput();
}

/**********************************************************************/
void CameraManager::update(float dt) {
	m_camera->update(dt);
	m_cameraUi->update(dt);
}

/**********************************************************************/
void CameraManager::dispose() {
}

/**********************************************************************/
CameraFPS& CameraManager::getFPSCamera() {
	return *m_camera;
}

/**********************************************************************/
CameraUi& CameraManager::getUiCamera() {
	return *m_cameraUi;
}
