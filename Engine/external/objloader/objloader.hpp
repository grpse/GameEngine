#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <Mesh.h>

bool loadOBJ(
	const char * path, 
	std::vector<Vertex>& vertices
);

bool loadOBJ(
	const char * path,
	std::vector<Vector3> & out_vertices,
	std::vector<Vector2> & out_uvs,
	std::vector<Vector3> & out_normals
);

bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif