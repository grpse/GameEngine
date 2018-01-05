#include "IndexBuffer.h"
#include "GLErrorHandling.h"

IndexBuffer::IndexBuffer()
{

}

IndexBuffer::IndexBuffer(const uint* data, uint count)
    : mCount(count)
{
    load(data, count);
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &mID));
}

void IndexBuffer::load(const uint* data, uint count)
{
    GLCall(glGenBuffers(1, &mID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID));
}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}