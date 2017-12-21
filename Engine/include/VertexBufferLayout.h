#pragma once
#include <GL/glew.h>
#include <vector>
#include "Typedefs.h"

#define STRINGIFY(value) #value

struct VertexBufferElement
{
    uint type;
    uint count;
    uint normalized;

    inline static uint GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT: return sizeof(GLfloat);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
        }
        return 0;
    }
};

class VertexBufferLayout 
{
public:
    VertexBufferLayout() : mStride(0) {}

    template<uint type, uint normalized>
    void push(uint count)
    {
        mElements.push_back({type, count, normalized});
        mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    inline const std::vector<VertexBufferElement>& getElements() const  { return mElements; }
    inline uint getStride() const { return mStride; }

private:
    std::vector<VertexBufferElement> mElements;
    uint mStride;
};