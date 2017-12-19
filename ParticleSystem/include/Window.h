#pragma once
#include "Rect.h"
#include "OSExport.h"

struct SDL_Window;

class ENGINE_API Window {

public:
	Window();
	void start();
	void swapBuffers();
	void setViewport(Rect viewport);
	Rect getViewport();
	void finish();
	void pollEvents();
	bool isOpen();
	
private:
	Rect mViewport;
	SDL_Window* mWindow;
	bool mIsOpen;
};