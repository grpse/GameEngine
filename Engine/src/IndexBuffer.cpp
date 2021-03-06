#include "IndexBuffer.h"
#include "GLErrorHandling.h"

IndexBuffer::IndexBuffer()
{

}

IndexBuffer::IndexBuffer(const IndexBuffer& other)
{
	mID = other.mID;
	mCount = other.mCount;
}

IndexBuffer::~IndexBuffer()
{
	//GLCall(glDeleteBuffers(1, &mID));
}

void IndexBuffer::loadData(const void* data, size_t count, size_t typeSize)
{
	mCount = count;
	GLCall(glGenBuffers(1, &mID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * typeSize, data, GL_STATIC_DRAW));
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID));
}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}