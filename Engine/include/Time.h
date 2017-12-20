#pragma once

class Time {

public:
	static double getDeltaTime();

private:
	static void setDeltaTime(double delta);

	friend class GameLoop;
};