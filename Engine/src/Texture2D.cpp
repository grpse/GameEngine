#include <GL/glew.h>
#include "Texture2D.h"
#include "GLErrorHandling.h"

Texture2D::Texture2D()
{
	mWasCopied = false;
    GLCall(glGenTextures(1, &mID));
    setupDefaults();
}

Texture2D::Texture2D(uint id)
{
	mWasCopied = false;
    mID = id;
    setupDefaults();
}

Texture2D::Texture2D(const Texture2D& other)
{
    mID = other.mID;
    mIndex = other.mIndex;
	mIndex = other.mIndex;
	mLayout = other.mLayout;
	other.mWasCopied = true;
}

Texture2D::~Texture2D()
{
	if (!mWasCopied)
	{
		GLCall(glDeleteTextures(1, &mID));
	}
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
    mID = other.mID;
    mIndex = other.mIndex;
	mLayout = other.mLayout;
	other.mWasCopied = true;
    return *this;
}

void Texture2D::setIndex(uint index)
{
    mIndex = index;
}

uint Texture2D::getIndex() const 
{
    return mIndex;
}

void Texture2D::setParameter(uint name, uint value)
{
    GLCall(glTexParameteri(GL_TEXTURE_2D, name, value));
}

void Texture2D::loadData(const void* data, const Texture2DLayout& layout)
{
	mLayout = layout;
    GLCall(glTexImage2D(
        mLayout.target, 
        mLayout.level, 
		mLayout.internalFormat,
        mLayout.width, 
        mLayout.height, 
        mLayout.border, 
        mLayout.format, 
        mLayout.type, 
        data
    ));
}

void Texture2D::generateMipMaps()
{
    GLCall(glGenerateMipmap(mLayout.target));
}

void Texture2D::bind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + mIndex));
    GLCall(glBindTexture(GL_TEXTURE_2D, mID));
}

void Texture2D::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

uint Texture2D::getWidth() const 
{
    return mLayout.width;
}

uint Texture2D::getHeight() const 
{
    return mLayout.height;
}

uint Texture2D::getId() const 
{
	return mID;
}

void Texture2D::setupDefaults()
{
    mIndex = 0;
}