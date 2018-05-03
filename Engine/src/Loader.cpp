#include <GL/glew.h>
#include <SOIL.h>
#include <vector>
#include "Typedefs.h"
#include "LinearMath.h"
#include "Texture2D.h"
#include "Mesh.h"
#include <vector>
#include <objloader/objloader.hpp>
#include <objloader/vboindexer.hpp>
#include "Loader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "GLErrorHandling.h"
#include "AttributesNamesDefines.h"

#define MAX_MATERIAL_COUNT 2

Loader::Loader() 
{

}

Loader::~Loader()
{
	
}

Texture2D Loader::loadRGBATexture2D(const char* filepath)
{
	int width, height, channels;
	byte* imageData = SOIL_load_image(filepath, &width, &height, &channels, SOIL_LOAD_RGBA);
	//byte* imageData = stbi_load(filepath, &width, &height, &channels, 4);

	Texture2DLayout textureLayout;
	textureLayout.target = GL_TEXTURE_2D;
	textureLayout.level = 0;
	textureLayout.internalFormat = GL_RGBA;
	textureLayout.border = 0;
	textureLayout.format = GL_RGBA;
	textureLayout.width = width;
	textureLayout.height = height;
	textureLayout.type = GL_UNSIGNED_BYTE;

	Texture2D texture;
	texture.start();
	texture.loadData(imageData, textureLayout);
	texture.stop();

	SOIL_free_image_data(imageData);
	//stbi_image_free(imageData);

	return texture;
}

Mesh Loader::loadSimpleMesh(const char* filepath)
{
	std::vector<Vertex> vertices;
	loadOBJ(filepath, vertices);

	std::vector<Vertex> out_vertices;
	std::vector<uint> out_indices;

	indexVBO(vertices, out_indices, out_vertices);

	VertexArray vao;
	IndexBuffer ibo;

	vao.createVertexBuffer<Vertex>(out_vertices.data(), out_vertices.size(), {
		{ 3, POSITION, false },
		{ 3, NORMAL, true },
		{ 2, TEXCOORD0, false }
	});

	ibo.load<uint>(out_indices.data(), out_indices.size());

	return Mesh(vao, ibo);
}

