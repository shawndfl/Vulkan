#pragma once
#include "utilities/Log.h"

struct Performance {
	float time;
	unsigned int drawCalls;
	unsigned int frameCount;
	float fps;

	Performance() {
		time = 0;
		drawCalls = 0;
		frameCount = 0;
		fps = 0;
	}

	void addDrawCall(int calls = 1) {
		drawCalls += calls;
	}

	void preUpdate() {
		drawCalls = 0;
	}
	  
	void update(float dt) {
		time += dt;
		frameCount++;

		if (time > 3) {
			fps = frameCount /(float) time;
			LOGI("FPS " << fps << " frames " << frameCount << " dt " << dt);
			time = 0;
			frameCount = 0;
		}
	}
};

