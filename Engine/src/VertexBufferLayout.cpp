#include <GL/glew.h>
#include "VertexBufferLayout.h"

uint VertexBufferElement::GetSizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT: return sizeof(GLfloat);
	case GL_UNSIGNED_INT: return sizeof(GLuint);
	case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
	}
	return 0;
}


VertexBufferLayout::VertexBufferLayout() 
	: mStride(0)
{

}

VertexBufferLayout::VertexBufferLayout(std::initializer_list<ElementDescription> descriptions) : mStride(0)
{
	for (auto& description : descriptions)
	{
		pushFloat(description.count, description.attributeName, description.normalized);
	}
}

void VertexBufferLayout::pushUint(uint count, const char* attributeName, bool normalized)
{
	VertexBufferElement element;
	element.type = GL_UNSIGNED_INT;
	element.count = count;
	element.normalized = normalized ? GL_TRUE : GL_FALSE;
	memcpy(element.attributeName, attributeName, strlen(attributeName) + 1);

	mElements.push_back(element);
	mStride += count * VertexBufferElement::GetSizeOfType(GL_INT);
}

void VertexBufferLayout::pushFloat(uint count, const char* attributeName, bool normalized)
{
	VertexBufferElement element;
	element.type = GL_FLOAT;
	element.count = count;
	element.normalized = normalized ? GL_TRUE : GL_FALSE;
	memcpy(element.attributeName, attributeName, strlen(attributeName) + 1);

	mElements.push_back(element);
	mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}