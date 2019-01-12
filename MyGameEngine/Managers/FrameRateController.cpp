#include "FrameRateController.h"
#include "GLFW/glfw3.h"

#include "stdio.h"

FrameRateController::FrameRateController(unsigned int MaxFrameRate)
{
	mTickEnd = mTickStart = mFrameTime = 0.0;

	if (0 != MaxFrameRate) {
		mNeededTicksPerFrame = 1.0 / MaxFrameRate;
	}
	else
		mNeededTicksPerFrame = 0.0;
}


FrameRateController::~FrameRateController()
{
}

void FrameRateController::FrameStart() {
	mTickStart = glfwGetTime();
	//printf("mTickStart : %lf\n", mTickStart);
}

void FrameRateController::FrameEnd() {
	mTickEnd = glfwGetTime();
	//printf("mTickEnd : %lf\n", mTickEnd);

	while (mTickEnd - mTickStart < mNeededTicksPerFrame) {
		mTickEnd = glfwGetTime();
	}

	mFrameTime = mTickEnd - mTickStart;
}

double FrameRateController::GetFrameTime() {
	return mFrameTime;
}