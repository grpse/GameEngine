#pragma once
#include <vector>
#include "Typedefs.h"
#include "RawModel.h"
#include "LinearMath.h"

class Loader {

public:
	Loader();
	~Loader();
	RawModel loadToVAO(void* data, uint itemSizeInBytes, uint itemCount);

private:
	std::vector<uint> mLoadedVAOS;
	std::vector<uint> mLoadedVBOs;

	uint createVAO();
	void storeDataInAttributeList(int attributeNumber, void* data, uint sizeInBytes);

	void unbindVAO();

};