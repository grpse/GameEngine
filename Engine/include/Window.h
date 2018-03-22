#pragma once
#include <functional>
#include <vector>
#include "Rect.h"
#include "OSExport.h"
#include "Typedefs.h"

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

	void onKeydown(const std::function<void(uint key)>& keydownListener);
	void onMouseMove(const std::function<void(int x, int y)>& mousemoveListener);

private:
	Rect mViewport;
	SDL_Window* mWindow;
	bool mIsOpen;

	std::vector< std::function<void(uint key)> > mKeydownListeners;
	std::vector< std::function<void(int x, int y)> > mMouseMoveListeners;
};