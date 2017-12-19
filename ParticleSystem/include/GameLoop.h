#pragma once

#include "OSExport.h"
#include "Window.h"

class ENGINE_API GameLoop {

public:
	static void start();
	static void update();
	static const float Gravity();
		
};