#pragma once
#include <vector>
#include "Typedefs.h"
#include "LinearMath.h"
#include "Texture2D.h"
#include "CubeMap.h"
#include "Mesh.h"
#include "OSExport.h"

class ENGINE_API Loader {

public:
	Loader();
	~Loader();

	static Texture2D loadRGBATexture2D(const char* filepath);
	static Mesh loadSimpleMesh(const char* filepath);
	static Mesh loadMesh(const char* filepath, float scaleFactor, bool reverseClockwise);
	static Mesh loadMeshAsArray(const char* filepath, float scaleFactor, bool reverseClockwise);
	static Mesh loadMeshAsArrayForDynamic(const char* filepath, float scaleFactor, bool reverseClockwise);
	static CubeMap loadCubeMap(const char* basefilepathname, const std::string& extension);
	static void loadRGBATexture2DSide(const std::string& sidefilepath, uint side_id);
private:

};