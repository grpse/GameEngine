#include "Mesh.h"

Mesh::Mesh()
	: mMarkedAsCopy(false)
{
	mIsIndexed = false;
}

Mesh::Mesh(const Mesh& other)
{
	mVertexArray = other.mVertexArray;
	mIndexBuffer = other.mIndexBuffer;
	mIsIndexed = other.mIsIndexed;
	mIndexStart = other.mIndexStart;
	mIndexEnd = other.mIndexEnd;
}

Mesh::Mesh(VertexArray& vertexArray, IndexBuffer& indexBuffer)
{
	load(vertexArray, indexBuffer);
}

Mesh::Mesh(VertexArray& vertexArray, uint indexStart, uint indexEnd)
{
	mIsIndexed = false;
	mVertexArray = vertexArray;
	mIndexStart = indexStart;
	mIndexEnd = indexEnd;
}

Mesh::Mesh(VertexArray& vertexArray, uint indexStart, uint indexEnd, std::vector<Vertex> vertices)
{
	mIsIndexed = false;
	mVertexArray = vertexArray;
	mIndexStart = indexStart;
	mIndexEnd = indexEnd;
	mVertices = vertices;
}

Mesh::~Mesh()
{
}

void Mesh::render(const Renderer& renderer) const
{
	if (mIsIndexed)
	{
		renderer.render(mVertexArray, mIndexBuffer);
	}
	else
	{
		renderer.render(mVertexArray, mIndexStart, mIndexEnd);
	}

}

void Mesh::load(VertexArray& vertexArray, IndexBuffer& indexBuffer)
{
	mVertexArray = vertexArray;
	mIndexBuffer = indexBuffer;
	mIsIndexed = true;
}

const std::vector<Vertex>& Mesh::getVertices() const
{
	return mVertices;
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
	VertexArray vao;
	IndexBuffer ibo;

	Vertex v1, v2, v3, v4;

	v1.position = { -1, 0, -1 };
	v2.position = { 1, 0, -1 };
	v3.position = { 1, 0,  1 };
	v4.position = { -1, 0,  1 };

	v1.texturecoord0 = { 0, 0 };
	v2.texturecoord0 = { 1, 0 };
	v3.texturecoord0 = { 1, 1 };
	v4.texturecoord0 = { 0, 1 };

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

	vao.createVertexBuffer<Vertex>(description, 4, {
		{ 3, POSITION, false },
		{ 3, NORMAL, true },
		{ 2, TEXCOORD0, false }
		});

	ibo.load<uint>(indices, 6);

	return Mesh(vao, ibo);
}