#pragma once
#include "OSExport.h"

class Window;
class ShadowRenderer;

class ENGINE_API Locator {
public:	
	inline static void provideWindow(Window* window)
	{
		mWindow = window;
	}

	inline static Window* locateWindow()
	{
		return mWindow;
	}

	inline static void provideShadowRenderer(ShadowRenderer* shadowRenderer)
	{
		mShadowRenderer = shadowRenderer;
	}

	inline static ShadowRenderer* locateShadowRenderer()
	{
		return mShadowRenderer;
	}

private:
	static Window* mWindow;
	static ShadowRenderer* mShadowRenderer;
};