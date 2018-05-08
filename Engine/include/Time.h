#pragma once
#include "OSExport.h"

class ENGINE_API Time {

public:
	static double getDeltaTime();

private:
	static void startDeltaTime();
	static void updateDeltaTime();
	friend class GameLoop;
};