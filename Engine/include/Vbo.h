#pragma once
#include "Typedefs.h"

class Vbo {

public:
	static Vbo create(uint type, uint usage);
	
	Vbo();
	Vbo(uint vboId, uint type, uint usage);
	~Vbo();

	void bind();
	void unbind();

	void allocate(uint sizeIsBytes);
	void storeData(void * data, uint sizeInBytes, uint startInBytes);
	uint getSizeInBytes();

private:
	uint mVboId;
	uint mType;
	uint mUsage;
	uint mSizeInBytes;
};