Mesh Loader::loadMesh(const char* filepath, float scaleFactor = 1.0f, bool reverseClockwise = false)
{
	Vector3 ambient[MAX_MATERIAL_COUNT], diffuse[MAX_MATERIAL_COUNT], specular[MAX_MATERIAL_COUNT];
	float shine[MAX_MATERIAL_COUNT];
	int material_count, color_index[3], i, NumTris;
	char ch;
	
	FILE* fp = fopen(filepath, "r");
	if (fp == NULL) 
	{ 
		printf("ERROR: unable to open TriObj [%s]!\n", filepath); 
		exit(1); 
	}

	fscanf(fp, "%c", &ch);
	while (ch != '\n') // skip the first line – object’s name
		fscanf(fp, "%c", &ch);
	//
	fscanf(fp, "# triangles = %d\n", &NumTris); // read # of triangles
	fscanf(fp, "Material count = %d\n", &material_count); // read material count
															//
	for (i = 0; i<material_count; i++) 
	{
		fscanf(fp, "ambient color %f %f %f\n", &(ambient[i].x), &(ambient[i].y), &(ambient[i].z));
		fscanf(fp, "diffuse color %f %f %f\n", &(diffuse[i].x), &(diffuse[i].y), &(diffuse[i].z));
		fscanf(fp, "specular color %f %f %f\n", &(specular[i].x), &(specular[i].y), &(specular[i].z));
		fscanf(fp, "material shine %f\n", &(shine[i]));
	}
		
	//
	fscanf(fp, "%c", &ch);
	while (ch != '\n') // skip documentation line
		fscanf(fp, "%c", &ch);
	//
	// allocate triangles for tri model
	//		
	printf("Reading in %s (%d triangles). . .\n", filepath, NumTris);
	
	std::vector<Vertex> vertices;

	//
	for (i = 0; i < NumTris; i++) // read triangles
	{
		Vertex vertice;
		vertice.texturecoord0 = Vector2(0, 0);

		fscanf(fp, "v0 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[0]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "v1 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[1]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "v2 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[2]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "face normal %f %f %f\n", &vertice.normal.x, &vertice.normal.y, &vertice.normal.z);
	}

	fclose(fp);

	if (reverseClockwise) {
		std::reverse(std::begin(vertices), std::end(vertices));
	}

	std::vector<Vertex> out_vertices;
	std::vector<uint> out_indices;

	indexVBO(vertices, out_indices, out_vertices);

	VertexArray vao;
	IndexBuffer ibo;

	vao.createVertexBuffer<Vertex>(out_vertices.data(), out_vertices.size(), {
		{ 3, POSITION, false },
		{ 3, NORMAL, true },
		{ 2, TEXCOORD0, false }
	});
	ibo.load<uint>(out_indices.data(), out_indices.size());
	
	return Mesh(vao, ibo);
}

Mesh Loader::loadMeshAsArray(const char* filepath, float scaleFactor, bool reverseClockwise)
{
	Vector3 ambient[MAX_MATERIAL_COUNT], diffuse[MAX_MATERIAL_COUNT], specular[MAX_MATERIAL_COUNT];
	float shine[MAX_MATERIAL_COUNT];
	int material_count, color_index[3], i, NumTris;
	char ch;

	FILE* fp = fopen(filepath, "r");
	if (fp == NULL)
	{
		printf("ERROR: unable to open TriObj [%s]!\n", filepath);
		exit(1);
	}

	fscanf(fp, "%c", &ch);
	while (ch != '\n') // skip the first line – object’s name
		fscanf(fp, "%c", &ch);
	//
	fscanf(fp, "# triangles = %d\n", &NumTris); // read # of triangles
	fscanf(fp, "Material count = %d\n", &material_count); // read material count
														  //
	for (i = 0; i<material_count; i++)
	{
		fscanf(fp, "ambient color %f %f %f\n", &(ambient[i].x), &(ambient[i].y), &(ambient[i].z));
		fscanf(fp, "diffuse color %f %f %f\n", &(diffuse[i].x), &(diffuse[i].y), &(diffuse[i].z));
		fscanf(fp, "specular color %f %f %f\n", &(specular[i].x), &(specular[i].y), &(specular[i].z));
		fscanf(fp, "material shine %f\n", &(shine[i]));
	}

	//
	fscanf(fp, "%c", &ch);
	while (ch != '\n') // skip documentation line
		fscanf(fp, "%c", &ch);
	//
	// allocate triangles for tri model
	//		
	printf("Reading in %s (%d triangles). . .\n", filepath, NumTris);

	std::vector<Vertex> vertices;

	for (i = 0; i < NumTris; i++) // read triangles
	{
		Vertex vertice;
		vertice.texturecoord0 = Vector2(0, 0);

		fscanf(fp, "v0 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[0]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "v1 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[1]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "v2 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[2]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "face normal %f %f %f\n", &vertice.normal.x, &vertice.normal.y, &vertice.normal.z);
	}

	fclose(fp);

	if (reverseClockwise) {
		std::reverse(std::begin(vertices), std::end(vertices));
	}
	
	VertexArray vao;

	vao.createVertexBuffer<Vertex>(vertices.data(), vertices.size(), {
		{ 3, POSITION, false },
		{ 3, NORMAL, true },
		{ 2, TEXCOORD0, false }
	});

	return Mesh(vao, 0, vertices.size());
}

Mesh Loader::loadMeshAsArrayForDynamic(const char* filepath, float scaleFactor, bool reverseClockwise)
{
	Vector3 ambient[MAX_MATERIAL_COUNT], diffuse[MAX_MATERIAL_COUNT], specular[MAX_MATERIAL_COUNT];
	float shine[MAX_MATERIAL_COUNT];
	int material_count, color_index[3], i, NumTris;
	char ch;

	FILE* fp = fopen(filepath, "r");
	if (fp == NULL)
	{
		printf("ERROR: unable to open TriObj [%s]!\n", filepath);
		exit(1);
	}

	fscanf(fp, "%c", &ch);
	while (ch != '\n') // skip the first line – object’s name
		fscanf(fp, "%c", &ch);
	//
	fscanf(fp, "# triangles = %d\n", &NumTris); // read # of triangles
	fscanf(fp, "Material count = %d\n", &material_count); // read material count
														  //
	for (i = 0; i < material_count; i++)
	{
		fscanf(fp, "ambient color %f %f %f\n", &(ambient[i].x), &(ambient[i].y), &(ambient[i].z));
		fscanf(fp, "diffuse color %f %f %f\n", &(diffuse[i].x), &(diffuse[i].y), &(diffuse[i].z));
		fscanf(fp, "specular color %f %f %f\n", &(specular[i].x), &(specular[i].y), &(specular[i].z));
		fscanf(fp, "material shine %f\n", &(shine[i]));
	}

	//
	fscanf(fp, "%c", &ch);
	while (ch != '\n') // skip documentation line
		fscanf(fp, "%c", &ch);
	//
	// allocate triangles for tri model
	//		
	printf("Reading in %s (%d triangles). . .\n", filepath, NumTris);

	std::vector<Vertex> vertices;

	for (i = 0; i < NumTris; i++) // read triangles
	{
		Vertex vertice;
		vertice.texturecoord0 = Vector2(0, 0);

		fscanf(fp, "v0 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[0]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "v1 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[1]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "v2 %f %f %f %f %f %f %d\n",
			&vertice.position.x, &vertice.position.y, &vertice.position.z,
			&vertice.normal.x, &vertice.normal.y, &vertice.normal.z,
			&(color_index[2]));

		vertice.position.x *= scaleFactor;
		vertice.position.y *= scaleFactor;
		vertice.position.z *= scaleFactor;

		vertices.push_back(vertice);

		fscanf(fp, "face normal %f %f %f\n", &vertice.normal.x, &vertice.normal.y, &vertice.normal.z);
	}

	fclose(fp);

	if (reverseClockwise) {
		std::reverse(std::begin(vertices), std::end(vertices));
	}

	VertexArray vao;

	vao.createVertexBuffer<Vertex>(vertices.data(), vertices.size(), {
		{ 3, POSITION, false },
		{ 3, NORMAL, true },
		{ 2, TEXCOORD0, false }
		});

	return Mesh(vao, 0, vertices.size(), vertices);
}
CubeMap Loader::loadCubeMap(const char* basefilepathname, const std::string& extension)
{
	std::string basepath = basefilepathname;

	CubeMap cubeMap;
	cubeMap.generateBuffer();

	cubeMap.bind();
	
	Loader::loadRGBATexture2DSide(basepath + "_front." + extension, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	Loader::loadRGBATexture2DSide(basepath + "_back." + extension, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	Loader::loadRGBATexture2DSide(basepath + "_top." + extension, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	Loader::loadRGBATexture2DSide(basepath + "_bottom." + extension, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	Loader::loadRGBATexture2DSide(basepath + "_left." + extension, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	Loader::loadRGBATexture2DSide(basepath + "_right." + extension, GL_TEXTURE_CUBE_MAP_POSITIVE_X);

	cubeMap.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	cubeMap.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	cubeMap.setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	cubeMap.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	cubeMap.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	cubeMap.unbind();

	return cubeMap;
}

void Loader::loadRGBATexture2DSide(const std::string& sidefilepath, uint side_target)
{
	int width, height, channels;
	const char* filepath = sidefilepath.c_str();
	byte* imageData = SOIL_load_image(filepath, &width, &height, &channels, SOIL_LOAD_RGBA);
	
	// copy image data into 'target' side of cube map
	GLCall(glTexImage2D(
		side_target,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		imageData));

	SOIL_free_image_data(imageData);
}
