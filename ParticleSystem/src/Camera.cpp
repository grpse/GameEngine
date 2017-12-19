#include "Camera.h"

void Camera::setPostion(Vector3 position)
{
}

void Camera::setTarget(Vector3 target)
{
}

const Vector3 & Camera::getPosition() const
{
	// TODO: insert return statement here
	return mPosition;
}

const Vector3 & Camera::getTarget() const
{
	// TODO: insert return statement here
	return mTarget;
}

const Matrix4 & Camera::createViewMatrix() const
{
	// TODO: insert return statement here
	return mViewMatrix;
}
