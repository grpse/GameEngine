#include "VertexBuffer.h"
#include "GLErrorHandling.h"

VertexBuffer::VertexBuffer()
{

}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
{
	mID = other.mID;
}

VertexBuffer::~VertexBuffer()
{
	//TODO: Discover if deleting Vertex Array Object, deletes the Vertex Buffer Object
	//GLCall(glDeleteBuffers(1, &mID));
}

void VertexBuffer::load(const void* data, uint size)
{
    GLCall(glGenBuffers(1, &mID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::loadDynamic(const void* data, uint size)
{
	GLCall(glGenBuffers(1, &mID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW));
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mID));
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}