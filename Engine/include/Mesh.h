#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "LinearMath.h"

class Renderer;

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texturecoord0;
	
	Vertex(const Vector3& p, const Vector2& uv, const Vector3& n)
	{
		position = p;
		texturecoord0 = uv;
		normal = n;
	}

	Vertex()
	{

	}
};

class Mesh {

public:
	Mesh();
	Mesh(const Mesh& other);
	Mesh(VertexArray& vertexArray, IndexBuffer& indexBuffer);
	Mesh(VertexArray& vertexArray, uint indexStart, uint indexEnd);
	Mesh(VertexArray& vertexArray, uint indexStart, uint indexEnd, std::vector<Vertex> vertices);
	~Mesh();

	void render(const Renderer& renderer) const;
	void load(VertexArray& vertexArray, IndexBuffer& indexBuffer);
	const std::vector<Vertex>& getVertices() const;
	const VertexArray& getVertexArray() const;
	const IndexBuffer& getIndexBuffer() const;

	static Mesh createQuad();

private:
    IndexBuffer mIndexBuffer;
    VertexArray mVertexArray;
	bool mIsIndexed;
	bool mMarkedAsCopy;
	std::vector<Vertex> mVertices;

	uint mIndexStart;
	uint mIndexEnd;

	friend class Renderer;
};

