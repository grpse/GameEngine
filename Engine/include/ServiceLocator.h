#pragma once
#include "OSExport.h"

template <class Type>
class ENGINE_API ServiceLocator {

public:
	static Type& getService()
	{
		return mService;
	}

public:
	static Type mService;
};