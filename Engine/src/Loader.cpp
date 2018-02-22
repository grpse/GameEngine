#include <GL/glew.h>
#include <SOIL.h>
#include <vector>
#include <objloader/objloader.hpp>
#include <objloader/vboindexer.hpp>
#include "Loader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "GLErrorHandling.h"

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

	return texture;
}

Mesh Loader::loadSimpleMesh(const char* filepath)
{
	Mesh mesh;
	std::vector<Vertex> file_vertices;
	loadOBJ(filepath, file_vertices);

	std::vector<Vertex> mesh_vertices;
	std::vector<unsigned int> mesh_indices;

	indexVBO(file_vertices, mesh_indices, mesh_vertices);

	VertexArray vao;
	VertexBuffer vbo;
	VertexBufferLayout layout;
	IndexBuffer ibo;

	vbo.load(mesh_vertices.data(), mesh_vertices.size() * sizeof(Vertex));
	layout.pushFloat(3);
	layout.pushFloat(3, true);
	layout.pushFloat(2);

	vao.generateBuffer();
	vao.setVertexBuffer(vbo, layout);

	ibo.load(mesh_indices.data(), mesh_indices.size());

	mesh.markAsCopy();
	mesh.load(vao, ibo);

	return mesh;
}