#pragma once

#include "OSExport.h"
#include "Window.h"

class ENGINE_API Scene;

class ENGINE_API GameLoop {

public:
	void start();
	void setScene(Scene* scene);
	static Window& getWindow();

private:
	Scene * currentScene;
	
};