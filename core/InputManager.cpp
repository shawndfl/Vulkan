#include "InputManager.h"
#include "utilities/Log.h"
#include "core/Application.h"

InputManager::InputManager() {
	_showingCursor = true;
}

/**********************************************************************/
void InputManager::cursorShowing(bool show) {
	_showingCursor = show;
	const Application& app = Application::get();
	glfwSetInputMode(app.getWindow(), GLFW_CURSOR, show ?GLFW_CURSOR_NORMAL: GLFW_CURSOR_HIDDEN);
}

/**********************************************************************/
void InputManager::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	LOGI("key down " << key << ", " << scancode << ", " << action << ", " << mods);

	// toggle showing and hiding the curso
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		cursorShowing(!_showingCursor);
	}
}

/**********************************************************************/
void InputManager::onCursorPos(GLFWwindow* window, int xpos, int ypos) {
	LOGI("cursor pos " << xpos << ", " << ypos);
}

/**********************************************************************/
void InputManager::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
	LOGI("mouse button " << button << ", " << action << ", " << mods);
}
