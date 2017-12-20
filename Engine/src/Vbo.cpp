#include <GL/glew.h>
#include "Vbo.h"
#include "GLErrorHandling.h"

Vbo Vbo::create(uint type, uint usage)
{
	uint vboId;
	GLCall(glGenBuffers(1, &vboId));
	return Vbo(vboId, type, usage);
}

Vbo::Vbo()
{
}

Vbo::Vbo(uint vboId, uint type, uint usage)
	: mVboId(vboId), mType(type), mUsage(usage)
{
	bind();
}

Vbo::~Vbo()
{
	GLCall(glDeleteBuffers(1, &mVboId));
}

void Vbo::bind()
{
	GLCall(glBindBuffer(mType, mVboId));
}

void Vbo::unbind()
{
	GLCall(glBindBuffer(mType, 0));
}

void Vbo::allocate(uint sizeIsBytes)
{
	GLCall(glBufferData(mType, sizeIsBytes, 0, mUsage));
}

void Vbo::storeData(void * data, uint sizeInBytes, uint startInBytes)
{
	GLCall(glBufferSubData(mType, startInBytes, sizeInBytes, data));
}

uint Vbo::getSizeInBytes()
{
	return mSizeInBytes;
}
