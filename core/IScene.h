#pragma once

/**
* Manages a scene lifecycle
*/
class IScene {
protected:
	IScene() {}
public:

	virtual void initialize() = 0;

	virtual void render(float dt) = 0;

	virtual void dispose() = 0;
};