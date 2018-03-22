#include <GL/glew.h>
#include "VertexArray.h"
#include "GLErrorHandling.h"
#include "VertexBuffer.h"

VertexArray::VertexArray() 
	: mWasGenerated(false)
{

}

VertexArray::VertexArray(const VertexArray & other)
{
	mID = other.mID;
	mWasGenerated = false;
}

VertexArray::~VertexArray() 
{

}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(mID));
    const auto& elements = mVertexBufferLayout.getElements();
    for (uint i = 0; i < elements.size(); i++) 
    {
        GLCall(glEnableVertexAttribArray(i));
    }

}

void VertexArray::unbind() const
{
    const auto& elements = mVertexBufferLayout.getElements();
    for (uint i = 0; i < elements.size(); i++) 
    {
        GLCall(glDisableVertexAttribArray(i));
    }
    GLCall(glBindVertexArray(0));
}

void VertexArray::setVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    mVertexBufferLayout = layout;

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
            element.normalized ? GL_TRUE : GL_FALSE, 
            layout.getStride(),
            (const void*)offset
        ));

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

        GLCall(glDisableVertexAttribArray(i));
    }

    vb.unbind();
    unbind();
}

void VertexArray::generateBuffer()
{
	if (!mWasGenerated)
	{
		mWasGenerated = true;
		GLCall(glGenVertexArrays(1, &mID));
	}
}

void VertexArray::deleteBuffer()
{
	GLCall(glDeleteVertexArrays(1, &mID));
}