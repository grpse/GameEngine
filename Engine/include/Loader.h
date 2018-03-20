#pragma once
#include <vector>
#include "Typedefs.h"
#include "LinearMath.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "OSExport.h"


class ENGINE_API Loader {

public:
	Loader();
	~Loader();

	static Texture2D loadRGBATexture2D(const char* filepath);
	static Mesh loadSimpleMesh(const char* filepath);

private:

};