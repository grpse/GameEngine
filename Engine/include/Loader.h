#pragma once
#include "OSExport.h"

class Texture2D;
class Mesh;

class ENGINE_API Loader {

public:
	Loader();
	~Loader();

	static Texture2D loadRGBATexture2D(const char* filepath);
	static Mesh loadSimpleMesh(const char* filepath);

private:

};