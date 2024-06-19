#pragma once

/**
* Manages a scene lifecycle
*/
class IScene {
protected:
	IScene() {}
public:

	virtual void initialize() = 0;

	virtual void render(float ms) = 0;

	virtual void preUpdate(float ms) {}

	virtual void postUpdate(float ms){}

	virtual void destroy() = 0;
};