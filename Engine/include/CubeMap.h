#pragma once
#include "Typedefs.h"
#include "Texture2D.h"
#include "OSExport.h"

class ENGINE_API CubeMap
{
public:
	CubeMap();
	CubeMap(const CubeMap& other);
	~CubeMap();

	void generateBuffer();
	void setParameter(uint name, uint value) const;
	uint getId() const;
	void setIndex(uint index);
	uint getIndex() const;
	void bind() const;
	void unbind() const;

private:
	int mID;
	int mIndex;
};