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
	Mesh()
		: mMarkedAsCopy(false)
    {

    }

	Mesh(const Mesh& other)
	{
		mVertexArray = other.mVertexArray;
		mIndexBuffer = other.mIndexBuffer;
	}

    Mesh(VertexArray& vertexArray, IndexBuffer& indexBuffer)
    {
        load(vertexArray, indexBuffer);
    }

    ~Mesh()
    {
		if (!mMarkedAsCopy)
		{
			mVertexArray.deleteBuffer();
			mIndexBuffer.deleteBuffer();
		}
    }

    void load(VertexArray& vertexArray, IndexBuffer& indexBuffer)
    {
        mVertexArray = vertexArray;
        mIndexBuffer = indexBuffer;
    }

	void markAsCopy()
	{
		mMarkedAsCopy = true;
	}

    const VertexArray& getVertexArray() const
    {
        return mVertexArray;
    }

    const IndexBuffer& getIndexBuffer() const
    {
        return mIndexBuffer;
    }

private:
    IndexBuffer mIndexBuffer;
    VertexArray mVertexArray;
	bool mMarkedAsCopy;
};

