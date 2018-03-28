#pragma once
#include <vector>
#include "Typedefs.h"
#include "LinearMath.h"
#include "Texture2D.h"
#include "Mesh.h"

class Loader {

public:
	Loader();
	~Loader();

	static Texture2D loadRGBATexture2D(const char* filepath);
	static Mesh loadSimpleMesh(const char* filepath);
	static Mesh loadMesh(const char* filepath);

private:

};