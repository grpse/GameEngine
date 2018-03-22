#pragma once

#include <vector>
#include "Typedefs.h"

#define STRINGIFY(value) #value

struct VertexBufferElement
{
    uint type;
    uint count;
    bool normalized;

	static uint GetSizeOfType(unsigned int type);
};

class VertexBufferLayout 
{
public:
	VertexBufferLayout();
	void pushUint(uint count, bool normalized = false);
	void pushFloat(uint count, bool normalized = false);
	const std::vector<VertexBufferElement>& getElements() const;
	uint getStride() const;
    
private:
    std::vector<VertexBufferElement> mElements;
    uint mStride;
};