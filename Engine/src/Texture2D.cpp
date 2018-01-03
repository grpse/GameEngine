#include "Texture2D.h"
#include "GLErrorHandling.h"

Texture2D::Texture2D()
{
    GLCall(glGenTextures(1, &mID));
    setupDefaults();
}

Texture2D::Texture2D(uint id)
{
    mID = id;
    setupDefaults();
}

Texture2D::Texture2D(const Texture2D& other)
{
    mID = other.mID;
    mIndex = other.mIndex;
    mWidth = other.mWidth;
    mHeight = other.mHeight;
    mFormat = other.mFormat;
}

void Texture2D::setIndex(uint index)
{
    mIndex = index;
}

uint Texture2D::getIndex() const 
{
    return mIndex;
}

void Texture2D::setFormat(uint format)
{
    mFormat = format;
}

uint Texture2D::getFormat() const 
{
    return mFormat;
}

void Texture2D::setParameter(uint name, uint value)
{
    GLCall(glTexParameteri(GL_TEXTURE_2D, name, value));
}

void Texture2D::loadData(const void* data, uint width, uint height)
{
    mWidth = width;
    mHeight = height;
    GLCall(glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        GL_RGBA, 
        width, 
        height, 
        0, 
        mFormat, 
        GL_UNSIGNED_BYTE, 
        data
    ));
}

void Texture2D::generateMipMaps()
{
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture2D::start() 
{
    GLCall(glActiveTexture(GL_TEXTURE0 + mIndex));
    GLCall(glBindTexture(GL_TEXTURE_2D, mID));
}

void Texture2D::stop()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

uint Texture2D::getWidth() const 
{
    return mWidth;
}

uint Texture2D::getHeight() const 
{
    return mHeight;
}

void Texture2D::setupDefaults()
{
    mFormat = GL_RGBA;
    mIndex = 0;
}