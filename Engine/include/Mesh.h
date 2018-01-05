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
    {

    }

    Mesh(VertexArray& vertexArray, IndexBuffer& indexBuffer)
    {
        load(vertexArray, indexBuffer);
    }

    ~Mesh()
    {

    }

    void load(VertexArray& vertexArray, IndexBuffer& indexBuffer)
    {
        mVertexArray = vertexArray;
        mIndexBuffer = indexBuffer;
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

};

