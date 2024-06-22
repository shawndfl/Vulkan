#include "InputManager.h"
#include "utilities/Log.h"
#include "core/Application.h"


InputManager::InputManager() {
	_showingCursor = true;
}

/**********************************************************************/
void InputManager::registerKeyboardHandler(const std::string& id, std::function<void( int key, int scancode, int action, int mods)> fn) {
	keyHandlers[id] = fn;
}

/**********************************************************************/
void InputManager::registerMouseHandler(const std::string& id, std::function<void(int button, int action, int mods, int x, int y)> fn) {
	mouseHandlers[id] = fn;
}

/**********************************************************************/
void InputManager::cursorShowing(bool show) {
	_showingCursor = show;
	const Application& app = Application::get();
	glfwSetInputMode(Application::get().getWindow(), GLFW_CURSOR, show ?GLFW_CURSOR_NORMAL: GLFW_CURSOR_DISABLED);
}

/**********************************************************************/
void InputManager::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//LOGI("key down " << key << ", " << scancode << ", " << action << ", " << mods);

	// toggle showing and hiding the curso
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		cursorShowing(!_showingCursor);
	}

	for (auto pair : keyHandlers) {
		pair.second(key, scancode, action, mods);
	}
}

/**********************************************************************/
void InputManager::onCursorPos(GLFWwindow* window, int xpos, int ypos) {
	lastX = xpos;
	lastY = ypos;
	for (auto pair : mouseHandlers) {
		pair.second(lastMouseButton, lastmouseAction, lastMouseMods, lastX, lastY);
	}
}

/**********************************************************************/
void InputManager::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
	//LOGI("mouse button " << button << ", " << action << ", " << mods);
	lastMouseButton = button;
	lastmouseAction = action;
	lastMouseMods = mods;
	for (auto pair : mouseHandlers) {
		pair.second( lastMouseButton, lastmouseAction, lastMouseMods, lastX, lastY);
	}
}
