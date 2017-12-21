#include <GL/glew.h>
#include "VertexArray.h"
#include "GLErrorHandling.h"

VertexArray::VertexArray() 
{
    GLCall(glGenVertexArrays(1, &mID));
}

VertexArray::~VertexArray() 
{
    GLCall(glDeleteVertexArrays(1, &mID));
}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(mID));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    bind();
    vb.bind();

    const auto& elements = layout.getElements();
    uint offset = 0;

    for (uint i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];

        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(
            i, 
            element.count, 
            element.type, 
            element.normalized, 
            layout.getStride(),
            (const void*)offset         
        ));

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}