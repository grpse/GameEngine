#pragma once
#include "LinearMath.h"

class Camera {
public:
	void setPostion(Vector3 position);
	void setTarget(Vector3 target);
	
	const Vector3& getPosition() const;
	const Vector3& getTarget() const;

	const Matrix4& createViewMatrix() const;

private:
	Matrix4 mViewMatrix;
	Vector3 mPosition;
	Vector3 mTarget;
	
};