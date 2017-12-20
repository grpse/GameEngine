#pragma once
#include "LinearMath.h"
#include "Transform.h"

class Camera {
public:
	Transform transform;
	Matrix4 createViewMatrix() const;

	void printPositionAndTarget();
};