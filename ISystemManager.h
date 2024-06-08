#pragma once
#include <memory>
#include <core/Application.h>

class ISystemManager {
public: 
	const std::unique_ptr<Application>& get();
};

