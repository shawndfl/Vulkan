#pragma once

#include <GLFW/glfw3.h>

class InputManager {
private:
	bool _showingCursor;

public:
	InputManager();

	void cursorShowing(bool show);

	void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

	void onCursorPos(GLFWwindow* window, int xpos, int ypos);

	void onMouseButton(GLFWwindow* window, int button, int action, int mods);
};

