#include <GL/glew.h>
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

Mesh::Mesh() : mMarkedAsCopy(false)
{

}

Mesh::Mesh(const Mesh& other)
{
	mVertexArray = other.mVertexArray;
	mIndexBuffer = other.mIndexBuffer;
	other.mMarkedAsCopy = true;
}

Mesh::Mesh(VertexArray& vertexArray, IndexBuffer& indexBuffer)
{
	load(vertexArray, indexBuffer);
}

Mesh::~Mesh()
{
	if (!mMarkedAsCopy)
	{
	//	mVertexArray.deleteBuffer();
	//	mIndexBuffer.deleteBuffer();
	}
}

void Mesh::load(VertexArray& vertexArray, IndexBuffer& indexBuffer)
{
	mVertexArray = vertexArray;
	mIndexBuffer = indexBuffer;
}

void Mesh::markAsCopy() const
{
	mMarkedAsCopy = true;
}

const VertexArray& Mesh::getVertexArray() const
{
	return mVertexArray;
}

const IndexBuffer& Mesh::getIndexBuffer() const
{
	return mIndexBuffer;
}

Mesh Mesh::createQuad() {
	Mesh quad;
	quad.markAsCopy();

	VertexArray vao;
	VertexBuffer vbo;
	VertexBufferLayout layout;
	IndexBuffer ibo;

	Vertex v1, v2, v3, v4;

	v1.position = { -1, 0, -1 }; v1.texturecoord0 = { 0, 0 };
	v2.position = { 1, 0, -1 }; v2.texturecoord0 = { 1, 0 };
	v3.position = { 1, 0,  1 }; v3.texturecoord0 = { 1, 1 };
	v4.position = { -1, 0,  1 }; v4.texturecoord0 = { 0, 1 };

	v1.normal = { 0, 0, -1 };
	v2.normal = { 0, 0, -1 };
	v3.normal = { 0, 0, -1 };
	v4.normal = { 0, 0, -1 };

	Vertex description[] = {
		v1, v2, v3, v4
	};

	uint indices[] = {
		0, 1, 2, 0, 2, 3
	};

	vbo.load(description, sizeof(description));

	layout.pushFloat(3);
	layout.pushFloat(3);
	layout.pushFloat(2);

	vao.generateBuffer();
	vao.setVertexBuffer(vbo, layout);

	ibo.load(indices, 6);

	quad.load(vao, ibo);

	return quad;
}