#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "LinearMath.h"
#include "Renderer.h"
#include <iostream>

#pragma pack(push, 1)

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
#pragma pack(pop)

class Mesh {

public:
	Mesh()
		: mMarkedAsCopy(false)
    {
		mIsIndexed = false;
    }

	Mesh(const Mesh& other)
	{
		mVertexArray = other.mVertexArray;
		mIndexBuffer = other.mIndexBuffer;
		mIsIndexed = other.mIsIndexed;
		mIndexStart = other.mIndexStart;
		mIndexEnd = other.mIndexEnd;
	}

    Mesh(VertexArray& vertexArray, IndexBuffer& indexBuffer)
    {
        load(vertexArray, indexBuffer);
    }

	Mesh(VertexArray& vertexArray)
	{
		mIsIndexed = false;
		mVertexArray = vertexArray;
	}

    ~Mesh()
    {
		if (!mMarkedAsCopy)
		{
			//mVertexArray.deleteBuffer();
			//mIndexBuffer.deleteBuffer();
		}
    }

	void render(const Renderer& renderer) const
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

    void load(VertexArray& vertexArray, IndexBuffer& indexBuffer)
    {
        mVertexArray = vertexArray;
        mIndexBuffer = indexBuffer;
		mIsIndexed = true;
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

	static Mesh createQuad() {
		Mesh quad;
		quad.markAsCopy();

		VertexArray vao;
		VertexBuffer vbo;
		VertexBufferLayout layout;
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

		vbo.load(description, sizeof(description));

		layout.pushFloat(3);
		layout.pushFloat(3);
		layout.pushFloat(2);

		vao.generateBuffer();
		vao.setVertexBuffer(vbo, layout);

		ibo.load<uint>(indices, 6);

		quad.load(vao, ibo);

		return quad;
	}

	uint mIndexStart;
	uint mIndexEnd;

private:
    IndexBuffer mIndexBuffer;
    VertexArray mVertexArray;
	bool mIsIndexed;
	bool mMarkedAsCopy;
};

