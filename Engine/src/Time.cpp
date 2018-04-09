#include "Typedefs.h"
#include "Time.h"
#include <iostream>
#include <chrono>

using ms = std::chrono::duration<double, std::milli>;
std::chrono::high_resolution_clock timer;
double mDeltaTime;
std::chrono::time_point<std::chrono::steady_clock> NOW, LAST;

double Time::getDeltaTime()
{
	return mDeltaTime;
}

void Time::startDeltaTime()
{
	NOW = std::chrono::high_resolution_clock::now();
}

void Time::updateDeltaTime()
{
	LAST = NOW;
	NOW = NOW = std::chrono::high_resolution_clock::now();
	mDeltaTime = std::chrono::duration_cast<ms>(NOW - LAST).count() / 1000;
}
