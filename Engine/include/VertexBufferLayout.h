#pragma once
#include <GL/glew.h>
#include <vector>
#include "Typedefs.h"

#define STRINGIFY(value) #value

struct VertexBufferElement
{
    uint type;
    uint count;
    bool normalized;

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

    void pushUint(uint count, bool normalized = false)
    {
        mElements.push_back({GL_UNSIGNED_INT, count, normalized});
        mStride += count * VertexBufferElement::GetSizeOfType(GL_INT);
    }

    void pushFloat(uint count, bool normalized = false)
    {
        mElements.push_back({GL_FLOAT, count, normalized});
        mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    inline const std::vector<VertexBufferElement>& getElements() const  { return mElements; }
    inline uint getStride() const { return mStride; }
    
private:
    std::vector<VertexBufferElement> mElements;
    uint mStride;
};