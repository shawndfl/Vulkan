#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "core/ISystemManager.h"

/**
* Manages drawing text on the screen
*/
class FontManager: public ISystemManager {
public:
	void initialize();
	void addText(const std::string& text, int x, int y);
	void update(float dt);
	void dispose();
private:
	void recordCommands();
};

