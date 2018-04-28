#pragma once
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

	static uint GetSizeOfType(unsigned int type);
};

class VertexBufferLayout 
{
public:

	struct ElementDescription
	{
		uint count;
		const char* attributeName;
		bool normalized;
	};

	VertexBufferLayout();
	VertexBufferLayout(std::initializer_list<ElementDescription> descriptions);

	void pushUint(uint count, const char* attributeName, bool normalized = false);
	void pushFloat(uint count, const char* attributeName, bool normalized = false);

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