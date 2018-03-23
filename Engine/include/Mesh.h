#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "LinearMath.h"

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texturecoord0;
};

class Mesh {

public:
	Mesh();
	Mesh(const Mesh& other);
	Mesh(VertexArray& vertexArray, IndexBuffer& indexBuffer);

	~Mesh();

	void load(VertexArray& vertexArray, IndexBuffer& indexBuffer);
	void markAsCopy() const;
	const VertexArray& getVertexArray() const;
	const IndexBuffer& getIndexBuffer() const;
	static Mesh createQuad();

private:
    IndexBuffer mIndexBuffer;
    VertexArray mVertexArray;
	mutable bool mMarkedAsCopy;
};

