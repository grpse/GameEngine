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
	int attributeLocation = -1;
	char attributeName[32] = "";

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
    VertexBufferLayout() : mStride(0) {
		
	}

    void pushUint(uint count, const char* attributeName, bool normalized = false)
    {
		VertexBufferElement element;
		element.type = GL_UNSIGNED_INT;
		element.count = count;
		element.normalized = normalized ? GL_TRUE : GL_FALSE;
		memcpy(element.attributeName, attributeName, strlen(attributeName) + 1);

        mElements.push_back(element);
        mStride += count * VertexBufferElement::GetSizeOfType(GL_INT);
    }

    void pushFloat(uint count, const char* attributeName, bool normalized = false)
    {
		VertexBufferElement element;
		element.type = GL_FLOAT;
		element.count = count;
		element.normalized = normalized ? GL_TRUE : GL_FALSE;
		memcpy(element.attributeName, attributeName, strlen(attributeName) + 1);

        mElements.push_back(element);
        mStride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

	inline void replaceElementsWith(const std::vector<VertexBufferElement>& newElements)
	{
		mElements = newElements;
	}

    inline const std::vector<VertexBufferElement>& getElements() const  { return mElements; }
    inline uint getStride() const { return mStride; }
    
private:
    std::vector<VertexBufferElement> mElements;
    uint mStride;
};