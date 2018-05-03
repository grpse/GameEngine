#pragma once
#ifndef USE_GLFW
	//#define USE_GLFW
#endif
class Time {

public:
	static double getDeltaTime();

private:
	static void startDeltaTime();
	static void updateDeltaTime();
	friend class GameLoop;
};