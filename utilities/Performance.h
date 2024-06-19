#pragma once
struct Performance {
	float dt;
	unsigned int drawCalls;
	unsigned int frameCount;

	Performance() {
		dt = 0;
		drawCalls = 0;
		frameCount = 0;
	}
};

