#pragma once
#include "Typedefs.h"
#include "Texture2D.h"

class CubeMap
{
public:
	CubeMap();
	CubeMap(const CubeMap& other);
	~CubeMap();

	void setParameter(uint name, uint value) const;
	uint getId() const;
	void setIndex(uint index);
	uint getIndex() const;
	void bind() const;
	void unbind() const;

private:
	uint mID;
	uint mIndex;
};