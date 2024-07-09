#pragma once
#include <memory>
#include "core/IDisposable.h"

class Application;

/**
* Interface for system manager
*/
class ISystemManager : public IDisposable {
public: 
	const Application& app();

protected:
	ISystemManager();
};

