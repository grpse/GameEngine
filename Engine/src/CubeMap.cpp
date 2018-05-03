#include <GL/glew.h>
#include "GLErrorHandling.h"
#include "CubeMap.h"

CubeMap::CubeMap()
	: mID(-1)
{
	mIndex = 0;
}

CubeMap::CubeMap(const CubeMap& other)
{
	mID = other.mID;
	mIndex = other.mIndex;
}

CubeMap::~CubeMap()
{
	// TODO: make a releaser
}

void CubeMap::generateBuffer()
{
	// Generate renderer id buffer, only if there is no generated yet
	if (mID < 0)
	{
		GLCall(glGenTextures(1, (GLuint*)&mID));
	}
}

void CubeMap::setParameter(uint name, uint value) const
{
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, name, value));
}

uint CubeMap::getId() const
{
	return mID;
}

void CubeMap::setIndex(uint index)
{
	mIndex = index;
}

uint CubeMap::getIndex() const
{
	return mIndex;
}

void CubeMap::bind() const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + mIndex));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, mID));
}

void CubeMap::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}