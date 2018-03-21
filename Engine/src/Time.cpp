#include "Time.h"
#include <SDL.h>

double mDeltaTime;
Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTimeInSecondsFraction = 0;
double mElapsedSecond = 0;


double Time::getDeltaTime()
{
	return mDeltaTime;
}

void Time::start()
{
	NOW = SDL_GetPerformanceCounter();
}

void Time::updateDeltaTime()
{	
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();

	deltaTimeInSecondsFraction = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency()) / 1000;
	setDeltaTime(deltaTimeInSecondsFraction);
}

void Time::setDeltaTime(double delta)
{
	mDeltaTime = delta;
}
