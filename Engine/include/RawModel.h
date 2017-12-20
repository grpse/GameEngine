#pragma once
#include "Typedefs.h"

class RawModel {

public:
	inline RawModel(uint vaoID, uint vertexCount) {
		mVaoID = vaoID;
		mVertexCount = vertexCount;
	}

	inline uint getVaoID() const { 
		return mVaoID; 
	}
	
	inline uint getVertexCount() const { 
		return mVertexCount; 
	}

private:
	uint mVaoID;
	uint mVertexCount;
};