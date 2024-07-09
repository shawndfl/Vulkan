#pragma once
#include "core/ISystemManager.h"

#include <GLFW/glfw3.h>
#include <functional>
#include <vector>
#include <map>
#include <string>

/**
* Manages input routing. Components can register to receive events raised to the
* InputManager
*/
class InputManager : public ISystemManager {
public:
	InputManager();

	void registerKeyboardHandler(const std::string& id, std::function<void(int key, int scancode, int action, int mods)> fn);

	void registerMouseHandler(const std::string& id, std::function<void(int button, int action, int mods, int x, int y)> fn);

	void cursorShowing(bool show);

	void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

	void onCursorPos(GLFWwindow* window, int xpos, int ypos);

	void onMouseButton(GLFWwindow* window, int button, int action, int mods);

	void dispose();
private:
	bool m_showingCursor;
	std::map<std::string, std::function<void(int key, int scancode, int action, int mods)>> keyHandlers;
	std::map<std::string, std::function<void(int button, int action, int mods, int x, int y)>> mouseHandlers;
	int lastX;
	int lastY;
	int lastMouseButton;
	int lastmouseAction;
	int lastMouseMods;

};

