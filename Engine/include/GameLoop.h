#pragma once

#include "OSExport.h"
#include "Window.h"
class Scene;

class ENGINE_API GameLoop {

public:
	void start();
	void setScene(Scene* scene);

private:
	Scene * mCurrentScene;
};