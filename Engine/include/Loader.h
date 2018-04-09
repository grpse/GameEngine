#pragma once
#include <vector>
#include "Typedefs.h"
#include "LinearMath.h"
#include "Texture2D.h"
#include "CubeMap.h"
#include "Mesh.h"

class Loader {

public:
	Loader();
	~Loader();

	static Texture2D loadRGBATexture2D(const char* filepath);
	static Mesh loadSimpleMesh(const char* filepath);
	static Mesh loadMesh(const char* filepath);
	static Mesh loadMeshAsArray(const char* filepath);
	static CubeMap loadCubeMap(const char* basefilepathname, const std::string& extension);
	static void loadRGBATexture2DSide(const std::string& sidefilepath, uint side_id);
private:

};