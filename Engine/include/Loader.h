#pragma once
#include <vector>
#include "Typedefs.h"
#include "LinearMath.h"
#include "Texture2D.h"

class Loader {

public:
	Loader();
	~Loader();

	static Texture2D loadRGBATexture2D(const char* filepath);

private:

};