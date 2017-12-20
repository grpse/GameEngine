#include "Time.h"

double mDeltaTime;

double Time::getDeltaTime()
{
	return mDeltaTime;
}

void Time::setDeltaTime(double delta)
{
	mDeltaTime = delta;
}
