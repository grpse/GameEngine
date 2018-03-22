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

VertexBufferLayout::VertexBufferLayout() : mStride(0) 
{
}

void VertexBufferLayout::pushUint(uint count, bool normalized)
{
	mElements.push_back({ GL_UNSIGNED_INT, count, normalized });
	mStride += count * VertexBufferElement::GetSizeOfType(GL_INT);
}

void VertexBufferLayout::pushFloat(uint count, bool normalized)
{
	mElements.push_back({ GL_FLOAT, count, normalized });
	mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

const std::vector<VertexBufferElement>& VertexBufferLayout::getElements() const 
{ 
	return mElements; 
}

uint VertexBufferLayout::getStride() const 
{ 
	return mStride; 
}