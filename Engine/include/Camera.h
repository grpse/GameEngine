#pragma once
#include "LinearMath.h"
#include "Transform.h"

class Camera {
public:
	Camera();
	Transform transform;
	const Matrix4 getViewMatrix() const;

private:
	mutable Matrix4 mViewMatrix;
};