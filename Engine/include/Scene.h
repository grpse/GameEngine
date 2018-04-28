#pragma once
#include "OSExport.h"

class ENGINE_API Scene {
public:
	virtual void start() = 0;
	virtual void onGUI() = 0;
	virtual void update(float dt) = 0;
};