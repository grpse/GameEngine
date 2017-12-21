#include "IndexBuffer.h"
#include "GLErrorHandling.h"

IndexBuffer::IndexBuffer(const uint* data, uint count)
    : mCount(count)
{
    GLCall(glGenBuffers(1, &mID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &mID));
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID));
}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}