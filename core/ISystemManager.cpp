#include "ISystemManager.h"
#include "Application.h"

/**********************************************************************/
ISystemManager::ISystemManager() {

}

/**********************************************************************/
const Application& ISystemManager::app() {
	return Application::get();
}