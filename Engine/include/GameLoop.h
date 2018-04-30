#pragma once

#include "OSExport.h"
#include "Window.h"
class Scene;

class ENGINE_API GameLoop {

public:
	GameLoop();
	void start();
	void setScene(Scene* scene);
	inline Window& getWindow() { return mWindow; }

private:
	Scene * mCurrentScene;
	Window mWindow;
	bool mWindowStarted;
};